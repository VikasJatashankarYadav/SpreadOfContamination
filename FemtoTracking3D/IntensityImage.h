
#ifndef PX_INTENSITY_IMAGE_H
#define PX_INTENSITY_IMAGE_H

#include <iostream>
#include <iomanip>
#include <limits>
#include <memory>
#include <fstream>
#include <png.h>
#include <vector>

#include "DataFrame.h"

namespace px {

template <typename ImageDataType>
class Kernel;

template <typename ImageDataType>
class IntensityImage;

template <typename ImageDataType>
std::ostream& operator << (std::ostream& out, const IntensityImage<ImageDataType>& image);

template <typename ImageDataType>
class IntensityImage : public DataFrame<ImageDataType> {
public:
    IntensityImage();
    IntensityImage(const IntensityImage<ImageDataType>& image);
    IntensityImage(std::size_t width, std::size_t height);
    IntensityImage(const ImageDataType* const data, std::size_t width, std::size_t height);
    virtual ~IntensityImage();

	bool loadFromPNG(const std::string& filename, bool bNormalize = false);
	bool saveAsPNG(const std::string& filename);
	bool saveAsPPM(const std::string& filename);

	bool uniform(ImageDataType value);
	bool zeromean();
	bool random();

	bool normalize(const ImageDataType& scaleMax);
	bool normalize(const ImageDataType& scaleMin, const ImageDataType& scaleMax);
    bool invert();
	bool scale(ImageDataType minScale, ImageDataType maxScale);
	bool mask(std::shared_ptr<IntensityImage<ImageDataType> >& mask) const;

	bool apply(const Kernel<ImageDataType>& kernel, IntensityImage<ImageDataType>& image, bool divide = false) const;
	bool applyKernel(const Kernel<ImageDataType>& kernel, IntensityImage<ImageDataType>& image, bool divide = false) const;
	bool applyKernel(const Kernel<ImageDataType>& kernel, std::shared_ptr<IntensityImage<ImageDataType> >& image, bool divide = false) const;
	bool evaluate(const Kernel<ImageDataType>& kernel, IntensityImage<ImageDataType>& image, bool divide = false) const;
	bool evaluate(const Kernel<ImageDataType>& kernel, std::shared_ptr<IntensityImage<ImageDataType> >& image, bool divide = false) const;

	bool getMinMax(ImageDataType& min, ImageDataType& max) const;
	ImageDataType getMin() const;
	ImageDataType getMax() const;

