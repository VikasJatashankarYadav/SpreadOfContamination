
#ifndef PX_MATHEMATICS_H
#define PX_MATHEMATICS_H

#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <fstream>
#include <limits>

#include <Eigen/Eigen>

namespace px {

typedef float Real;

static const Real PI = Real(3.14159265);
static const Real HALF_PI = Real(1.57079633);
static const Real QUARTER_PI = Real(0.7853981625);
static const Real E = Real(2.71828182845904523536);

const static Real DEGREE_180 = Real(180);
const static Real DegToRadConv = Real(PI) / DEGREE_180;
const static Real RadToDegConv = DEGREE_180 / Real(PI);

template <typename Real>
inline Real DegreesToRadians(Real degrees) {
	return degrees * Real(DegToRadConv);
}

template <typename Real>
inline Real RadiansToDegrees(Real radians) {
	return radians * Real(RadToDegConv);
}

template <typename Real>
using Vector2 = Eigen::Matrix<Real, 2, 1>;

template <typename Real>
using Normal2 = Eigen::Matrix<Real, 2, 1>;

template <typename Real>
using Vector3 = Eigen::Matrix<Real, 3, 1>;

template <typename Real>
using Normal3 = Eigen::Matrix<Real, 3, 1>;

template <typename Real>
using Vector4 = Eigen::Matrix<Real, 4, 1>;

template <typename Real>
using Normal4 = Eigen::Matrix<Real, 4, 1>;

template <typename Real>
using Matrix2 = Eigen::Matrix<Real, 2, 2>;

template <typename Real>
using Matrix3 = Eigen::Matrix<Real, 3, 3>;

template <typename Real>
using Vector = Eigen::Matrix<Real, -1, 1>;

template <typename Real>
using Matrix4 = Eigen::Matrix<Real, 4, 4>;

template <typename Real>
using Matrix = Eigen::Matrix<Real, -1, -1>;

template <typename Real>
using Quaternion = Eigen::Quaternion<Real>;

template <typename Real>
using AngleAxis = Eigen::AngleAxis<Real>;

typedef Vector2<float> Vector2f;
typedef Normal2<float> Normal2f;
typedef Vector3<float> Vector3f;
typedef Normal3<float> Normal3f;
typedef Vector4<float> Vector4f;
typedef Normal4<float> Normal4f;
typedef Matrix2<float> Matrix2f;
typedef Matrix3<float> Matrix3f;
typedef Matrix4<float> Matrix4f;
typedef Vector<float> VectorXf;
typedef Matrix<float> MatrixXf;
typedef Quaternion<float> Quaternionf;

typedef Vector2<double> Vector2d;
typedef Normal2<double> Normal2d;
typedef Vector3<double> Vector3d;
typedef Normal3<double> Normal3d;
typedef Vector4<double> Vector4d;
typedef Normal4<double> Normal4d;
typedef Matrix2<double> Matrix2d;
typedef Matrix3<double> Matrix3d;
typedef Matrix4<double> Matrix4d;
typedef Vector<double> VectorXd;
typedef Matrix<double> MatrixXd;
typedef Quaternion<double> Quaterniond;

typedef Vector2<int> Vector2i;
typedef Vector3<int> Vector3i;
typedef Vector4<int> Vector4i;
typedef Matrix2<int> Matrix2i;
typedef Matrix3<int> Matrix3i;
typedef Matrix4<int> Matrix4i;
typedef Vector<int> VectorXi;
typedef Matrix<int> MatrixXi;

typedef std::vector<Real> RealArray;

template <typename Real>
using Vector2Array = std::vector<Vector2<Real> >;

template <typename Real>
using Vector3Array = std::vector<Vector3<Real> >;

/*
* A simple enumeration of all major axis directions. This is
* a non scoped enum due to some usage of the mapping between
* axes: X = 0, Y = 1, Z = 2;
* 
* X, Y, Z represent the positive directions.
* NX, NY, NZ represent the negative directions.
*/
enum Axis { X, Y, Z, NX, NY, NZ, NO_AXIS };
enum class AxisPlane { XY, YZ, XZ, NO_AXISPLANE };
enum Dimension { DIM1 = 1, DIM2 = 2, DIM3 = 3, DIM4 = 4 };

template <typename Real>
inline Real Gaussian(Real x, Real sigma) {
	Real p = x/sigma;
	return (Real(1) / (sigma * std::sqrt(Real(2) * PI))) * std::exp((-Real(0.5))*(p*p));
}

template <typename Real>
inline Real Normalize(const Real& value, const Real& min, const Real& max) {
	return (value - min) / (max - min);
}

template <typename Real>
inline Real Denormalize(const Real& value, const Real& min, const Real& max) {
	return value * (max - min) + min;
}

template <typename Real>
inline bool NormalizeVector(std::vector<Real>& vector, const Real& min, const Real& max) {
	if ( vector.size() == 0 ) return false;
	for ( std::size_t i = 0; i < vector.size(); i++ )
		vector[i] = Normalize(vector[i], min, max);
	return true;
}

template <typename Real>
inline bool DenormalizeVector(std::vector<Real>& vector, const Real& min, const Real& max) {
	if ( vector.size() == 0 ) return false;
	for ( std::size_t i = 0; i < vector.size(); i++ )
		vector[i] = Deormalize(vector[i], min, max);
	return true;
}

template <typename Real>
inline void ZeroFillVector(Vector<Real>& vector) {
	std::memset(vector.data(), 0, vector.size() * sizeof(Real));
}

template <typename Real>
inline void ZeroFillMatrix(Matrix<Real>& matrix) {
	std::memset(matrix.data(), 0, matrix.cols() * matrix.rows() * sizeof(Real));
}

template <typename Real>
bool CopyVector(const Vector<Real>& vIn, Vector<Real>& vOut) {
	if ( vIn.size() == 0 ) return false;
	vOut.resize(vIn.size());
	for ( std::size_t i = 0; i < vIn.size(); i++ )
		vOut[i] = vIn[i];
	return true;
}

template <typename Real>
inline Real ComputeAverage(const std::vector<Real>& values) {
	Real sum = Real(0);
	std::size_t n = values.size();
	for ( std::size_t i = 0; i < n; i++ ) sum += values[i];
	return sum / Real(n);
}

template <typename Real>
bool GetVectorMinMax(const std::vector<Real>& vector, Real& min, Real& max) {
	if ( vector.size() == 0 ) return false;
	Real curMin = std::numeric_limits<Real>::max();
	Real curMax = std::numeric_limits<Real>::min();

	for ( std::size_t i = 0; i < vector.size(); i++ ) {
		const Real& value = vector[i];
		if ( value < curMin ) curMin = value;
		if ( value > curMax ) curMax = value;
	}

	min = curMin;
	max = curMax;
	return true;
}

template <typename Real>
inline bool MultiplyVectors(const Vector<Real>& uVector, const Vector<Real>& vVector, Vector<Real>& out) {
	if ( uVector.size() != vVector.size() ) return false;
	out.resize(uVector.size());
	for ( std::size_t i = 0; i < uVector.size(); i++ ) {
		out[i] = uVector[i] * vVector[i];
	}
	return true;
}

template <typename Real>
inline bool AddVectors(const Vector<Real>& uVector, const Vector<Real>& vVector, Vector<Real>& out) {
	if ( uVector.size() != vVector.size() ) return false;
	out.resize(uVector.size());
	for ( std::size_t i = 0; i < uVector.size(); i++ ) {
		out[i] = uVector[i] + vVector[i];
	}
	return true;
}

template <typename Real>
bool PrintVector(const std::string& name, const Vector<Real>& vector) {
	if ( vector.size() == 0 ) return false;
	std::cout << "[Vector: " << name << "]" << std::endl;
	for ( std::size_t i = 0; i < vector.size(); i++ )
		std::cout << vector[i] << ' ';
	std::cout << std::endl;
	return true;
}

template <typename Real>
inline Matrix<Real> Identity(std::size_t rows, std::size_t cols) {
	Matrix<Real> I = Matrix<Real>(rows, cols);
	for ( uint8_t i = 0; i < rows; i++ ) {
		for ( uint8_t j = 0; j < cols; j++ ) {
			if ( i == j ) I(i, j) = Real(1);
			else I(i, j) = Real(0);
		}
	}
	return I;
}

template <typename Real>
inline Matrix2<Real> RotationMatrix2D(Real angle) {
	Matrix2<Real> rot;
	rot << std::cos(angle), -std::sin(angle), std::sin(angle), std::cos(angle);
	return rot;
}

template <typename Real>
inline Matrix3<Real> RotationMatrixX(Real angle) {
	Matrix3<Real> xRot;
	xRot << 1, 0, 0, 0, std::cos(angle), -std::sin(angle), 0, std::sin(angle), std::cos(angle);
	return xRot;
}

template <typename Real>
inline Matrix3<Real> RotationMatrixY(Real angle) {
	Matrix3<Real> yRot;
	yRot << std::cos(angle), 0, std::sin(angle), 0, 1, 0, -std::sin(angle), 0, std::cos(angle);
	return yRot;
}

template <typename Real>
inline Matrix3<Real> RotationMatrixZ(Real angle) {
	Matrix3<Real> zRot;
	zRot << std::cos(angle), -std::sin(angle), 0, std::sin(angle), std::cos(angle), 0, 0, 0, 1;
	return zRot;
}

template <typename Real>
inline Real square(const Real& x) { return x*x; }

template <typename Real>
inline void ClampToZero(Real& value, Real epsilon) noexcept {
	if ( value > -epsilon && value < epsilon ) value = Real(0);
}

template <typename Real>
inline void ClampToZero(Vector2<Real>& vector, Real epsilon) noexcept {
	ClampToZero(vector.x(), epsilon);
	ClampToZero(vector.y(), epsilon);
}

template <typename Real>
inline void ClampToZero(Vector3<Real>& vector, Real epsilon) noexcept {
	ClampToZero(vector.x(), epsilon);
	ClampToZero(vector.y(), epsilon);
	ClampToZero(vector.z(), epsilon);
}

template <typename Real>
inline void ClampToZero(Vector4<Real>& vector, Real epsilon) noexcept {
	ClampToZero(vector.w(), epsilon);
	ClampToZero(vector.x(), epsilon);
	ClampToZero(vector.y(), epsilon);
	ClampToZero(vector.z(), epsilon);
}

template <typename Real>
Real GenerateUniformReal(Real min, Real max) {
	//std::random_device rand_dev;
	//std::mt19937 generator(rand_dev());
	//std::uniform_real_distribution<Real> distr(min, max);
	//return static_cast<Real>(distr(generator));
	return (Real)rand() / (Real)RAND_MAX ;
}

template <typename Real>
bool Equivalent(const Real& a, const Real& b, const Real& epsilon = Real(1.0e-4)) {
	if ( a >= b - epsilon && a <= b + epsilon ) return true;
	return false;
}

template <typename Real>
bool Equivalent(const Vector2<Real>& a, const Vector2<Real>& b, Real epsilon = Real(1.0e-4)) {
	if ( a.x() < b.x() - epsilon || a.x() > b.x() + epsilon ) return false;
	if ( a.y() < b.y() - epsilon || a.y() > b.y() + epsilon ) return false;
	return true;
}

template <typename Real>
bool Equivalent(const Vector3<Real>& a, const Vector3<Real>& b, Real epsilon = Real(1.0e-4)) {
	if ( a.x() < b.x() - epsilon || a.x() > b.x() + epsilon ) return false;
	if ( a.y() < b.y() - epsilon || a.y() > b.y() + epsilon ) return false;
	if ( a.z() < b.z() - epsilon || a.z() > b.z() + epsilon ) return false;
	return true;
}

template <typename Real>
inline Real AngleBetweenVectors(const Vector2<Real>& a, const Vector2<Real>& b) {
	return std::acos((a.x()*b.x() + a.y()*b.y()) / (a.norm() * b.norm()));
}


template <typename Real>
inline Real AngleBetweenVectors(const Vector3<Real>& a, const Vector3<Real>& b) {
	return std::acos((a.x()*b.x() + a.y()*b.y() + a.z()*b.z()) / (a.norm() * b.norm()));
}

template <typename Real>
Real Distance(const Vector2<Real>& a, const Vector2<Real>& b) {
	Real dx = b.x() - a.x();
	Real dy = b.y() - a.y();
	return std::sqrt(dx*dx + dy*dy);
}

template <typename Real>
Real Distance(const Vector3<Real>& a, const Vector3<Real>& b) {
	Real dx = b.x() - a.x();
	Real dy = b.y() - a.y();
	Real dz = b.z() - a.z();
	return std::sqrt(dx*dx + dy*dy + dz*dz);
}

/*
* Scales the provided value v, which resides within the source interval:
* [sourceMin, sourceMax] to its respective location within the target
* interval: [targetMin, targetMax]. If the intervals are invalid or the 
* provided value does not reside within the source interval this function
* will return false; otherwise it will return true.
*
* @param v - The value to be modified (scaled)
* @param sourceMin - Lower limit of the source interval
* @param sourceMax - Upper limit of the source interval
* @param targetMin - Lower limit of the target interval
* @param targetMax - Upper limit of the target interval
* @return success
* 
* Usuage:
* double value = 50.0;
* px::Scale(value, 0.0, 100.0, 0.0, 1.0);
*
* Result:
* value = 0.5
*/
template <typename Real>
inline bool Scale(Real& v, Real sourceMin, Real sourceMax, Real targetMin, Real targetMax) {
	Real l = (targetMax - targetMin);
	Real m = l / Real(sourceMax - sourceMin);
	if ( l == static_cast<Real>(0) ) m = static_cast<Real>(0);
	v = targetMin + ((v-sourceMin) * m);
	return true;
}

/*
* Scales an entire array of values included within the source interval:
* [sourceMin, sourceMax] to their respective values within the target interval:
* [targetMin, targetMax]. If each value is correctly scaled this function returns
* true; otherwise, if any value cannot be scaled the algorithm halts and returns
* false.
* 
* @param array - The pointer to the value array
* @param n - The length of the input array
* @param sourceMin - Lower limit of the source interval
* @param sourceMax - Upper limit of the source interval
* @param targetMin - Lower limit of the target interval
* @param targetMax - Upper limit of the target interval
* @return success
*
* Usage:
* const unsigned int n = 4;
* double* values = new double[n + 1];
* for ( unsigned int i = 0; i < n + 1; i++ )
*    values[i] = static_cast<double>(i);
* px::Scale(values, n + 1, 0.0, 4.0, 0.0, 1.0);
* for ( unsigned int i = 0; i < n + 1; i++ )
*    std::cout << values[i] << std::endl;
* delete [] values;
* 
* Result:
* values = 0.0, 0.25, 0.5, 0.75, 1.0
*/
template <typename Real>
inline bool ScaleArray(Real*& array, unsigned int n, Real sourceMin, Real sourceMax, Real targetMin, Real targetMax) {
	if ( array == nullptr ) return false;
	if ( n <= 1 ) return false;

	for ( unsigned int i = 0; i < n; i++ ) {
		bool success = Scale(array[i], sourceMin, sourceMax, targetMin, targetMax);
		if ( !success ) return false;
	}

	return true;
}

template <typename Real>
inline bool Clamp(Real& value, Real targetMin, Real targetMax) {
	if ( value < targetMin ) value = targetMin;
	if ( value > targetMax ) value = targetMax;
	return true;
}

template <typename Real>
Vector3<Real> CartesianToSpherical(const Vector3<Real>& cartesianCoord) {
	Vector3<Real> sphericalCoord;

	Real x = cartesianCoord.x();
	Real z = cartesianCoord.y();
	Real y = cartesianCoord.z();

	Real r = std::sqrt(x*x + y*y + z*z);
	Real theta = std::atan2(y, x);
	Real phi = std::acos(z / r);

	sphericalCoord.x() = r;
	sphericalCoord.y() = theta;
	sphericalCoord.z() = phi;

	return sphericalCoord;
}

template <typename Real>
Vector3<Real> SphericalToCartesian(const Vector3<Real>& sphericalCoord) {
	Vector3<Real> cartesianCoord;
	Real r = sphericalCoord.x();
	Real theta = sphericalCoord.z();
	Real phi = sphericalCoord.y();

	Real cosTheta = std::cos(theta);
	Real cosPhi = std::cos(phi);
	Real sinTheta = std::sin(theta);
	Real sinPhi = std::sin(phi);

	cartesianCoord.x() = r * (cosTheta * sinPhi);  
	cartesianCoord.y() = r * (sinTheta * sinPhi);
	cartesianCoord.z() = r * cosPhi;

	return cartesianCoord;
}

}

#endif
