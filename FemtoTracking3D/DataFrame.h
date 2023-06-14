
#ifndef PX_DATA_FRAME_H
#define PX_DATA_FRAME_H

#include <iostream>
#include <memory>
#include <chrono>
#include <sstream>
#include "Object.h"

namespace px {

template <typename DataType>
class DataFrame : public Object {
public:
	DataFrame();
	DataFrame(const DataFrame<DataType>& data_frame);
	DataFrame(std::size_t width, std::size_t height);
	DataFrame(const DataType* new_data, std::size_t width, std::size_t height);
	virtual ~DataFrame();

	bool resize(std::size_t width, std::size_t height);

	bool serialize(BinaryFileWriter& out, SerializeType type = SERIALIZE_DEFAULT);
	bool deserialize(BinaryFileReader& in, SerializeType type = SERIALIZE_DEFAULT);

	bool uniform(const DataType& in_data);
	bool zero();
	bool flip(bool vertical = true);

	bool get(std::size_t index, DataType& out_data);
	bool get(std::size_t i, std::size_t j, DataType& out_data);
	bool set(std::size_t index, const DataType& in_data);
	bool set(std::size_t i, std::size_t j, const DataType& in_data);

	bool copy(const DataType* new_data, std::size_t width, std::size_t height);
	bool copy(const DataFrame<DataType>& data_frame);
	bool copy(const std::shared_ptr<DataFrame<DataType> >& data_frame);
	bool validIndex(int i, int j) const;

	std::size_t getWidth() const;
	std::size_t getHeight() const;
	std::size_t width() const;
	std::size_t height() const;
	virtual std::size_t size() const;

	std::string toString() const;

	DataType* getData();
	virtual DataType* getData() const;
    virtual const DataType* constData() const;

	template <typename ChronoType = std::chrono::microseconds>
	std::size_t timestamp();
	bool setTimestamp(std::size_t timestamp);
	std::size_t getTimestamp() const;