	friend std::ostream& operator << <> (std::ostream& out, const IntensityImage<ImageDataType>& image);
};

const double REAL_IMAGE_MIN = double(0);
const double REAL_IMAGE_MAX = double(1);
const double IMAGE_MIN = double(0);
const double IMAGE_MAX = double(255);

template <typename ImageDataType>
IntensityImage<ImageDataType>::IntensityImage() : DataFrame<ImageDataType>() {}

template <typename ImageDataType>
IntensityImage<ImageDataType>::IntensityImage(const IntensityImage<ImageDataType>& image) : DataFrame<ImageDataType>() {
	this->w = image.w;
	this->h = image.h;
	this->stamp = image.stamp;
	this->data = new ImageDataType[this->w * this->h];
	std::memcpy(this->data, image.data, this->w * this->h * sizeof(ImageDataType));
}

template <typename ImageDataType>
IntensityImage<ImageDataType>::IntensityImage(std::size_t width, std::size_t height) : DataFrame<ImageDataType>(width, height) {}

template <typename ImageDataType>
IntensityImage<ImageDataType>::IntensityImage(const ImageDataType* const data, std::size_t width, std::size_t height) : DataFrame<ImageDataType>(data, width, height) {}

template <typename ImageDataType>
IntensityImage<ImageDataType>::~IntensityImage() {}

template <typename ImageDataType>
bool IntensityImage<ImageDataType>::loadFromPNG(const std::string& filename, bool bNormalize) {
	if ( filename.length() == 0 ) return false;

	const uint8_t PNG_SCALE = static_cast<uint8_t>(255);
	const double PNG_INV_SCALE = double(1) / double(PNG_SCALE);
	const double RCONV = 0.2989;
	const double GCONV = 0.5870;
	const double BCONV = 0.1140;

	FILE* fp = fopen(filename.c_str(), "rb");
    auto png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if ( !png_ptr ) { std::cerr << "[IntensityImage:loadFromPNG] Error: Loading PNG pointer." << std::endl; return false; }
	auto info_ptr = png_create_info_struct(png_ptr);
	if ( !info_ptr ) { std::cerr << "[IntensityImage:loadFromPNG] Error: Loading PNG info." << std::endl; return false; }

	if ( setjmp(png_jmpbuf(png_ptr)) ) return false;

	png_init_io(png_ptr, fp);
	png_read_info(png_ptr, info_ptr);

	uint32_t width = png_get_image_width(png_ptr, info_ptr);
	uint32_t height = png_get_image_height(png_ptr, info_ptr);
	auto color_type = png_get_color_type(png_ptr, info_ptr);
	auto bit_depth  = png_get_bit_depth(png_ptr, info_ptr);

	if ( bit_depth != 8 ) {
		std::cerr << "[IntensityImage:loadFromPNG] Error: Only supports 8-Bit PNG images." << std::endl;
		return false;
	}

	png_read_update_info(png_ptr, info_ptr);
	png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
	for ( uint32_t i = 0; i < height; i++ )
		row_pointers[i] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));
	png_read_image(png_ptr, row_pointers);

	this->resize(width, height);
	
	if ( color_type == PNG_COLOR_TYPE_GRAY ) {
		std::size_t index;
		for ( std::size_t i = 0; i < height; i++ ) {
			for ( std::size_t j = 0; j < width; j++ ) {
				index = i * width + j;
				if ( bNormalize ) this->data[index] = row_pointers[i][j] * PNG_INV_SCALE;
				else this->data[index] = row_pointers[i][j];
			}
		}
	}
	else if ( color_type == PNG_COLOR_TYPE_RGB ) {
		const uint8_t CMP = 3;
		std::size_t index, cmp_index;
		float color[CMP];

		for ( std::size_t i = 0; i < height; i++ ) {
			for ( std::size_t j = 0; j < width; j++ ) {
				index = i * width + j;
				cmp_index = i * (width*CMP) + (j*CMP);

				for ( std::size_t c = 0; c < CMP; c++ ) {
					if ( bNormalize ) color[c] = row_pointers[i][j*CMP+c] * PNG_INV_SCALE;
					else color[c] = row_pointers[i][j*CMP+c];
				}

				// https://www.mathworks.com/help/matlab/ref/rgb2gray.html
				this->data[index] = RCONV * color[0] + GCONV * color[1] + BCONV * color[2];
			}
		}
	}
	else if ( color_type == PNG_COLOR_TYPE_RGBA ) {
		const uint8_t CMP = 4;
		std::size_t index, cmp_index;
		float color[CMP];

		for ( std::size_t i = 0; i < height; i++ ) {
			for ( std::size_t j = 0; j < width; j++ ) {
				index = i * width + j;
				cmp_index = i * (width*CMP) + (j*CMP);

				for ( std::size_t c = 0; c < CMP; c++ ) {
					if ( bNormalize ) color[c] = row_pointers[i][j*CMP+c] * PNG_INV_SCALE;
					else color[c] = row_pointers[i][j*CMP+c];
				}

				// https://www.mathworks.com/help/matlab/ref/rgb2gray.html
				this->data[index] = RCONV * color[0] + GCONV * color[1] + BCONV * color[2];
			}
		}
	}
	else {
		std::cerr << "[IntensityImage:loadFromPNG] Error: Unsupported color type (libpng:" << color_type << ")." << std::endl;
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(fp);
		return false;
	}

    png_destroy_read_struct(&png_ptr, NULL, NULL);
	fclose(fp);

	return true;
}

