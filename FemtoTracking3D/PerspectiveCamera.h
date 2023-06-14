
#ifndef PX_PERSPECTIVE_CAMERA_H
#define PX_PERSPECTIVE_CAMERA_H

#include "Camera.h"

namespace px {

template <typename Real>
class PerspectiveCamera : public Camera<Real> {
public:
	PerspectiveCamera(Real radius = Real(1));
	virtual ~PerspectiveCamera();

	bool resize(std::size_t width, std::size_t height) noexcept;
	bool resize(std::size_t width, std::size_t height, Real nearPlane, Real farPlane) noexcept;
	bool resize(Real fov, std::size_t width, std::size_t height) noexcept;
	bool resize(Real fov, std::size_t width, std::size_t height, Real nearPlane, Real farPlane) noexcept;

	bool rotate(Real dTheta, Real dPhi);
	bool pan(Real du, Real dv);
	bool zoom(Real du) = 0;
	bool scope(Real du, Real dv) = 0;

	void setPerspective(Real fov, std::size_t width, std::size_t height, Real nearPlane, Real farPlane);
	void setFrustum(Real left, Real right, Real top, Real bottom, Real nearPlane, Real farPlane);

	void setPosition(const Vector3<Real>& position);
	void setPosition(Real x, Real y, Real z);
	void addPosition(const Vector3<Real>& displacement);
	void addPosition(Real dx, Real dy, Real dz);

	void setRotation(Real theta, Real phi);
	void setHorizontalRotation(Real theta);
	void setVerticalRotation(Real phi);
	void setSphericalPosition(Real r, Real theta, Real phi);
	void addRadius(Real dRadius);
	void setRadius(Real radius);
	void setFieldOfView(Real fov);
	void setNearPlane(Real nearPlane);
	void setFarPlane(Real farPlane);
	void setMinRadius(Real minRadius);
	void setMaxRadius(Real maxRadius);

	const Real& getRadius() const;
	const Real& getFieldOfView() const;

	Vector3<Real> getForwardDirection();

	void reset();
	void resetPosition();
	void resetRotation();
	void resetRadius();

	static Vector3<Real> SphereicalToCartesian(Real r, Real theta, Real phi);
	static Vector3<Real> SphereicalToCartesian_dTheta(Real r, Real theta, Real phi);
	static Vector3<Real> SphereicalToCartesian_dPhi(Real r, Real theta, Real phi);
	static Vector3<Real> SphereicalToCartesian_dPhiCrossdTheta(Real r, Real theta, Real phi);

protected:
	Real fov, aspectRatio;
	Real minRadius, maxRadius;