	DataType& operator () (std::size_t i, std::size_t j);
    const DataType& operator () (std::size_t i, std::size_t j) const;
    DataFrame<DataType>& operator = (const DataFrame<DataType>& data_frame);

protected:
	DataType* data;
	std::size_t w, h;
};

//------------------------------------------------------------------------------
// Implementation
//------------------------------------------------------------------------------
template <typename DataType>
DataFrame<DataType>::DataFrame() : Object() {
	this->w = 0;
	this->h = 0;
	this->timestamp();
	this->data = nullptr;
	this->name = "DataFrame";
}

template <typename DataType>
DataFrame<DataType>::DataFrame(const DataFrame<DataType>& data_frame) : Object() {
	if ( data_frame.data != nullptr ) {
		std::size_t n = data_frame.w * data_frame.h;
		if ( this->w * this->h != n ) {
			delete [] this->data;
			this->data = new DataType[data_frame.w * data_frame.h];
		}

		this->w = data_frame.w;
		this->h = data_frame.h;
		this->stamp.store(data_frame.stamp);
		std::memcpy(this->data, data_frame.data, this->w * this->h * sizeof(DataType));
		this->name = data_frame.name;
	}
	else {
		this->w = 0;
		this->h = 0;
		this->stamp.store(0);
		this->data = nullptr;
		this->name = "DataFrame";
	}
}

template <typename DataType>
DataFrame<DataType>::DataFrame(std::size_t width, std::size_t height) : Object() {
	if ( width == 0 || height == 0 ) throw std::exception("[DataFrame] Error: Invalid frame size.");
	this->w = width;
	this->h = height;
	this->timestamp();
	this->data = new DataType[this->w * this->h];
	this->name = "DataFrame";
}

template <typename DataType>
DataFrame<DataType>::DataFrame(const DataType* new_data, std::size_t width, std::size_t height) : Object() {
	if ( new_data == nullptr ) throw std::exception("[DataFrame] Error: Input data is nullptr.");
	this->w = width;
	this->h = height;
	this->timestamp();
	this->data = new DataType[this->w * this->h];
	std::memcpy(this->data, new_data, this->w * this->h * sizeof(DataType));
	this->name = "DataFrame";
}

template <typename DataType>
DataFrame<DataType>::~DataFrame() {
	delete [] this->data;
}

template <typename DataType>
bool DataFrame<DataType>::resize(std::size_t width, std::size_t height) {
	if ( width == 0 || height == 0 ) {
		std::cerr << "[DataFrame:resize] Error: Width or height = 0." << std::endl;
		return false;
	}

	if ( this->w * this->h == width * height ) {
		this->w = width;
		this->h = height;
		return true;
	}

	if ( this->data != nullptr ) delete [] this->data;

	this->w = width;
	this->h = height;
	this->data = new DataType[this->w * this->h];
	return true;
}

template <typename ImageDataType>
bool DataFrame<ImageDataType>::serialize(BinaryFileWriter& out, SerializeType type) {
	if ( out.isOpen() == false ) return false;
	out.writeUInt32(static_cast<uint32_t>(this->w));
	out.writeUInt32(static_cast<uint32_t>(this->h));
	out.writeUInt64(static_cast<uint64_t>(this->stamp));
	out.writeBuffer(this->data, this->w*this->h);
	return true;
}

template <typename ImageDataType>
bool DataFrame<ImageDataType>::deserialize(BinaryFileReader& in, SerializeType type) {
	if ( in.isOpen() == false ) return false;
	uint32_t width = in.readUInt32();
	uint32_t height = in.readUInt32();
	uint64_t timestamp = in.readUInt64();

	this->w = width;
	this->h = height;
	this->setTimestamp(timestamp);
	if ( this->data != nullptr ) delete [] this->data;
	this->data = new ImageDataType[this->w * this->h];
	in.readBuffer(this->data, this->w * this->h);
	return true;
}

template <typename DataType>
bool DataFrame<DataType>::uniform(const DataType& in_data) {
	if ( this->data == nullptr ) return false;
	std::size_t n = this->w * this->h;
	for ( std::size_t i = 0; i < n; i++ )
		this->data[i] = in_data;
	return true;
}

template <typename DataType>
bool DataFrame<DataType>::zero() {
	if ( this->data == nullptr ) return false;
	if ( this->w == 0 || this->h == 0 ) {
		std::cerr << "[DataFrame:zero] Error: Dataframe with size 0." << std::endl;
		return false;
	}
	std::memset(this->data, 0, this->w * this->h * sizeof(DataType));
	return true;
}

template <typename DataType>
bool DataFrame<DataType>::flip(bool vertical) {
	if ( this->data == nullptr ) return false;
	DataType temp;
	std::size_t h2 = this->h / 2;
	std::size_t w2 = this->w / 2;

	if ( vertical == true ) {
		for ( std::size_t i = 0; i < h2; i++ ) {
			for ( std::size_t j = 0; j < this->w; j++ ) {
				temp = this->data[i * this->w + j];
				this->data[i * this->w + j] = this->data[(this->h-1 - i) * this->w + j];
				this->data[(this->h-1 - i) * this->w + j] = temp;
			}
		}
	}
	else {
		for ( std::size_t i = 0; i < this->h; i++ ) {
			for ( std::size_t j = 0; j < w2; j++ ) {
				temp = this->data[i * this->w + j];
				this->data[i * this->w + j] = this->data[i * this->w + (this->w-1 - j)];
				this->data[i * this->w + (this->w-1 - j)] = temp;
			}
		}
	}

	return true;
}

template <typename DataType>
bool DataFrame<DataType>::get(std::size_t index, DataType& out_data) {
	if ( this->data == nullptr ) return false;
	if ( index >= this->w * this->h ) {
		std::cerr << "[DataFrame:get] Error: Invalid index: " << index << " for size: " << this->w*this->h << std::endl;
		return false;
	}
	out_data = this->data[index];
	return true;
}

template <typename DataType>
bool DataFrame<DataType>::get(std::size_t i, std::size_t j, DataType& out_data) {
	if ( this->data == nullptr ) return false;
	if ( i >= this->h ) {
		std::cerr << "[DataFrame:get] Error: Invalid i: " << i << " for height: " << this->h << std::endl;
		return false;
	}
	
	if ( j >= this->w ) {
		std::cerr << "[DataFrame:get] Error: Invalid j: " << j << " for width: " << this->w << std::endl;
		return false;
	}

	out_data = this->data[i * this->w + j];
	return true;
}

template <typename DataType>
bool DataFrame<DataType>::set(std::size_t index, const DataType& in_data) {
	if ( this->data == nullptr ) return false;
	if ( index >= this->w * this->h ) {
		std::cerr << "[DataFrame:set] Error: Invalid index: " << index << " for size: " << this->w*this->h << std::endl;
		return false;
	}
	this->data[index] = in_data;
	return true;
}

template <typename DataType>
bool DataFrame<DataType>::set(std::size_t i, std::size_t j, const DataType& in_data) {
	if ( this->data == nullptr ) return false;
	if ( i >= this->h ) {
		std::cerr << "[DataFrame:set] Error: Invalid i: " << i << " for height: " << this->h << std::endl;
		return false;
	}
	
	if ( j >= this->w ) {
		std::cerr << "[DataFrame:set] Error: Invalid j: " << j << " for width: " << this->w << std::endl;
		return false;
	}

	this->data[i * this->w + j] = in_data;
	return true;
}

template <typename DataType>
bool DataFrame<DataType>::copy(const DataType* new_data, std::size_t width, std::size_t height) {
	std::size_t n = width * height;
	if ( this->w * this->h != n || this->data == nullptr ) {
		delete [] this->data;
		this->data = new DataType[width * height];
	}

	this->w = width;
	this->h = height;
	this->timestamp();
	std::memcpy(this->data, new_data, this->w * this->h * sizeof(DataType));
	return true;
}

template <typename DataType>
bool DataFrame<DataType>::copy(const DataFrame<DataType>& data_frame) {
	if ( data_frame.w == 0 || data_frame.h == 0 ) return false;
	std::size_t n = data_frame.w * data_frame.h;

	if ( this->w * this->h != n || this->data == nullptr ) {
		delete [] this->data;
		this->data = new DataType[data_frame.w * data_frame.h];
	}

	this->w = data_frame.w;
	this->h = data_frame.h;
	this->stamp.store(data_frame.stamp.load());
	std::memcpy(this->data, data_frame.data, this->w * this->h * sizeof(DataType));
	return true;
}

template <typename DataType>
bool DataFrame<DataType>::copy(const std::shared_ptr<DataFrame<DataType> >& data_frame) {
	if ( data_frame == nullptr ) {
		std::cerr << "[DataFrame:copy] Error: Provided frame is nullptr." << std::endl;
		return false;
	}

	if ( data_frame->w == 0 || data_frame->h == 0 ) return false;

	std::size_t n = data_frame->w * data_frame->h;

	if ( this->w * this->h != n || this->data == nullptr ) {
		delete [] this->data;
		this->data = new DataType[data_frame->w * data_frame->h];
	}

	this->w = data_frame->w;
	this->h = data_frame->h;
	this->stamp.store(data_frame->stamp.load());
	std::memcpy(this->data, data_frame->data, this->w * this->h * sizeof(DataType));

	return true;
}

template <typename DataType>
bool DataFrame<DataType>::validIndex(int i, int j) const {
	if ( i >= 0 && j >= 0 && i < this->h && j < this->w ) return true;
	return false;
}

template <typename DataType>
std::size_t DataFrame<DataType>::getWidth() const {
	return this->w;
}

template <typename DataType>
std::size_t DataFrame<DataType>::getHeight() const {
	return this->h;
}

template <typename DataType>
std::size_t DataFrame<DataType>::width() const {
	return this->w;
}

template <typename DataType>
std::size_t DataFrame<DataType>::height() const {
	return this->h;
}

template <typename DataType>
std::size_t DataFrame<DataType>::size() const {
	return this->w * this->h;
}

template <typename DataType>
std::string DataFrame<DataType>::toString() const {
	std::stringstream stream;
	stream << "Image (" << this->stamp << ") [" << std::endl;
	for ( std::size_t i = 0; i < this->h; i++ ) {
		for ( std::size_t j = 0; j < this->w; j++ ) {
			stream << this->data[i * this->w + j] << ' ';
		}
		stream << std::endl;
	}
	stream << "]" << std::endl;

	return stream.str();
}

template <typename DataType>
DataType* DataFrame<DataType>::getData() {
	return this->data;
}

template <typename DataType>
DataType* DataFrame<DataType>::getData() const {
	return this->data;
}

template <typename DataType>
const DataType* DataFrame<DataType>::constData() const {
	return this->data;
}

template <typename DataType>
template <typename ChronoType>
std::size_t DataFrame<DataType>::timestamp() {
	auto time = std::chrono::duration_cast<ChronoType>(std::chrono::system_clock::now().time_since_epoch());
	this->stamp = time.count();
	return this->stamp;
}

template <typename DataType>
bool DataFrame<DataType>::setTimestamp(std::size_t timestamp) {
	this->stamp = timestamp;
	return true;
}

template <typename DataType>
std::size_t DataFrame<DataType>::getTimestamp() const {
	return this->stamp;
}

template <typename DataType>
DataType& DataFrame<DataType>::operator () (std::size_t i, std::size_t j) {
	if ( i * this->w + j >= this->w * this->h ) throw std::exception("[DataFrame:()] Error: Index out of bounds");
	if ( this->data == nullptr ) throw std::exception("[DataFrame:()] Error: DataFrame data nullptr.");
	return this->data[i * this->w + j];
}

template <typename DataType>
const DataType& DataFrame<DataType>::operator () (std::size_t i, std::size_t j) const {
	if ( i * this->w + j >= this->w * this->h ) throw std::exception("[DataFrame:()] Error: Index out of bounds");
	if ( this->data == nullptr ) throw std::exception("[DataFrame:()] Error: DataFrame data nullptr.");
	return this->data[i * this->w + j];
}

template <typename DataType>
DataFrame<DataType>& DataFrame<DataType>::operator = (const DataFrame<DataType>& data_frame) {
    if ( this == &data_frame ) return *this;
    
	std::size_t n = data_frame.w * data_frame.h;
	if ( this->w * this->h != n || this->data == nullptr ) {
		delete [] this->data;
		this->data = new DataType[data_frame.w * data_frame.h];
	}

	this->w = data_frame.w;
    this->h = data_frame.h;
	this->stamp.store(data_frame.stamp.load());
	std::memcpy(this->data, data_frame.data, this->w * this->h * sizeof(DataType));

    return *this;
}

}

#endif