template <typename ImageDataType>
bool IntensityImage<ImageDataType>::saveAsPNG(const std::string& filename) {
	if ( filename.length() == 0 ) return false;
	if ( this->data == nullptr ) return false;
    if ( this->w == 0 || this->h == 0 ) return false;

	const uint8_t PNG_SCALE = static_cast<uint8_t>(255);

	FILE* fp = fopen(filename.c_str(), "wb");
    auto png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	auto info_ptr = png_create_info_struct(png_ptr);
    png_init_io(png_ptr, fp);

	png_set_IHDR(png_ptr, info_ptr, this->w, this->h, 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png_ptr, info_ptr);

	if constexpr ( std::is_integral<ImageDataType>::value ) {
		auto row_ptr = this->data;
		for ( std::size_t i = 0; i < this->h; i++ ) {
			png_write_row(png_ptr, row_ptr);
			row_ptr += this->w;
		}
	}
	// Internal representation is floating type
	else {
		bool bNormalized = false;
		if ( this->getMax() <= ImageDataType(1) ) bNormalized = true;
		else bNormalized = false;

		std::size_t n = this->w * this->h;
		std::vector<uint8_t> image(n);
		for ( std::size_t i = 0; i < n; i++ ) {
			if ( bNormalized ) image[i] = this->data[i] * PNG_SCALE;
			else image[i] = this->data[i];
		}

		auto row_ptr = &image[0];
		for ( std::size_t i = 0; i < this->h; i++ ) {
			png_write_row(png_ptr, row_ptr);
			row_ptr += this->w;
		}
	}

    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
	return true;
}

template <typename ImageDataType>
bool IntensityImage<ImageDataType>::saveAsPPM(const std::string& filename) {
	if ( filename.length() == 0 ) return false;
	if ( this->data == nullptr ) return false;
    if ( this->w == 0 || this->h == 0 ) return false;

	const uint8_t PPM_SCALE = static_cast<uint8_t>(255);
	const char PPM_DELIMITER = ' ';

	std::ofstream out;
	out.open(filename.c_str());

	if ( !out.is_open() ) {
		std::cerr << "[IntensityImage:saveAsPPM] Error: Could not open file: " << filename << std::endl;
		return false;
	}

	out << "P3" << std::endl;
	out << this->w << PPM_DELIMITER << this->h << std::endl;
	out << PPM_SCALE << std::endl;

	bool bNormalized = false;
	if ( this->getMax() > REAL_IMAGE_MAX ) bNormalized = false;
	else bNormalized = true;

	for ( std::size_t i = 0; i < this->h; i++ ) {
		for ( std::size_t j = 0; j < this->w; j++ ) {
			ImageDataType value = this->data[i * this->w + j];

			if ( bNormalized == true ) {
				unsigned int iValue = static_cast<unsigned char>(value * PPM_SCALE);
				if ( j == this->w - 1 ) out << iValue << PPM_DELIMITER << iValue << PPM_DELIMITER << iValue << std::endl;
				else out << iValue << PPM_DELIMITER << iValue << PPM_DELIMITER << iValue << PPM_DELIMITER;
			}
			else {
				unsigned int iValue = static_cast<unsigned char>(value);
				if ( j == this->w - 1 ) out << iValue << PPM_DELIMITER << iValue << PPM_DELIMITER << iValue << std::endl;
				else out << iValue << PPM_DELIMITER << iValue << PPM_DELIMITER << iValue << PPM_DELIMITER;
			}
		}
	}

	out.close();
	return true;
}

template <typename ImageDataType>
bool IntensityImage<ImageDataType>::uniform(ImageDataType value) {
	if ( this->data == nullptr ) return false;
	std::size_t n = this->w * this->h;
	for ( std::size_t i = 0; i < n; i++ )
		this->data[i] = value;
	return true;
}

template <typename ImageDataType>
bool IntensityImage<ImageDataType>::zeromean() {
	if ( this->data == nullptr ) return false;

	std::size_t n = this->w * this->h;
	double mean = double(0);
	for ( std::size_t i = 0; i < n; i++ )
		mean += this->data[i];
	mean /= static_cast<double>(n);

	for ( std::size_t i = 0; i < n; i++ )
		this->data[i] -= mean;

	return true;
}

template <typename ImageDataType>
bool IntensityImage<ImageDataType>::random() {
	if ( this->data == nullptr ) return false;
	std::size_t n = this->w * this->h;
	for ( std::size_t i = 0; i < n; i++ ) {
		if ( std::is_floating_point<ImageDataType>::value ) this->data[i] = (double)rand() / (double)RAND_MAX;
		else this->data[i] = static_cast<ImageDataType>(rand());
	}
	return true;
}

