
#ifndef PX_DEVICE_H
#define PX_DEVICE_H

#include "Object.h"

namespace px {

class Device : public Object {
public:
	virtual bool connect() = 0;
	virtual bool disconnect() = 0;
	virtual bool isConnected() const = 0;
};

}

#endif
