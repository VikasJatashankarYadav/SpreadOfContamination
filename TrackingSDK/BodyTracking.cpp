#include "BodyTracking.h"

namespace px {

Body::Body() {
	this->joints.resize(JOINT_COUNT);
	this->unique_id = 0u;
	this->tracking_status = BodyStatus::BODY_NOT_TRACKING;
}

Body::~Body() {}

inline bool CopyFromAstraJoint(const astra_pose::Joint& in_joint, Joint& out_joint) {
	out_joint.status = JointStatus(uint8_t(in_joint.status()));
	out_joint.type = BodyJoint(uint8_t(in_joint.type()));
	const auto& world_space_point = in_joint.world_position();
	const auto& image_space_point = in_joint.depth_position();
	out_joint.x = world_space_point.x;
	out_joint.y = world_space_point.y;
	out_joint.z = world_space_point.z;
	out_joint.u = image_space_point.x;
	out_joint.v = image_space_point.y;
	return true;
}

bool Body::update(const astra_pose::Body& body) {
	this->unique_id = body.id();
	this->tracking_status = BodyStatus(uint8_t(body.status()));
	const auto& joint_list = body.joints();
	for ( std::size_t i = 0; i < joint_list.size(); ++i )
		CopyFromAstraJoint(joint_list[i], this->joints[i]);
	return true;
}

const uint8_t& Body::id() const {
	return this->unique_id;
}

const BodyStatus& Body::getStatus() const {
	return this->tracking_status;
}

const std::vector<Joint>& Body::getJoints() const {
	return this->joints;
}

const std::size_t Body::getJointCount() const {
	return this->joints.size();
}

}
