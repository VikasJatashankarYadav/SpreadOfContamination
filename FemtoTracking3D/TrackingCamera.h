
#pragma once

#include <memory>
#include "OrbbecCamera.h"
#include <BodyTracking.h>

/* Orbbec */
#include <libobsensor/ObSensor.hpp>

namespace px {

class PoseStream;
class BodyTracker;

class TrackingCamera : public OrbbecCamera {
public:
	TrackingCamera(bool bEnableInfrared = true, bool bEnablePointCloud = false);
	virtual ~TrackingCamera();

	bool connect();
	bool update();
	bool disconnect();

	bool grab(std::shared_ptr<ob::Frame>& colorFrame, std::shared_ptr<ob::Frame>& depthFrame);
	Real getDepthSensorHfov();
	Real getDepthSensorVfov();

	const std::vector<Body>& getBodies() const;

protected:
	OBCameraParam depth_params;
	std::shared_ptr<PoseStream> pose_stream;
	std::shared_ptr<BodyTracker> body_tracker;
	std::vector<Body> bodies;
};

}
