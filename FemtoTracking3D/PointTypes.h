
#ifndef PX_POINT_TYPES_H
#define PX_POINT_TYPES_H

namespace px {

template <typename Real>
struct PointXYZ {
	Real x, y, z;

	friend std::ostream& operator << (std::ostream& out, const PointXYZ<Real>& p) {
		out << '(' << p.x << ", " << p.y << ", " << p.z << ')'; return out;
	}
};

template <typename Real>
struct PointXYZN : public PointXYZ<Real> {

	friend std::ostream& operator << (std::ostream& out, const PointXYZN<Real>& p) {
		out << '(' << p.x << ", " << p.y << ", " << p.z << ", " << p.nx << ", " << p.ny << ", " << p.nz << ')'; return out;
	}

	Real nx, ny, nz;
};

template <typename Real>
struct PointXYZRGB : public PointXYZ<Real> {

	friend std::ostream& operator << (std::ostream& out, const PointXYZRGB<Real>& p) {
		out << '(' << p.x << ", " << p.y << ", " << p.z << ", " << p.r << ", " << p.g << ", " << p.b << ')'; return out;
	}

	Real r, g, b;
};

}

#endif
