#include "DepthCloud.h"

namespace px {

const static Real DEFAULT_MIN_DIST = Real(0);
const static Real DEFAULT_MAX_DIST = Real(8);
const static Real DEFAULT_MIN_RANGE = Real(0);
const static Real DEFAULT_MAX_RANGE = Real(8);
const static int16_t COMP_SHORT = 32767;

DepthCloud::DepthCloud() : OrganizedCloud<PointXYZ<Real> >() {
	this->min_distance = DEFAULT_MIN_DIST;
	this->max_distance = DEFAULT_MAX_DIST;
	this->min_range = DEFAULT_MIN_RANGE;
	this->max_range = DEFAULT_MAX_RANGE;
}

DepthCloud::DepthCloud(std::size_t width, std::size_t height) : OrganizedCloud<PointXYZ<Real> >(width, height) {
	this->min_distance = DEFAULT_MIN_DIST;
	this->max_distance = DEFAULT_MAX_DIST;
	this->min_range = DEFAULT_MIN_RANGE;
	this->max_range = DEFAULT_MAX_RANGE;
}

DepthCloud::~DepthCloud() {}

bool DepthCloud::setDistanceBounds(Real min, Real max) {
	this->min_distance = min;
	this->max_distance = max;
	if ( this->min_distance < 0 ) return false;
	if ( this->max_distance < 0 || this->max_distance < this->min_distance ) return false;
	return true;
}

bool DepthCloud::setRangeBounds(Real min, Real max) {
	this->min_range = min;
	this->max_range = max;
	if ( this->min_range < 0 ) return false;
	if ( this->max_range < 0 || this->max_range < this->min_range ) return false;
	return true;
}

bool DepthCloud::serialize(BinaryFileWriter& out, SerializeType type) {
	if ( out.isOpen() == false ) return false;
	if ( this->data == nullptr ) return false;

	out.writeUInt32(static_cast<uint32_t>(this->w));
	out.writeUInt32(static_cast<uint32_t>(this->h));
	out.writeUInt64(static_cast<uint64_t>(this->stamp));

	PointXYZ<Real>* point_data = this->data;
	std::ofstream& stream = out.getStream();
	std::size_t n = this->w * this->h;

	if ( type == SERIALIZE_DEFAULT || type == SERIALIZE_RAW ) {
		stream.write(reinterpret_cast<char*>(point_data), n * sizeof(PointXYZ<Real>));
		return true;
	}

	out.writeFloat(this->min_distance);
	out.writeFloat(this->max_distance);
	out.writeFloat(this->min_range);
	out.writeFloat(this->max_range);

	Real x, y, z;
    int16_t ux, uy, uz;

	if ( this->compressed.size() != n ) this->compressed.resize(n * DIM3);
	int16_t* compressed_data = &this->compressed[0];

	std::size_t index = 0;
	constexpr Real comp = Real(COMP_SHORT);
	const Real m_xy = Real(1) / Real(this->max_range - this->min_range);
	const Real m_z = Real(1) / Real(this->max_distance - this->min_distance);

    for ( std::size_t i = 0; i < n; ++i ) {
        x = this->data[i].x;
		y = this->data[i].y;
		z = this->data[i].z;

		x = ((x - this->min_range) * m_xy);
		y = ((y - this->min_range) * m_xy);
		z = ((z - this->min_distance) * m_z);

        ux = int16_t(x * comp);
        uy = int16_t(y * comp);
        uz = int16_t(z * comp);

		compressed_data[index] = ux;
		compressed_data[index+1] = uy;
		compressed_data[index+2] = uz;

		index += DIM3;
    }

	stream.write(reinterpret_cast<char*>(compressed_data), n * DIM3 * sizeof(int16_t));

	return true;
}

bool DepthCloud::deserialize(BinaryFileReader& in, SerializeType type) {
	if ( in.isOpen() == false ) return false;

	uint32_t width = in.readUInt32();
	uint32_t height = in.readUInt32();
	uint64_t timestamp = in.readUInt64();

	this->w = width;
	this->h = height;
	this->stamp = timestamp;

	std::size_t n = this->w * this->h;
	std::ifstream& stream = in.getStream();

	if ( type == SERIALIZE_DEFAULT || type == SERIALIZE_RAW ) {
		this->w = width;
		this->h = height;
		this->stamp = timestamp;
		if ( this->data != nullptr ) delete [] this->data;
		this->data = new PointXYZ<Real>[this->w * this->h];

		stream.read(reinterpret_cast<char*>(this->data), n * sizeof(PointXYZ<Real>));
		return true;
	}

	this->min_distance = in.readFloat();
	this->max_distance = in.readFloat();
	this->min_range = in.readFloat();
	this->max_range = in.readFloat();
	
	if ( this->data != nullptr ) delete [] this->data;
	this->data = new PointXYZ<Real>[this->w * this->h];

	Real x, y, z;
	int16_t ux, uy, uz;
	constexpr Real inv_comp = Real(1) / Real(COMP_SHORT);

	if ( this->compressed.size() == 0 ) this->compressed.resize(this->w * this->h * DIM3);
	int16_t* compressed_data = &this->compressed[0];
	stream.read(reinterpret_cast<char*>(compressed_data), n * DIM3 * sizeof(int16_t));

	const Real m_xy = (this->max_range - this->min_range);
	const Real m_z = (this->max_distance - this->min_distance);
	
	std::size_t index = 0;
	for ( std::size_t i = 0; i < n; ++i ) {
		ux = compressed_data[index];
		uy = compressed_data[index+1];
		uz = compressed_data[index+2];

		x = Real(ux * inv_comp);
		y = Real(uy * inv_comp);
		z = Real(uz * inv_comp);

		x = this->min_range + (x * m_xy);
		y = this->min_range + (y * m_xy);
		z = this->min_distance + (z * m_z);

		this->data[i].x = x;
		this->data[i].y = y;
		this->data[i].z = z;

		index += DIM3;
    }

	return true;
}

bool DepthCloud::scale(Real uniform_scale) {
	if ( this->data == nullptr ) return false;
	std::size_t n = this->w * this->h;
	for ( std::size_t i = 0; i < n; i++ ) {
		auto& p = this->data[i];
		p.x *= uniform_scale;
		p.y *= uniform_scale;
		p.z *= uniform_scale;
	}
	return true;
}

Real DepthCloud::getMinDepth() const {
	if ( this->data == nullptr ) return Real(0);
	Real min = std::numeric_limits<Real>::max();
	for ( std::size_t i = 0; i < this->w * this->h; i++ )
		if ( this->data[i].z < min ) min = this->data[i].z;
	return min;
}

Real DepthCloud::getMaxDepth() const {
	if ( this->data == nullptr ) return Real(0);
	Real max = std::numeric_limits<Real>::min();
	for ( std::size_t i = 0; i < this->w * this->h; i++ )
		if ( this->data[i].z > max ) max = this->data[i].z;
	return max;
}

Real DepthCloud::getMinDistance() const {
	return this->min_distance;
}

Real DepthCloud::getMaxDistance() const {
	return this->max_distance;
}

Real DepthCloud::getMinRange() const {
	return this->min_range;
}

Real DepthCloud::getMaxRange() const {
	return this->max_range;
}

bool DepthCloud::Load(const std::string& filename, std::vector<std::shared_ptr<DepthCloud> >& clouds) {
	if ( filename.length() == 0 ) return false;

	BinaryFileReader in;
	in.open(filename);
	
	if ( in.isOpen() == false ) {
		std::cerr << "[DepthCloud:Load] Error: Could not open: " << filename << std::endl;
		return false;
	}

	while ( in.hasNext() ) {
		std::shared_ptr<DepthCloud> cloud = std::make_shared<DepthCloud>();
		cloud->deserialize(in);
		clouds.push_back(cloud);
	}

	in.close();
	return true;
}

}
