
#ifndef PX_POINT_CLOUD_H
#define PX_POINT_CLOUD_H

#include <iostream>
#include "PointTypes.h"
#include "Interface.h"

namespace px {

template <class PointType>
class PointCloud : public Interface {
public:
	virtual std::size_t size() const = 0;
	virtual PointType* getData() const = 0;
	virtual const PointType* constData() const = 0;
};

}

#endif
