#include "OrbbecCamera.h"
#include <sstream>

namespace px {

const static uint32_t DEFAULT_TIMEOUT_MS = 100;
const static std::size_t INFRARED_WIDTH = 640;
const static std::size_t INFRARED_HEIGHT = 480;
const static std::size_t DEPTH_WIDTH = 640;
const static std::size_t DEPTH_HEIGHT = 480;
const static std::size_t COLOR_WIDTH = 1920;
const static std::size_t COLOR_HEIGHT = 1080;

OrbbecCamera::OrbbecCamera(bool bEnableInfrared, bool bEnablePointCloud) : PhysicalCamera() {
	this->camera = std::make_unique<FemtoImp>();
	this->bInfraredEnabled = bEnableInfrared;
	this->bPointCloudEnabled = bEnablePointCloud;
	this->timeout_ms = DEFAULT_TIMEOUT_MS;
	this->infrared_image = nullptr;
	this->depth_cloud = nullptr;
	this->bConnected = false;
}

OrbbecCamera::~OrbbecCamera() {}

bool OrbbecCamera::connect() {
	if ( this->camera == nullptr ) return false;

	try {
		ob::Context::setLoggerSeverity(OB_LOG_SEVERITY_WARN);
		this->camera->pipeline = std::make_shared<ob::Pipeline>();
	}
	catch (ob::Error e) {
		std::cerr << "[OrbbecCamera:connect] Error: " << e.getMessage() << std::endl;
		return false;
	}

	std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

	if ( this->bInfraredEnabled ) {
		auto& p = this->camera->pipeline;
		auto profiles = p->getStreamProfileList(OB_SENSOR_IR);
		auto irProfile = profiles->getVideoStreamProfile(INFRARED_WIDTH, INFRARED_HEIGHT, OB_FORMAT_Y16);

		if ( !irProfile ) irProfile = profiles->getProfile(0)->as<ob::VideoStreamProfile>();
		config->enableStream(irProfile);

		this->infrared_image = std::make_shared<IntensityImage<uint16_t>>();
		this->infrared_image->resize(INFRARED_WIDTH, INFRARED_HEIGHT);
	}

	if ( this->bPointCloudEnabled ) {
		auto& p = this->camera->pipeline;
		auto depthProfiles = p->getStreamProfileList(OB_SENSOR_DEPTH);
		auto depthProfile = depthProfiles->getVideoStreamProfile(DEPTH_WIDTH, DEPTH_HEIGHT, OB_FORMAT_Y16);

		if ( !depthProfile ) depthProfile = depthProfiles->getProfile(0)->as<ob::VideoStreamProfile>();
		config->enableStream(depthProfile);

		this->depth_cloud = std::make_shared<DepthCloud>();
		this->depth_cloud->resize(DEPTH_WIDTH, DEPTH_HEIGHT);
	}

	this->camera->pipeline->start(config);
	this->bConnected = true;
	return true;
}

inline bool UpdateIR(const std::unique_ptr<FemtoImp>& camera, const std::shared_ptr<ob::FrameSet>& frameset, const std::shared_ptr<IntensityImage<uint16_t>>& infrared_image) {
	if ( camera == nullptr ) return false;
	if ( frameset == nullptr ) return false;

	const auto& frame = frameset->irFrame();
	
	if ( frame && frame->format() == OB_FORMAT_Y16 ) {
		auto video_frame = frame->as<ob::VideoFrame>();
		std::size_t width = video_frame->width();
		std::size_t height = video_frame->height();

		if ( infrared_image->width() != width || infrared_image->height() != height )
			infrared_image->resize(width, height);

		uint16_t* ir_data = infrared_image->getData();
		std::memcpy(ir_data, video_frame->data(), width*height*sizeof(uint16_t));
	}

	return false;
}

inline bool UpdateCloud(const std::unique_ptr<FemtoImp>& camera, const std::shared_ptr<ob::FrameSet>& frameset, const std::shared_ptr<DepthCloud>& cloud) {
	if ( camera == nullptr ) return false;
	if ( frameset == nullptr ) return false;

	try {
		camera->point_cloud_filter.setCameraParam(camera->pipeline->getCameraParam());
		camera->point_cloud_filter.setCreatePointFormat(OB_FORMAT_POINT);
		std::shared_ptr<ob::Frame> frame = camera->point_cloud_filter.process(frameset);
		
		if ( frame ) {
			std::size_t n = frame->dataSize() / sizeof(OBPoint);

			if ( n != DEPTH_WIDTH * DEPTH_HEIGHT ) {
				std::cerr << "[OrbbecCamera:UpdateCloud] Error: Frame size n != width*height" << std::endl;
				return false;
			}

			OBPoint* obpoint_data = (OBPoint*)frame->data();
			PointXYZ<Real>* cloud_data = cloud->getData();
			std::memcpy(cloud_data, obpoint_data, n*sizeof(OBPoint));
		}

	}
	catch (std::exception& e) {
		std::cout << "[OrbbecCamera:UpdateCloud] Error: Get point cloud failed: " << e.what() << std::endl;
	}

	return false;
}

bool OrbbecCamera::update() {
	if ( this->camera == nullptr ) return false;
	auto& p = this->camera->pipeline;

	this->camera->frame_set = p->waitForFrames(this->timeout_ms);
	if ( this->camera->frame_set != nullptr && this->camera->frame_set->depthFrame() )
		UpdateCloud(this->camera, this->camera->frame_set, this->depth_cloud);
	if ( this->camera->frame_set != nullptr && this->camera->frame_set->irFrame() )
		UpdateIR(this->camera, this->camera->frame_set, this->infrared_image);
}

bool OrbbecCamera::disconnect() {
	if ( this->camera == nullptr ) return false;
	if ( this->isConnected() == false ) return false;
	
	this->camera->pipeline->stop();
	this->bInfraredEnabled = false;
	this->bPointCloudEnabled = false;
	this->timeout_ms = DEFAULT_TIMEOUT_MS;
	this->infrared_image = nullptr;
	this->depth_cloud = nullptr;
	this->bConnected = false;

	return true;
}

bool OrbbecCamera::isConnected() const {
	if ( this->camera == nullptr ) return false;
	return this->bConnected;
}

std::string OrbbecCamera::toString() const {
	if ( this->camera == nullptr ) return std::string("No camera");

	auto& p = this->camera->pipeline;
	const auto info = p->getDevice()->getDeviceInfo();
	const auto asic_name = info->asicName();
	const auto serial = info->serialNumber();
	const auto name = info->name();
	const auto connection_type = info->connectionType();
	const auto device_id = info->uid();
	const auto version = info->hardwareVersion();
	const auto device_type = info->deviceType();

	std::stringstream stream;
	stream << "ASIC Name: " << asic_name << std::endl;
	stream << "Serial: " << serial << std::endl;
	stream << "Name: " << name << std::endl;
	stream << "Connection Type: " << connection_type << std::endl;
	stream << "Device ID: " << device_id << std::endl;
	stream << "Version: " << version << std::endl;
	stream << "Device Type: OBDeviceType(" << device_type << ")" << std::endl;
	return stream.str();
}

std::size_t OrbbecCamera::getInfraredWidth() const {
	return INFRARED_WIDTH;
}

std::size_t OrbbecCamera::getInfraredHeight() const {
	return INFRARED_HEIGHT;
}

std::size_t OrbbecCamera::getCloudWidth() const {
	return DEPTH_WIDTH;
}

std::size_t OrbbecCamera::getCloudHeight() const {
	return DEPTH_HEIGHT;
}

std::size_t OrbbecCamera::getColorWidth() const {
	return COLOR_WIDTH;
}

std::size_t OrbbecCamera::getColorHeight() const {
	return COLOR_HEIGHT;
}

const std::shared_ptr<IntensityImage<uint16_t>>& OrbbecCamera::getInfraredImage() const {
	return this->infrared_image;
}

const std::shared_ptr<DepthCloud>& OrbbecCamera::getDepthCloud() const {
	return this->depth_cloud;
}

}
