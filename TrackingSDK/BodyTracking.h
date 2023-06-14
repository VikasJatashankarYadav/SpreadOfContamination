
#pragma once

#include <iostream>
#include <vector>
#include <astra/streams/Body.hpp>

namespace px {

typedef enum BodyJoint {
	JOINT_HEAD              = 0,
	JOINT_SHOULDER_SPINE    = 1,
	JOINT_LEFT_SHOULDER     = 2,
	JOINT_LEFT_ELBOW        = 3,
	JOINT_LEFT_HAND         = 4,
	JOINT_RIGHT_SHOULDER    = 5,
	JOINT_RIGHT_ELBOW       = 6,
	JOINT_RIGHT_HAND        = 7,
	JOINT_MID_SPINE         = 8,
	JOINT_BASE_SPINE        = 9,
	JOINT_LEFT_HIP          = 10,
	JOINT_LEFT_KNEE         = 11,
	JOINT_LEFT_FOOT         = 12,
	JOINT_RIGHT_HIP         = 13,
	JOINT_RIGHT_KNEE        = 14,
	JOINT_RIGHT_FOOT        = 15,
	JOINT_LEFT_WRIST        = 16,
	JOINT_RIGHT_WRIST       = 17,
	JOINT_NECK              = 18,
	JOINT_COUNT             = 19,
} BodyJointType;

typedef enum BodyStatus {
	BODY_NOT_TRACKING = 0,
	BODY_TRACKING_LOST = 1,
	BODY_TRACKING_STARTED = 2,
	BODY_TRACKING = 3,
} BodyStatusType;

typedef enum JointStatus {
	JOINT_STATUS_NOT_TRACKED = 0,
	JOINT_STATUS_LOW_CONFIDENCE = 1,
	JOINT_STATUS_TRACKED = 2
} BodyJointStatusType;

struct Joint {
	JointStatus status;
	float x, y, z;
	float u, v;
	uint8_t type;
};

class Body {
public:
	Body();
	virtual ~Body();

	bool update(const astra_pose::Body& body);

	const uint8_t& id() const;
	const BodyStatus& getStatus() const;
	const std::vector<Joint>& getJoints() const;
	const std::size_t getJointCount() const;

	const static uint8_t MAX_JOINT_COUNT = 19u;

protected:
	std::vector<Joint> joints;
	BodyStatus tracking_status;
	uint8_t unique_id;
};

}
