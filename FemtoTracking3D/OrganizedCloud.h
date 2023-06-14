
#ifndef PX_ORGANIZED_CLOUD_H
#define PX_ORGANIZED_CLOUD_H

#include <vector>
#include <iomanip>
#include "PointCloud.h"
#include "DataFrame.h"
#include "Mathematics.h"

namespace px {

template <class PointType>
class OrganizedCloud;

template <typename PointType>
std::ostream& operator << (std::ostream& out, const OrganizedCloud<PointType>& cloud);

template <class PointType>
class OrganizedCloud : public DataFrame<PointType>, public PointCloud<PointType> {
public:
	OrganizedCloud();
	OrganizedCloud(const OrganizedCloud<PointType>& cloud);
	OrganizedCloud(std::size_t width, std::size_t height);
	OrganizedCloud(const PointType* new_data, std::size_t width, std::size_t height);
	virtual ~OrganizedCloud();

	std::size_t size() const;
	PointType* getData() const;
	const PointType* constData() const;

	friend std::ostream& operator << <> (std::ostream& out, const OrganizedCloud<PointType>& cloud);
};

template <class PointType>
OrganizedCloud<PointType>::OrganizedCloud() : DataFrame<PointType>() {
	this->data = nullptr;
	this->w = 0;
	this->h = 0;
	this->timestamp();
	this->name = "OrganizedCloud";
}

template <class PointType>
OrganizedCloud<PointType>::OrganizedCloud(const OrganizedCloud<PointType>& cloud) {
	if ( cloud.data != nullptr ) {
		std::size_t n = cloud.w * cloud.h;
		if ( this->w * this->h != n ) {
			delete [] this->data;
			this->data = new PointType[cloud.w * cloud.h];
		}

		this->w = cloud.w;
		this->h = cloud.h;
		this->stamp = cloud.stamp;
		std::memcpy(this->data, cloud.data, this->w * this->h * sizeof(PointType));
		this->name = cloud.name;
	}
	else {
		this->w = 0;
		this->h = 0;
		this->stamp = 0;
		this->data = nullptr;
		this->name = "OrganizedCloud";
	}
}

template <class PointType>
OrganizedCloud<PointType>::OrganizedCloud(std::size_t width, std::size_t height) : DataFrame<PointType>() {
	if ( width == 0 || height == 0 ) throw std::exception("[OrganizedCloud] Error: Invalid frame size.");
	this->w = width;
	this->h = height;
	this->timestamp();
	this->data = new PointType[this->w * this->h];
	this->name = "OrganizedCloud";
}

template <class PointType>
OrganizedCloud<PointType>::OrganizedCloud(const PointType* new_data, std::size_t width, std::size_t height) : DataFrame(new_data, width, height) {}

template <class PointType>
OrganizedCloud<PointType>::~OrganizedCloud() {}

template <class PointType>
std::size_t OrganizedCloud<PointType>::size() const {
	return this->w * this->h;
}

template <class PointType>
PointType* OrganizedCloud<PointType>::getData() const {
	return this->data;
}

template <class PointType>
const PointType* OrganizedCloud<PointType>::constData() const {
	return this->data;
}

template <typename PointType>
std::ostream& operator << (std::ostream& out, const OrganizedCloud<PointType>& cloud) {
    if ( cloud.data == nullptr || cloud.w == 0 || cloud.h == 0 ) {
        out << "[]";
        return out;
    }

	out << "[" << std::endl;
    for ( std::size_t i = 0; i < cloud.h; i++ ) {
        for ( std::size_t j = 0; j < cloud.w; j++ ) {
            out << std::setw(4);
            out << cloud.data[i * cloud.w + j] << ' ';
        }
        out << std::endl;
    }
	out << "]" << std::endl;

    return out;
}

}

#endif
