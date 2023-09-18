#pragma once
#include "utils.h"

struct dvec {
	int x = 0, y = 0;
	dvec() = default;
	dvec(int x, int y) : x(x), y(y) {}
	explicit dvec(wstring const& s);

	int lensqr() const { return x * x + y * y; }
	dvec const& operator+() const { return *this; }
	dvec operator-() const { return { -x, -y }; }
	dvec& operator+=(dvec a) { x += a.x; y += a.y; return *this; }
	dvec& operator-=(dvec a) { return *this += -a; }
	dvec& operator*=(dvec a) { x *= a.x; y *= a.y; return *this; }
	dvec& operator/=(dvec a) { x /= a.x; y /= a.y; return *this; }
	dvec& operator*=(int a) { x *= a; y *= a; return *this; }
	dvec& operator/=(int a) { x /= a; y /= a; return *this; }
};
inline dvec operator+(dvec a, dvec b) { return a += b; }
inline dvec operator-(dvec a, dvec b) { return a -= b; }
inline dvec operator*(dvec a, dvec b) { return a *= b; }
inline dvec operator/(dvec a, dvec b) { return a /= b; }
inline dvec operator*(dvec a, int b) { return a *= b; }
inline dvec operator/(dvec a, int b) { return a /= b; }
inline dvec operator*(int a, dvec b) { return b * a; }
inline bool operator==(dvec a, dvec b) {
	return memcmp(&a, &b, sizeof(dvec)) == 0;
}
inline bool operator!=(dvec a, dvec b) { return !(a == b); }
wstring tw(dvec v);

struct drect {
	dvec tl;
	int w = 0, h = 0;
	drect() = default;
	drect(int w, int h) : drect({}, w, h) {}
	drect(dvec tl, int w, int h) : tl(tl), w(w), h(h) {}

	int left() const { return tl.x; }
	int top() const { return tl.y; }
	int right() const { return left() + w; }
	int bottom() const { return top() + h; }
	dvec ct() const { return tl + dvec(w, h) / 2; }
};
bool insd(dvec v, drect r);
// 这里可能会出现负的 drect。
drect overlap(drect a, drect b);
