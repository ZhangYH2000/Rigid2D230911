#pragma once
#include "vec2.h"

struct vec3;
inline vec3 operator/(vec3 a, double b);
inline bool operator==(vec3 a, vec3 b);
struct vec3 {
	double x = 0, y = 0, z = 0;
	vec3() = default;
	vec3(double x, double y, double z) : x(x), y(y), z(z) {}
	vec3(double a) : vec3(a, a, a) {}
	explicit vec3(wstring const& s);

	double& r() { return x; }
	double& g() { return y; }
	double& b() { return z; }
	double r() const { return x; }
	double g() const { return y; }
	double b() const { return z; }

	vec3 operator+() const { return *this; }
	vec3 operator-() const { return { -x, -y, -z }; }
	vec3& operator+=(vec3 a) { x += a.x; y += a.y; z += a.z; return *this; }
	vec3& operator-=(vec3 a) { return *this += -a; }
	vec3& operator*=(vec3 a) { x *= a.x; y *= a.y; z *= a.z; return *this; }
	vec3& operator/=(vec3 a) { x /= a.x; y /= a.y; z /= a.z; return *this; }
	vec3& operator*=(double a) { x *= a; y *= a; z *= a; return *this; }
	vec3& operator/=(double a) { return *this *= 1 / a; }

