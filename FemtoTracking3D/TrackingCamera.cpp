#include "TrackingCamera.h"

/* Orbbec Experimental Skeletal Tracking SDK */
#include <astra_core/ImageGraber.hpp>
#include <astra/astra.hpp>

using namespace astra_pose;

namespace px {

class PoseStream: public ImageGraber {
public:
	PoseStream(TrackingCamera* camera) : camera(camera) {}

	virtual bool on_grab_images(DepthImageData& depth, ColorImageData& color) override {
		if ( this->camera->isConnected() == false ) return false;

		if ( this->camera->grab(this->color_image, this->depth_image) == false ) return false;

		depth.height = this->depth_image->as<ob::VideoFrame>()->height();
		depth.width = this->depth_image->as<ob::VideoFrame>()->width();
		depth.hfov = this->camera->getDepthSensorHfov();
		depth.vfov = this->camera->getDepthSensorVfov();
		depth.dataSize = this->depth_image->dataSize();
		depth.data = this->depth_image->data();
		depth.isMirrored = false;

		return true;
	}

	TrackingCamera* camera;
	std::shared_ptr<ob::Frame> color_image;
	std::shared_ptr<ob::Frame> depth_image;
	std::shared_ptr<StreamSet> stream_set;
	StreamReader stream_reader;
};

class BodyTracker : public FrameListener {
public:
	BodyTracker(std::vector<Body>& bodies) : bodies(bodies) {}

	void on_frame_ready(StreamReader& reader, Frame& frame) {
		this->processBodies(frame);
	}

private:
	std::vector<Body>& bodies;

	void processBodies(Frame& frame) {
        BodyFrame bodyFrame = frame.get<BodyFrame>();
		if ( !bodyFrame.is_valid() || bodyFrame.info().width() == 0 || bodyFrame.info().height() == 0 ) {
			return;
		}

		const auto& tracked_bodies = bodyFrame.bodies();
		for ( std::size_t i = 0; i < tracked_bodies.size(); i++ ) {
			const auto& body = tracked_bodies[i];
			this->bodies[i].update(body);
		}

    }
};

TrackingCamera::TrackingCamera(bool bEnableInfrared, bool bEnablePointCloud) : OrbbecCamera(bEnableInfrared, bEnablePointCloud) {
	std::memset(&this->depth_params, 0, sizeof(OBCameraParam));
	this->pose_stream = nullptr;
	this->bodies.resize(ASTRA_MAX_BODIES);
	this->body_tracker = std::make_shared<BodyTracker>(this->bodies);
}

TrackingCamera::~TrackingCamera() {}

bool TrackingCamera::connect() {
	bool bSuccess = OrbbecCamera::connect();
	if ( bSuccess == false ) {
		std::cerr << "[TrackingCamera:connect] Error: Could not open Femto camera." << std::endl;
		return false;
	}

	astra_pose::initialize();
	this->pose_stream = std::make_shared<PoseStream>(this);
	this->pose_stream->stream_set = std::make_shared<StreamSet>(*this->pose_stream.get());
	this->pose_stream->stream_reader = this->pose_stream->stream_set->create_reader();

    auto bodyStream = this->pose_stream->stream_reader.stream<BodyStream>();
    bodyStream.start();

    this->pose_stream->stream_reader.add_listener(*this->body_tracker.get());
    SkeletonProfile profile = bodyStream.get_skeleton_profile();
    BodyTrackingFeatureFlags features = BodyTrackingFeatureFlags::Joints;
	bodyStream.set_default_body_features(features);

	return true;
}

bool TrackingCamera::update() {
	OrbbecCamera::update();
	astra_update();
	return true;
}

bool TrackingCamera::disconnect() {
	auto bodyStream = this->pose_stream->stream_reader.stream<BodyStream>();
	bodyStream.stop();
	OrbbecCamera::disconnect();
	astra_pose::terminate();
	return true;
}

bool TrackingCamera::grab(std::shared_ptr<ob::Frame>& colorFrame, std::shared_ptr<ob::Frame>& depthFrame) {
	if ( this->isConnected() == false ) {
		std::cout << "[TrackingCamera:grab] Error: Camera is not connected." << std::endl;
		return false;
	}

	auto& p = this->camera->pipeline;
	const auto& device = p->getDevice();

	if ( this->camera->frame_set == nullptr ) return false;

    depthFrame = this->camera->frame_set->depthFrame();
    if ( nullptr == depthFrame ) {
        std::cout << "[TrackingCamera:grab] Error: Frameset depth frame is nullptr." << std::endl;
        return false;
    }

	this->depth_params = p->getCameraParam();

	return true;
}

Real TrackingCamera::getDepthSensorHfov() {
	return Real(2) * std::atan2(this->depth_params.depthIntrinsic.cx, this->depth_params.depthIntrinsic.fx);
}

Real TrackingCamera::getDepthSensorVfov() {
	return Real(2) * std::atan2(this->depth_params.depthIntrinsic.cy, this->depth_params.depthIntrinsic.fy);
}

const std::vector<Body>& TrackingCamera::getBodies() const {
	return this->bodies;
}

}