template <typename ImageDataType>
bool IntensityImage<ImageDataType>::normalize(const ImageDataType& scaleMax) {
	if ( this->data == nullptr ) return false;
	if ( this->w == 0 || this->h == 0 ) return false;

	ImageDataType maximum = this->getMax();
	for ( std::size_t i = 0; i < this->h; i++ ) {
		for ( std::size_t j = 0; j < this->w; j++ ) {
			ImageDataType value = this->data[i * this->w + j];
			double dv = value / static_cast<double>(maximum);
			value = static_cast<ImageDataType>(dv * scaleMax);
			this->data[i * this->w + j] = value;
		}
	}

	return true;
}

template <typename ImageDataType>
bool IntensityImage<ImageDataType>::normalize(const ImageDataType& scaleMin, const ImageDataType& scaleMax) {
	if ( this->data == nullptr ) return false;
	if ( this->w == 0 || this->h == 0 ) return false;

	ImageDataType minimum, maximum;
	if ( !this->getMinMax(minimum, maximum) ) return false;
	double sourceMin = static_cast<double>(minimum);
	double sourceMax = static_cast<double>(maximum);
	double targetMin = static_cast<double>(scaleMin);
	double targetMax = static_cast<double>(scaleMax);

	double l = (targetMax - targetMin);
	double m = l / double(sourceMax - sourceMin);

	for ( std::size_t i = 0; i < this->h; i++ ) {
		for ( std::size_t j = 0; j < this->w; j++ ) {
			double v = static_cast<double>(this->data[i * this->w + j]);
			if ( l == static_cast<double>(0) ) m = static_cast<double>(0);
			v = targetMin + ((v-sourceMin) * m);
			if ( v < targetMin ) v = targetMin;
			if ( v > targetMax ) v = targetMax;
			this->data[i * this->w + j] = v;
		}
	}

	return true;
}

template <typename ImageDataType>
bool IntensityImage<ImageDataType>::invert() {
    if ( this->data == nullptr ) return false;
    if ( this->w == 0 || this->h == 0 ) return false;

	ImageDataType maxValue = this->getMax();
    for ( std::size_t i = 0; i < this->h; i++ ) {
        for ( std::size_t j = 0; j < this->w; j++ ) {
            ImageDataType intensity = this->data[i * this->w + j];
            ImageDataType inverse = maxValue - intensity;
            this->data[i * this->w + j] = inverse;
        }
    }

    return true;
}

template <typename ImageDataType>
bool IntensityImage<ImageDataType>::scale(ImageDataType minScale, ImageDataType maxScale) {
	ImageDataType min, max;
	this->getMinMax(min, max);
    return ScaleArray(this->data, this->w * this->h, min, max, minScale, maxScale);
}

template <typename ImageDataType>
bool IntensityImage<ImageDataType>::mask(std::shared_ptr<IntensityImage<ImageDataType> >& mask) const {
    if ( mask == nullptr ) return false;
    if ( mask->size() != this->size() ) mask->resize(this->w, this->h);

    for ( std::size_t i = 0; i < this->h; i++ ) {
        for ( std::size_t j = 0; j < this->w; j++ ) {
            const ImageDataType& value = this->data[i * this->w + j];
            if ( value > ImageDataType(0) ) mask->data[i * this->w + j] = ImageDataType(1);
            else mask->data[i * this->w + j] = ImageDataType(0);
        }
    }

    return true;
}

template <typename ImageDataType>
bool IntensityImage<ImageDataType>::apply(const Kernel<ImageDataType>& kernel, IntensityImage<ImageDataType>& image, bool divide) const {
    return this->evaluate(kernel, image, divide);
}

template <typename ImageDataType>
bool IntensityImage<ImageDataType>::applyKernel(const Kernel<ImageDataType>& kernel, IntensityImage<ImageDataType>& image, bool divide) const {
    return this->evaluate(kernel, image, divide);
}

template <typename ImageDataType>
bool IntensityImage<ImageDataType>::applyKernel(const Kernel<ImageDataType>& kernel, std::shared_ptr<IntensityImage<ImageDataType> >& image, bool divide) const {
    return this->evaluate(kernel, image, divide);
}