	Real r, theta, phi;
	Vector3<Real> displacement;
};

const Real DEFAULT_RADIUS = Real(1);
const Real DEFAULT_THETA = Real(1.57079632679);
const Real DEFAULT_PHI = Real(1.57079632679);
const Real DEFAULT_FOV = Real(65.0);
const Real DEFAULT_ASPECT_RATIO = Real(1.0);
const Real DEFAULT_MIN_RADIUS = Real(0.1);
const Real DEFAULT_MAX_RADIUS = Real(1000.0);
const Real MAX_FOV = Real(120.0);
const Real MIN_FOV = Real(10.0);

template <typename Real>
PerspectiveCamera<Real>::PerspectiveCamera(Real radius) : Camera<Real>() {
	this->r = radius;
	this->theta = Real(DEFAULT_THETA);
	this->phi = Real(DEFAULT_PHI);
	this->fov = Real(DEFAULT_FOV);
	this->aspectRatio = Real(DEFAULT_ASPECT_RATIO);
	this->minRadius = Real(DEFAULT_MIN_RADIUS);
	this->maxRadius = Real(DEFAULT_MAX_RADIUS);
	this->displacement = Vector3<Real>::Zero();
}

template <typename Real>
PerspectiveCamera<Real>::~PerspectiveCamera() {}

template <typename Real>
bool PerspectiveCamera<Real>::resize(std::size_t width, std::size_t height) noexcept {
	this->setPerspective(this->fov, width, height, this->nearPlane, this->farPlane);
	this->compile();
	return true;
}

template <typename Real>
bool PerspectiveCamera<Real>::resize(std::size_t width, std::size_t height, Real nearPlane, Real farPlane) noexcept {
	this->setPerspective(this->fov, width, height, this->nearPlane, this->farPlane);
	this->compile();
	return true;
}

template <typename Real>
bool PerspectiveCamera<Real>::resize(Real fov, std::size_t width, std::size_t height) noexcept {
	this->setPerspective(fov, width, height, this->nearPlane, this->farPlane);
	this->compile();
	return true;
}

template <typename Real>
bool PerspectiveCamera<Real>::resize(Real fov, std::size_t width, std::size_t height, Real nearPlane, Real farPlane) noexcept {
	this->setPerspective(fov, width, height, nearPlane, farPlane);
	this->compile();
	return true;
}

template <typename Real>
bool PerspectiveCamera<Real>::rotate(Real dTheta, Real dPhi) {
	this->theta += (dTheta * this->rotationSensitivity);
	this->phi += (dPhi * this->rotationSensitivity);
	this->compile();
	return true;
}

template <typename Real>
bool PerspectiveCamera<Real>::pan(Real du, Real dv) {
	Vector3<Real> uDir = this->getLeftDirection();
	Vector3<Real> vDir = this->getDownDirection();

	Vector3<Real> uDisp = (du * this->panSensitivity) * uDir;
	Vector3<Real> vDisp = (dv * this->panSensitivity) * vDir;
	Vector3<Real> panDisp = uDisp + vDisp;

	this->displacement += panDisp;
	this->compile();
	return true;
}

template <typename Real>
void PerspectiveCamera<Real>::setPerspective(Real fov, std::size_t width, std::size_t height, Real nearPlane, Real farPlane) {
	if ( fov > Real(MAX_FOV) ) this->fov = Real(MAX_FOV);
	else if ( fov < Real(MIN_FOV) ) this->fov = Real(MIN_FOV);
	else this->fov = fov;

	this->width = width;
	this->height = height;
	this->nearPlane = nearPlane;
    this->farPlane = farPlane;

	Real aspectRatio = static_cast<Real>(width) / static_cast<Real>(height);
    Real ymax = nearPlane * std::tan(fov * Real(PI) / Real(360));
    Real xmax = ymax * aspectRatio;
    this->setFrustum(-xmax, xmax, ymax, -ymax, nearPlane, farPlane);
}

template <typename Real>
void PerspectiveCamera<Real>::setFrustum(Real left, Real right, Real top, Real bottom, Real nearPlane, Real farPlane) {
    this->projectionMatrix.setZero();

    Real temp, temp2, temp3, temp4;
    temp = Real(2) * nearPlane;
    temp2 = right - left;
    temp3 = top - bottom;
    temp4 = farPlane - nearPlane;

	this->projectionMatrix(0) = temp / temp2;
	this->projectionMatrix(5) = temp / temp3;
	this->projectionMatrix(8) = (right + left) / temp2;
    this->projectionMatrix(9) = (top + bottom) / temp3;
    this->projectionMatrix(10) = (-farPlane - nearPlane) / temp4;
    this->projectionMatrix(11) = Real(-1);
    this->projectionMatrix(14) = (-temp * farPlane) / temp4;
}

template <typename Real>
void PerspectiveCamera<Real>::setPosition(const Vector3<Real>& pos) {
	this->position.x() = pos.x();
	this->position.y() = pos.y();
	this->position.z() = pos.z();
	this->compile();
}

template <typename Real>
void PerspectiveCamera<Real>::setPosition(Real x, Real y, Real z) {
	this->position.x() = x;
	this->position.y() = y;
	this->position.z() = z;
	this->compile();
}

template <typename Real>
void PerspectiveCamera<Real>::addPosition(const Vector3<Real>& displacement) {
	this->position += displacement;
	this->compile();
}

template <typename Real>
void PerspectiveCamera<Real>::addPosition(Real dx, Real dy, Real dz) {
	this->displacement.x() += dx;
	this->displacement.y() += dy;
	this->displacement.z() += dz;
	this->compile();
}

template <typename Real>
void PerspectiveCamera<Real>::setRotation(Real theta, Real phi) {
	this->theta = theta;
	this->phi = phi;
	this->compile();
}

template <typename Real>
void PerspectiveCamera<Real>::setHorizontalRotation(Real theta) {
	this->theta = theta;
	this->compile();
}

template <typename Real>
void PerspectiveCamera<Real>::setVerticalRotation(Real phi) {
	this->phi = phi;
	this->compile();
}

template <typename Real>
void PerspectiveCamera<Real>::setSphericalPosition(Real r, Real theta, Real phi) {
	this->r = r;
	this->theta = theta;
	this->phi = phi;
	this->compile();
}

template <typename Real>
void PerspectiveCamera<Real>::addRadius(Real dRadius) {
	this->r += dRadius;
	this->compile();
}

template <typename Real>
void PerspectiveCamera<Real>::setRadius(Real radius) {
	this->r = radius;
	this->compile();
}

template <typename Real>
void PerspectiveCamera<Real>::setFieldOfView(Real fov) {
	if ( fov > Real(MAX_FOV) ) this->fov = Real(MAX_FOV);
	else if ( fov < Real(MIN_FOV) ) this->fov = Real(MIN_FOV);
	else this->fov = fov;
	this->setPerspective(this->fov, this->aspectRatio, this->nearPlane, this->farPlane);
}

template <typename Real>
void PerspectiveCamera<Real>::setNearPlane(Real nearPlane) {
	this->nearPlane = nearPlane;
	this->setPerspective(this->fov, this->aspectRatio, this->nearPlane, this->farPlane);
}

template <typename Real>
void PerspectiveCamera<Real>::setFarPlane(Real farPlane) {
	this->farPlane = farPlane;
	this->setPerspective(this->fov, this->aspectRatio, this->nearPlane, this->farPlane);
}

template <typename Real>
void PerspectiveCamera<Real>::setMinRadius(Real minRadius) {
	this->minRadius = minRadius;
}

template <typename Real>
void PerspectiveCamera<Real>::setMaxRadius(Real maxRadius) {
	this->maxRadius = maxRadius;
}

template <typename Real>
const Real& PerspectiveCamera<Real>::getRadius() const {
	return this->r;
}

template <typename Real>
const Real& PerspectiveCamera<Real>::getFieldOfView() const {
	return this->fov;
}

template <typename Real>
Vector3<Real> PerspectiveCamera<Real>::getForwardDirection() {
	this->compile();
	Vector3<Real> forwardDir;

	if ( this->phi < QUARTER_PI )
		forwardDir = this->up;
	else if ( this->phi > (HALF_PI + QUARTER_PI) )
		forwardDir = this->up;
	else forwardDir = (this->lookAt - this->eye).normalized();

	return forwardDir.normalized();
}

template <typename Real>
void PerspectiveCamera<Real>::reset() {
	Camera<Real>::reset();

	this->resetPosition();
	this->resetRotation();
	this->resetRadius();
}

template <typename Real>
void PerspectiveCamera<Real>::resetPosition() {
	this->position = Vector3<Real>::Zero();
	this->compile();
}

template <typename Real>
void PerspectiveCamera<Real>::resetRotation() {
	this->theta = DEFAULT_THETA;
	this->phi = DEFAULT_PHI;
	this->compile();
}

template <typename Real>
void PerspectiveCamera<Real>::resetRadius() {
	this->r = DEFAULT_RADIUS;
	this->compile();
}

/*
 * Cartesian from Spherical (form)
 * x = r * cos(theta) * sin(phi)
 * y = r * sin(theta) * sin(phi)
 * z = r * cos(phi)
 *
 * Spherical from Cartesian (form)
 * r = sqrt(x^2 + y^2 + z^2)
 * theta = atan(y / x)
 * phi = acos(z / r);
 *
 * Unswapped: R(r, t, p) = rcos(theta)sin(phi)i + rsin(phi)sin(theta)j + rcos(phi)k
 */
template <typename Real>
inline Vector3<Real> PerspectiveCamera<Real>::SphereicalToCartesian(Real r, Real theta, Real phi) {
    Vector3<Real> result;

	Real sinPhi = std::sin(phi);
	Real cosPhi = std::cos(phi);
	Real sinTheta = std::sin(theta);
	Real cosTheta = std::cos(theta);

    result.x() = r * (cosTheta * sinPhi);
    result.z() = r * (sinTheta * sinPhi);
    result.y() = r * cosPhi;
    return result;
}

/* Unswapped: Rt(r, t, p) = -rsin(phi)sin(theta)i + rsin(phi)cos(theta)j + 0k */
template <typename Real>
inline Vector3<Real> PerspectiveCamera<Real>::SphereicalToCartesian_dTheta(Real r, Real theta, Real phi) {
    Vector3<Real> result;

	Real sinPhi = std::sin(phi);
	Real cosPhi = std::cos(phi);
	Real sinTheta = std::sin(theta);
	Real cosTheta = std::cos(theta);

    result.x() = -r * (sinPhi * sinTheta);
    result.z() = r * (sinPhi * cosTheta);
    result.y() = Real(0);
    return result;
}

/* Unswapped: Rp(r, t, p) = rcos(phi)cos(theta)i + rcos(phi)sin(theta)j - rsin(phi)k */
template <typename Real>
inline Vector3<Real> PerspectiveCamera<Real>::SphereicalToCartesian_dPhi(Real r, Real theta, Real phi) {
    Vector3<Real> result;

	Real sinPhi = std::sin(phi);
	Real cosPhi = std::cos(phi);
	Real sinTheta = std::sin(theta);
	Real cosTheta = std::cos(theta);

    result.x() = r * (cosPhi * cosTheta);
    result.z() = r * (cosPhi * sinTheta);
    result.y() = -r * sinPhi;
    return result;
}

/* Rp X Rt = r^2 * sin^2(phi)cos(theta)i + r^2 * sin^2(phi)sin(theta)j + r^2 * sin(phi)cos(phi)k */
template <typename Real>
inline Vector3<Real> PerspectiveCamera<Real>::SphereicalToCartesian_dPhiCrossdTheta(Real r, Real theta, Real phi) {
    Vector3<Real> result;

    Real rs = (r*r);
	Real sinPhi = std::sin(phi);
	Real cosPhi = std::cos(phi);
	Real sinTheta = std::sin(theta);
	Real cosTheta = std::cos(theta);

    result.x() = -rs * ((sinPhi*sinPhi) * cosTheta);
    result.y() = -rs * ((sinPhi*sinPhi) * sinTheta);
    result.z() = -rs * sinPhi * cosPhi;
    return result;
}

}

#endif