	bool zero() const { return x == 0 && y == 0 && z == 0; }
	bool near_zero(double eps) const {
		return abs(x) <= eps && abs(y) <= eps && abs(z) <= eps;
	}
	vec3 unit() const { return zero() ? vec3{} : *this / len(); }
	double len() const { return sqrt(lensqr()); }
	double lensqr() const { return x * x + y * y + z * z; }
};
inline double dot(vec3 a, vec3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
inline vec3 cross(vec3 a, vec3 b) {
	return {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x,
	};
}
inline vec3 operator+(vec3 a, vec3 b) { return a += b; }
inline vec3 operator-(vec3 a, vec3 b) { return a -= b; }
inline vec3 operator*(vec3 a, vec3 b) { return a *= b; }
inline vec3 operator/(vec3 a, vec3 b) { return a /= b; }
inline vec3 operator*(vec3 a, double b) { return a *= b; }
inline vec3 operator/(vec3 a, double b) { return a /= b; }
inline vec3 operator*(double a, vec3 b) { return b * a; }
inline bool operator==(vec3 a, vec3 b) {
	return memcmp(&a, &b, sizeof(vec3)) == 0;
}
inline bool operator!=(vec3 a, vec3 b) { return !(a == b); }
wstring tw2(vec3 v);
typedef vec3 col3;

struct mat3;
mat3 operator+(mat3 a, mat3 const& b);
mat3 operator-(mat3 a, mat3 const& b);
mat3 operator*(double a, mat3 b);
mat3 operator/(mat3 a, double b);
struct mat3 {
	double
		a00 = 0, a01 = 0, a02 = 0,
		a10 = 0, a11 = 0, a12 = 0,
		a20 = 0, a21 = 0, a22 = 0;

	mat3() = default;
	mat3(
		double a00, double a01, double a02,
		double a10, double a11, double a12,
		double a20, double a21, double a22) :
		a00(a00), a01(a01), a02(a02),
		a10(a10), a11(a11), a12(a12),
		a20(a20), a21(a21), a22(a22) {}
	mat3(vec3 v0, vec3 v1, vec3 v2) : mat3(
		v0.x, v1.x, v2.x,
		v0.y, v1.y, v2.y,
		v0.z, v1.z, v2.z) {}

	static mat3 id() {
		return {
			1, 0, 0,
			0, 1, 0,
			0, 0, 1, 
		};
	}
	static mat3 cross(vec3 v) {
		// 指的是 u 变成 v 叉积 u。
		mat3 m;
		m.a01 = -v.z;  m.a02 = v.y;
		m.a12 = -v.x;  m.a10 = v.z;
		m.a20 = -v.y;  m.a21 = v.x; return m;
	}
	static mat3 tensor(vec3 a, vec3 b) {
		return mat3(b.x * a, b.y * a, b.z * a);
	}
	static mat3 rot(vec3 a, double theta) {
		// 这里假设了 a 是单位的。
		mat3 mz = tensor(a, a);
		mat3 mx = mat3::id() - mz, my = cross(a);
		return cos(theta) * mx + sin(theta) * my + mz;
	}

	vec3 v0() const { return { a00, a10, a20 }; }
	vec3 v1() const { return { a01, a11, a21 }; }
	vec3 v2() const { return { a02, a12, a22 }; }
	mat3 operator+() const { return *this; }
	mat3 operator-() const {
		return {
			-a00, -a01, -a02,
			-a10, -a11, -a12,
			-a20, -a21, -a22,
		};
	}
	mat3& operator+=(mat3 const& a) {
		a00 += a.a00; a01 += a.a01; a02 += a.a02;
		a10 += a.a10; a11 += a.a11; a12 += a.a12;
		a20 += a.a20; a21 += a.a21; a22 += a.a22; return *this;
	}
	mat3& operator-=(mat3 const& a) { return *this += -a; }
	mat3& operator*=(double a) {
		a00 *= a; a01 *= a; a02 *= a;
		a10 *= a; a11 *= a; a12 *= a;
		a20 *= a; a21 *= a; a22 *= a; return *this;
	}
	mat3& operator/=(double a) { return *this *= 1 / a; }

	mat3 tsp() const {
		return {
			a00, a10, a20,
			a01, a11, a21,
			a02, a12, a22,
		};
	}
	mat3 inv() const {
		mat3 m;
		m.a00 = a11 * a22 - a12 * a21;
		m.a10 = a12 * a20 - a10 * a22;
		m.a20 = a10 * a21 - a11 * a20;
		m.a01 = a02 * a21 - a01 * a22;
		m.a11 = a00 * a22 - a02 * a20;
		m.a21 = a01 * a20 - a00 * a21;
		m.a02 = a01 * a12 - a02 * a11;
		m.a12 = a02 * a10 - a00 * a12;
		m.a22 = a00 * a11 - a01 * a10; return m / det();
	}
	double det() const {
		return
			a00 * (a11 * a22 - a12 * a21) +
			a01 * (a12 * a20 - a10 * a22) +
			a02 * (a10 * a21 - a11 * a20);
	}
};
inline mat3 operator+(mat3 a, mat3 const& b) { return a += b; }
inline mat3 operator-(mat3 a, mat3 const& b) { return a -= b; }
inline mat3 operator*(mat3 a, double b) { return a *= b; }
inline mat3 operator/(mat3 a, double b) { return a /= b; }
inline mat3 operator*(double a, mat3 b) { return b * a; }
inline vec3 operator*(mat3 const& a, vec3 b) {
	return {
		a.a00 * b.x + a.a01 * b.y + a.a02 * b.z,
		a.a10 * b.x + a.a11 * b.y + a.a12 * b.z,
		a.a20 * b.x + a.a21 * b.y + a.a22 * b.z,
	};
}
inline mat3 operator*(mat3 const& a, mat3 const& b) {
	return {
		a.a00 * b.a00 + a.a01 * b.a10 + a.a02 * b.a20,
		a.a00 * b.a01 + a.a01 * b.a11 + a.a02 * b.a21,
		a.a00 * b.a02 + a.a01 * b.a12 + a.a02 * b.a22,
		a.a10 * b.a00 + a.a11 * b.a10 + a.a12 * b.a20,
		a.a10 * b.a01 + a.a11 * b.a11 + a.a12 * b.a21,
		a.a10 * b.a02 + a.a11 * b.a12 + a.a12 * b.a22,
		a.a20 * b.a00 + a.a21 * b.a10 + a.a22 * b.a20,
		a.a20 * b.a01 + a.a21 * b.a11 + a.a22 * b.a21,
		a.a20 * b.a02 + a.a21 * b.a12 + a.a22 * b.a22,
	};
}

struct cam {
	drect vp;
	vec3 p, look;
	double scl = 0, r = 0;
	// min_y 必须为正数，为了防止除以 0。
	double min_d = 0, max_d = 0;
	double theta = 0, phi = 0, psi = 0;

	dvec ct; 
	mat3 mt;

	explicit cam(drect vp = {});
	void save(FILE* f);
	explicit cam(FILE* f);

	void calc();
	void calc_fp();
};
