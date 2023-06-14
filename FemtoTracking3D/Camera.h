
#ifndef PX_CAMERA_H
#define PX_CAMERA_H

#include "Mathematics.h"

namespace px {

template <typename Real>
class Camera {
public:
    Camera();
    virtual ~Camera();

	virtual bool resize(std::size_t width, std::size_t height) noexcept = 0;
	virtual bool resize(std::size_t width, std::size_t height, Real nearPlane, Real farPlane) noexcept = 0;

	virtual bool rotate(Real du, Real dv) = 0;
	virtual bool pan(Real du, Real dv) = 0;
	virtual bool zoom(Real du) = 0;
	virtual bool scope(Real du, Real dv) = 0;

	void setRotationSensitivity(Real sen);
	void setPanSensitivity(Real sen);
	void setZoomSensitivity(Real sen);
	void setScopeSensitivity(Real sen);

	void reset();
	void resetPlanes();
	void resetView();
	void resetMatrices();
	void resetSensitivities();

	Real getRotationSensitivity() const;
	Real getPanSensitivity() const;
	Real getZoomSensitivity() const;
	Real getScopeSensitivity() const;

	Vector3<Real> getViewDirection();
	Vector3<Real> getRightDirection();
	Vector3<Real> getLeftDirection();
	Vector3<Real> getUpDirection();
	Vector3<Real> getDownDirection();

	const Vector3<Real>& getEye() const;
	const Vector3<Real>& getLookAt() const;
	const Vector3<Real>& getUp() const;

	std::size_t getWidth() const;
	std::size_t getHeight() const;
	const Real& getNear() const;
    const Real& getFar() const;

	const Matrix4<Real>& getViewMatrix();
    const Matrix4<Real>& getProjectionMatrix();

	Matrix4<Real> toViewMatrix();
    Matrix4<Real> toProjectionMatrix();

	static Matrix4<Real> LookAt(const Vector3<Real>& eye, const Vector3<Real>& lookAt, const Vector3<Real>& up);
	static Matrix4<Real> LookAt(Real eyex, Real eyey, Real eyez, Real atx, Real aty, Real atz, Real upx, Real upy, Real upz);

	static Matrix4<Real> PerspectiveMatrix(Real fov, std::size_t width, std::size_t height, Real nearPlane, Real farPlane) noexcept;
	static Matrix4<Real> OrthographicMatrix(Real left, Real right, Real bottom, Real top, Real nearPlane, Real farPlane) noexcept;

public:
	Vector2<int32_t> mouse_cur;
	Vector2<int32_t> mouse_last;

	bool bPan;
	bool bRotate;
	bool bZoom;
	bool bScope;

protected:
	virtual void compile() noexcept = 0;

protected:
	Matrix4<Real> viewMatrix;
	Matrix4<Real> projectionMatrix;
	Real nearPlane, farPlane;

	std::size_t width, height;
	Vector3<Real> eye;
	Vector3<Real> lookAt;
	Vector3<Real> up;