template <typename ImageDataType>
inline ImageDataType ProcessPixel(const Kernel<ImageDataType>& kernel, const IntensityImage<ImageDataType>& image, std::size_t x, std::size_t y, bool divide = false) {
    std::size_t kh = kernel.height();
    std::size_t kw = kernel.width();
    std::size_t cx = kernel.centerY();
    std::size_t cy = kernel.centerY();

    double sum = double(0);
    int imageX;
    int imageY;
    double kernelValue, imageValue;

    for ( std::size_t i = 0; i < kh; i++ ) {
        for ( std::size_t j = 0; j < kw; j++ ) {
            kernelValue = kernel(i, j);
            imageX = x - cx + i;
            imageY = y - cy + j;

            if ( image.validIndex(imageX, imageY) ) {
                imageValue = image(imageX, imageY);
                sum += kernelValue * imageValue;
            }
        }
    }

    if ( divide ) sum /= static_cast<double>(kernel.size());
    return sum;
}

template <typename ImageDataType>
bool IntensityImage<ImageDataType>::evaluate(const Kernel<ImageDataType>& kernel, IntensityImage<ImageDataType>& image, bool divide) const {
    if ( this->data == nullptr ) return false;
    if ( kernel.width() == 0 || kernel.height() == 0 ) return false;
    if ( image.width() != this->w || image.height() != this->h )
        image.resize(this->w, this->h);

    for ( std::size_t i = 0; i < this->h; i++ ) {
        for ( std::size_t j = 0; j < this->w; j++ ) {
            ImageDataType pixelValue = ProcessPixel(kernel, *this, i, j);
            image(i, j) = pixelValue;
        }
    }

    return true;
}

template <typename ImageDataType>
bool IntensityImage<ImageDataType>::evaluate(const Kernel<ImageDataType>& kernel, std::shared_ptr<IntensityImage<ImageDataType> >& image, bool divide) const {
    if ( this->data == nullptr ) return false;
    if ( kernel.width() == 0 || kernel.height() == 0 ) return false;
	if ( image == nullptr ) image = std::make_shared<px::IntensityImage<ImageDataType> >();
    if ( image->width() != this->w || image->height() != this->h ) image->resize(this->w, this->h);

    for ( std::size_t i = 0; i < this->h; i++ ) {
        for ( std::size_t j = 0; j < this->w; j++ ) {
            ImageDataType pixelValue = ProcessPixel(kernel, *this, i, j, divide);
            image->set(i, j, pixelValue);
        }
    }

    return true;
}

template <typename ImageDataType>
bool IntensityImage<ImageDataType>::getMinMax(ImageDataType& min, ImageDataType& max) const {
	if ( this->data == nullptr ) return false;
	if ( this->w == 0 || this->h == 0 ) return false;

	ImageDataType curMin = std::numeric_limits<ImageDataType>::max();
	ImageDataType curMax = std::numeric_limits<ImageDataType>::min();

	for ( std::size_t i = 0; i < this->w * this->h; i++ ) {
        if ( this->data[i] < curMin ) curMin = this->data[i];
		if ( this->data[i] > curMax ) curMax = this->data[i];
	}

	min = curMin;
	max = curMax;
	return true;
}

template <typename ImageDataType>
ImageDataType IntensityImage<ImageDataType>::getMin() const {
    ImageDataType curMin = std::numeric_limits<ImageDataType>::max();
    for ( std::size_t i = 0; i < this->w * this->h; i++ )
        if ( this->data[i] < curMin ) curMin = this->data[i];
    return curMin;
}

template <typename ImageDataType>
ImageDataType IntensityImage<ImageDataType>::getMax() const {
    ImageDataType curMax = std::numeric_limits<ImageDataType>::min();
    for ( std::size_t i = 0; i < this->w * this->h; i++ )
        if ( this->data[i] > curMax ) curMax = this->data[i];
    return curMax;
}

template <typename ImageDataType>
std::ostream& operator << (std::ostream& out, const IntensityImage<ImageDataType>& image) {
    if ( image.data == nullptr || image.w == 0 || image.h == 0 ) {
        out << "[]";
        return out;
    }

	out << "[" << std::endl;
    for ( std::size_t i = 0; i < image.h; i++ ) {
        for ( std::size_t j = 0; j < image.w; j++ ) {
            out << std::setw(4);
            out << image.data[i * image.w + j] << ' ';
        }
        out << std::endl;
    }
	out << "]" << std::endl;

    return out;
}

typedef IntensityImage<unsigned int> IntensityImagei;
typedef IntensityImage<float> IntensityImagef;
typedef IntensityImage<double> IntensityImaged;
typedef IntensityImage<uint8_t> IntensityImageu8;
typedef IntensityImage<uint16_t> IntensityImageu16;

}

#endif
