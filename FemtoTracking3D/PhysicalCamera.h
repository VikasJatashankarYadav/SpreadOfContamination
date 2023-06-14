
#ifndef PX_PHYSICAL_CAMERA_H
#define PX_PHYSICAL_CAMERA_H

#include "Device.h"

namespace px {

class PhysicalCamera : public Device {
public:
	virtual bool connect() = 0;
	virtual bool disconnect() = 0;
	virtual bool isConnected() const = 0;
};

}

#endif
