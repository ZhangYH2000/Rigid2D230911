#pragma once
#include "dvec.h"

struct vec2;
inline vec2 operator/(vec2 a, double b);
inline bool operator==(vec2 a, vec2 b);
struct vec2 {
	double x = 0, y = 0;
	vec2() = default;
	vec2(double x, double y) : x(x), y(y) {}
	explicit vec2(double a) : vec2(a, a) {}
	explicit vec2(wstring const& s);
	explicit vec2(dvec v) : x(v.x), y(v.y) {}
	explicit operator dvec() const { return dvec(int(x), int(y)); }

	vec2 operator+() const { return *this; }
	vec2 operator-() const { return { -x, -y }; }
	vec2& operator+=(vec2 a) { x += a.x; y += a.y; return *this; }
	vec2& operator-=(vec2 a) { return *this += -a; }
	vec2& operator*=(vec2 a) { x *= a.x; y *= a.y; return *this; }
	vec2& operator/=(vec2 a) { x /= a.x; y /= a.y; return *this; }
	vec2& operator*=(double a) { x *= a; y *= a; return *this; }
	vec2& operator/=(double a) { return *this *= 1 / a; }

	bool zero() const { return x == 0 && y == 0; }
	bool near_zero(double eps) const {
		return abs(x) <= eps && abs(y) <= eps;
	}
	vec2 unit() const { return zero() ? vec2{} : *this / len(); }
	double len() const { return sqrt(lensqr()); }
	double lensqr() const { return x * x + y * y; }
};
inline double dot(vec2 a, vec2 b) { return a.x * b.x + a.y * b.y; }
inline vec2 operator+(vec2 a, vec2 b) { return a += b; }
inline vec2 operator-(vec2 a, vec2 b) { return a -= b; }
inline vec2 operator*(vec2 a, vec2 b) { return a *= b; }
inline vec2 operator/(vec2 a, vec2 b) { return a /= b; }
inline vec2 operator*(vec2 a, double b) { return a *= b; }
inline vec2 operator/(vec2 a, double b) { return a /= b; }
inline vec2 operator*(double a, vec2 b) { return b * a; }
inline bool operator==(vec2 a, vec2 b) {
	return memcmp(&a, &b, sizeof(vec2)) == 0;
}
inline bool operator!=(vec2 a, vec2 b) { return !(a == b); }
wstring tw2(vec2 v);

struct rect {
	vec2 tl;
	double w = 0, h = 0;

	rect() = default;
	rect(double w, double h) : rect({}, w, h) {}
	rect(vec2 tl, double w, double h) : tl(tl), w(w), h(h) {}

	double left() const { return tl.x; }
	double top() const { return tl.y; }
	double right() const { return left() + w; }
	double bottom() const { return top() + h; }
};
bool insd(vec2 v, rect r);

struct mat2;
inline mat2 operator/(mat2 a, double b);
struct mat2 {
	double a00 = 0, a01 = 0, a10 = 0, a11 = 0;
	mat2() = default;
	mat2(double a00, double a01, double a10, double a11) :
		a00(a00), a01(a01), a10(a10), a11(a11) {}
	mat2(vec2 v0, vec2 v1) : mat2(v0.x, v1.x, v0.y, v1.y) {}

	static mat2 rot(double rad) {
		double c = cos(rad), s = sin(rad);
		return { c, -s, s, c };
	}
	static mat2 id() { return { 1, 0, 0, 1 }; }

	mat2 operator+() const { return *this; }
	mat2 operator-() const { return { -a00, -a01, -a10, -a11 }; }
	mat2& operator+=(mat2 a) {
		a00 += a.a00; a01 += a.a01; a10 += a.a10; a11 += a.a11;
		return *this;
	}
	mat2& operator-=(mat2 a) { return *this += -a; }
	mat2& operator*=(double a) {
		a00 *= a; a01 *= a; a10 *= a; a11 *= a;
		return *this;
	}
	mat2& operator/=(double a) { return *this *= 1 / a; }

	mat2 tsp() const { return { a00, a10, a01, a11 }; }
	mat2 inv() const { return mat2(a11, -a01, -a10, a00) / det(); }
	mat2 abs() const {
		return mat2(::abs(a00), ::abs(a01), ::abs(a10), ::abs(a11));
	}
	double det() const { return a00 * a11 - a01 * a10; }
};
inline mat2 operator+(mat2 a, mat2 b) { return a += b; }
inline mat2 operator-(mat2 a, mat2 b) { return a -= b; }
inline mat2 operator*(mat2 a, double b) { return a *= b; }
inline mat2 operator/(mat2 a, double b) { return a /= b; }
inline mat2 operator*(double a, mat2 b) { return b * a; }
inline vec2 operator*(mat2 a, vec2 b) {
	return {
		a.a00 * b.x + a.a01 * b.y,
		a.a10 * b.x + a.a11 * b.y
	};
}
inline mat2 operator*(mat2 a, mat2 b) {
	return {
		a.a00 * b.a00 + a.a01 * b.a10,
		a.a00 * b.a01 + a.a01 * b.a11,
		a.a10 * b.a00 + a.a11 * b.a10,
		a.a10 * b.a01 + a.a11 * b.a11
	};
}

vec2 bezier(vector<vec2> ps, double t);
double dist_sqr(vec2 a, vec2 b, vec2 p);
