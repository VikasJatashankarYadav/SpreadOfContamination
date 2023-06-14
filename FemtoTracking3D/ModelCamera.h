
#ifndef PX_MODEL_CAMERA_H
#define PX_MODEL_CAMERA_H

#include "PerspectiveCamera.h"

namespace px {

template <typename Real>
class ModelCamera : public PerspectiveCamera<Real> {
public:
	ModelCamera();
	virtual ~ModelCamera();

	bool rotate(Real du, Real dv);
	bool zoom(Real du);
	bool scope(Real du, Real dv);

protected:
	void compile() noexcept;
};

template <typename Real>
ModelCamera<Real>::ModelCamera() : PerspectiveCamera<Real>() {

}

template <typename Real>
ModelCamera<Real>::~ModelCamera() {}

template <typename Real>
bool ModelCamera<Real>::rotate(Real du, Real dv) {
	this->theta -= (du * this->rotationSensitivity);
	this->phi += (dv * this->rotationSensitivity);
	this->compile();
	return true;
}

template <typename Real>
bool ModelCamera<Real>::zoom(Real dr) {
	if ( this->r + dr > this->maxRadius ) {
		this->r = this->maxRadius;
		this->compile();
		return false;
	}
	else if ( this->r + dr < this->minRadius ) {
		this->r = this->minRadius;
		this->compile();
		return false;
	}
	else {
		this->r += dr;
		this->compile();
		return true;
	}

	return false;
}

template <typename Real>
bool ModelCamera<Real>::scope(Real du, Real dv) {
	Vector3<Real> dir = (this->lookAt - this->eye).normalized();
	Vector3<Real> disp = du * dir;
	this->displacement += disp;
	this->compile();
	return true;
}

template <typename Real>
inline void ModelCamera<Real>::compile() noexcept {
	this->lookAt = Vector3<Real>::Zero();
	this->eye = PerspectiveCamera<Real>::SphereicalToCartesian(this->r, this->theta, this->phi);
	this->up = PerspectiveCamera<Real>::SphereicalToCartesian_dPhi(this->r, this->theta, this->phi).normalized();
	this->up *= Real(-1.0);

	Real epsilon = GetCameraEpsilon<Real>();
	ClampToZero(this->eye, epsilon);
	ClampToZero(this->lookAt, epsilon);
	ClampToZero(this->up, epsilon);

	this->lookAt += this->displacement;
	this->eye += this->displacement;

	this->viewMatrix = Camera<Real>::LookAt(this->eye, this->lookAt, this->up);
}

}

#endif
