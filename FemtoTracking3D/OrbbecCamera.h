
#pragma once

#include <memory>
#include "PhysicalCamera.h"
#include "IntensityImage.h"
#include "DepthCloud.h"

/* Orbbec */
#include "libobsensor/ObSensor.hpp"

namespace px {

struct FemtoImp {
	std::shared_ptr<ob::Pipeline> pipeline = nullptr;
	ob::PointCloudFilter point_cloud_filter;
	std::shared_ptr<ob::FrameSet> frame_set = nullptr;
};

class OrbbecCamera : public PhysicalCamera {
public:
	OrbbecCamera(bool bEnableInfrared, bool bEnablePointCloud);
	virtual ~OrbbecCamera();

	virtual bool connect();
	virtual bool update();
	virtual bool disconnect();
	bool isConnected() const;
	std::string toString() const;

	std::size_t getInfraredWidth() const;
	std::size_t getInfraredHeight() const;
	std::size_t getCloudWidth() const;
	std::size_t getCloudHeight() const;
	std::size_t getColorWidth() const;
	std::size_t getColorHeight() const;

	const std::shared_ptr<IntensityImage<uint16_t>>& getInfraredImage() const;
	const std::shared_ptr<DepthCloud>& getDepthCloud() const;

protected:
	std::unique_ptr<FemtoImp> camera;
	std::shared_ptr<IntensityImage<uint16_t>> infrared_image;
	std::shared_ptr<DepthCloud> depth_cloud;
	bool bInfraredEnabled;
	bool bPointCloudEnabled;
	bool bConnected;
	uint32_t timeout_ms;
};

}