	Real panSensitivity;
	Real zoomSensitivity;
	Real rotationSensitivity;
	Real scopeSensitivity;
};

const Real CAMERA_EPSILON = Real(0.00001);
const Real DEFAULT_NEAR_PLANE = Real(0.1);
const Real DEFAULT_FAR_PLANE = Real(1000.0);
const Real DEFAULT_ROTATION_SENSITIVITY = Real(0.01);
const Real DEFAULT_PAN_SENSITIVITY = Real(0.01);
const Real DEFAULT_ZOOM_SENSITIVITY = Real(0.1);
const Real DEFAULT_SCOPE_SENSITIVITY = Real(0.01);

template <typename Real>
inline Real GetCameraEpsilon() {
	return Real(CAMERA_EPSILON);
}

template <typename Real>
Camera<Real>::Camera() {
	this->viewMatrix = Matrix4<Real>::Identity();
	this->projectionMatrix = Matrix4<Real>::Identity();

	this->eye = Vector3<Real>::UnitZ();
	this->lookAt = Vector3<Real>::Zero();
	this->up = Vector3<Real>::UnitY();

	this->nearPlane = Real(DEFAULT_NEAR_PLANE);
	this->farPlane = Real(DEFAULT_FAR_PLANE);

	this->rotationSensitivity = Real(DEFAULT_ROTATION_SENSITIVITY);
	this->panSensitivity = Real(DEFAULT_PAN_SENSITIVITY);
	this->zoomSensitivity = Real(DEFAULT_ZOOM_SENSITIVITY);
	this->scopeSensitivity = Real(DEFAULT_SCOPE_SENSITIVITY);

	this->bPan = false;
	this->bRotate = false;
	this->bZoom = false;
	this->bScope = false;
}

template <typename Real>
Camera<Real>::~Camera() {}

template <typename Real>
void Camera<Real>::setRotationSensitivity(Real sen) {
	this->rotationSensitivity = sen;
}

template <typename Real>
void Camera<Real>::setPanSensitivity(Real sen) {
	this->panSensitivity = sen;
}

template <typename Real>
void Camera<Real>::setZoomSensitivity(Real sen) {
	this->zoomSensitivity = sen;
}

template <typename Real>
void Camera<Real>::setScopeSensitivity(Real sen) {
	this->scopeSensitivity = sen;
}

template <typename Real>
void Camera<Real>::reset() {
	this->resetMatrices();
	this->resetPlanes();
	this->resetView();
	this->resetSensitivities();
	this->compile();
}

template <typename Real>
void Camera<Real>::resetPlanes() {
	this->nearPlane = Real(DEFAULT_NEAR_PLANE);
	this->farPlane = Real(DEFAULT_FAR_PLANE);
}

template <typename Real>
void Camera<Real>::resetView() {
	this->eye = Vector3<Real>::UnitZ();
	this->lookAt = Vector3<Real>::Zero();
	this->up = Vector3<Real>::UnitY();
}

template <typename Real>
void Camera<Real>::resetMatrices() {
	this->viewMatrix = Matrix4<Real>::Identity();
	this->projectionMatrix = Matrix4<Real>::Identity();
}

template <typename Real>
void Camera<Real>::resetSensitivities() {
	this->rotationSensitivity = Real(DEFAULT_ROTATION_SENSITIVITY);
	this->panSensitivity = Real(DEFAULT_PAN_SENSITIVITY);
	this->zoomSensitivity = Real(DEFAULT_ZOOM_SENSITIVITY);
	this->scopeSensitivity = Real(DEFAULT_SCOPE_SENSITIVITY);
}

template <typename Real>
Real Camera<Real>::getRotationSensitivity() const {
	return this->rotationSensitivity;
}

template <typename Real>
Real Camera<Real>::getPanSensitivity() const {
	return this->panSensitivity;
}

template <typename Real>
Real Camera<Real>::getZoomSensitivity() const {
	return this->zoomSensitivity;
}

template <typename Real>
Real Camera<Real>::getScopeSensitivity() const {
	return this->scopeSensitivity;
}

template <typename Real>
inline Vector3<Real> Camera<Real>::getViewDirection() {
	this->compile();
	return (this->lookAt - this->eye).normalized();
}

template <typename Real>
inline Vector3<Real> Camera<Real>::getRightDirection() {
	this->compile();
	Vector3<Real> dir = (this->lookAt - this->eye).normalized();
	return (this->up.cross(dir)).normalized();
}

template <typename Real>
inline Vector3<Real> Camera<Real>::getLeftDirection() {
	this->compile();
	Vector3<Real> dir = (this->lookAt - this->eye).normalized();
	return (dir.cross(this->up)).normalized();
}

template <typename Real>
inline Vector3<Real> Camera<Real>::getUpDirection() {
	this->compile();
	return this->up;
}

template <typename Real>
inline Vector3<Real> Camera<Real>::getDownDirection() {
	this->compile();
	return -this->up;
}

template <typename Real>
const Vector3<Real>& Camera<Real>::getEye() const {
	return this->eye;
}

template <typename Real>
const Vector3<Real>& Camera<Real>::getLookAt() const {
	return this->lookAt;
}

template <typename Real>
const Vector3<Real>& Camera<Real>::getUp() const {
	return this->up;
}

template <typename Real>
std::size_t Camera<Real>::getWidth() const {
	return this->width;
}
	
template <typename Real>
std::size_t Camera<Real>::getHeight() const {
	return this->height;
}

template <typename Real>
const Real& Camera<Real>::getNear() const {
	return this->nearPlane;
}

template <typename Real>
const Real& Camera<Real>::getFar() const {
	return this->farPlane;
}

template <typename Real>
const Matrix4<Real>& Camera<Real>::getViewMatrix() {
	this->compile();
	return this->viewMatrix;
}

template <typename Real>
const Matrix4<Real>& Camera<Real>::getProjectionMatrix() {
	this->compile();
	return this->projectionMatrix;
}

template <typename Real>
Matrix4<Real> Camera<Real>::toViewMatrix() {
	this->compile();
	return this->viewMatrix;
}

template <typename Real>
Matrix4<Real> Camera<Real>::toProjectionMatrix() {
	this->compile();
	return this->projectionMatrix;
}

template <typename Real>
Matrix4<Real> Camera<Real>::LookAt(const Vector3<Real>& eye, const Vector3<Real>& lookAt, const Vector3<Real>& up) {
	return Camera<Real>::LookAt(eye.x(), eye.y(), eye.z(), lookAt.x(), lookAt.y(), lookAt.z(), up.x(), up.y(), up.z());
}

template <typename Real>
Matrix4<Real> Camera<Real>::LookAt(Real eyex, Real eyey, Real eyez, Real atx, Real aty, Real atz, Real upx, Real upy, Real upz) {
	Matrix4<Real> matrix;
	Vector3<Real> x, y, z;
	Vector3<Real> eye(eyex, eyey, eyez);

	y = Vector3<Real>(upx, upy, upz);
	z = Vector3<Real>(atx - eyex, aty - eyey, atz - eyez);
	x = y.cross(z).normalized();
	y = z.cross(x).normalized();
	z.normalize();

	matrix(0, 0) = -x.x();
    matrix(0, 1) = -x.y();
    matrix(0, 2) = -x.z();
    matrix(0, 3) = x.dot(eye);

    matrix(1, 0) = y.x();
    matrix(1, 1) = y.y();
    matrix(1, 2) = y.z();
    matrix(1, 3) = -y.dot(eye);

    matrix(2, 0) = -z.x();
    matrix(2, 1) = -z.y();
    matrix(2, 2) = -z.z();
    matrix(2, 3) = z.dot(eye);

    matrix(3, 0) = Real(0);
    matrix(3, 1) = Real(0);
    matrix(3, 2) = Real(0);
    matrix(3, 3) = Real(1);
	
	return matrix;
}

template <typename Real>
Matrix4<Real> Camera<Real>::PerspectiveMatrix(Real fov, std::size_t width, std::size_t height, Real nearPlane, Real farPlane) noexcept {
	Matrix4<Real> proj;
	proj.setZero();

	Real aspectRatio = static_cast<Real>(width) / static_cast<Real>(height);
    Real ymax = nearPlane * std::tan(fov * Real(PI) / Real(360));
    Real xmax = ymax * aspectRatio;
	Real left = -xmax;
	Real right = xmax;
	Real top = ymax;
	Real bottom = -ymax;

    Real temp, temp2, temp3, temp4;
    temp = Real(2) * nearPlane;
    temp2 = right - left;
    temp3 = top - bottom;
    temp4 = farPlane - nearPlane;

	proj(0) = temp / temp2;
	proj(5) = temp / temp3;
	proj(8) = (right + left) / temp2;
    proj(9) = (top + bottom) / temp3;
    proj(10) = (-farPlane - nearPlane) / temp4;
    proj(11) = Real(-1);
    proj(14) = (-temp * farPlane) / temp4;

	return proj;
}

template <typename Real>
Matrix4<Real> Camera<Real>::OrthographicMatrix(Real left, Real right, Real bottom, Real top, Real nearPlane, Real farPlane) noexcept {
	Matrix4<Real> proj;

	Real x = Real(2) / (right - left);
	Real y = Real(2) / (top - bottom);
	Real z = Real(-2) / (farPlane - nearPlane);
	Real tx = (right + left) / (right - left);
	Real ty = (top + bottom) / (top - bottom);
	Real tz = (farPlane + nearPlane) / (farPlane - nearPlane);

	proj.setZero();

	proj(0, 0) = x;
    proj(1, 1) = y;
    proj(2, 2) = z;
    proj(3, 3) = Real(1);

    proj(0, 3) = -tx;
    proj(1, 3) = -ty;
    proj(2, 3) = -tz;

	return proj;
}

}

#endif
