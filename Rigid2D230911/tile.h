#pragma once
#include "vec3.h"

struct dcol {
	BYTE b = 0, g = 0, r = 0;
	dcol() = default;
	dcol(BYTE r, BYTE g, BYTE b) : r(r), g(g), b(b) {}
	explicit dcol(BYTE a) : dcol(a, a, a) {}
	explicit dcol(wstring const& s);
	explicit dcol(col3 c) {
		r = clmp<int>(0, 255, c.r() * 255);
		g = clmp<int>(0, 255, c.g() * 255);
		b = clmp<int>(0, 255, c.b() * 255);
	}
	explicit operator col3() const { return col3(r, g, b) / 255; }
};
inline bool operator==(dcol a, dcol b) {
	return memcmp(&a, &b, sizeof(dcol)) == 0;
}
inline bool operator!=(dcol a, dcol b) { return !(a == b); }
wstring tw(dcol c);
void cover(dcol& bc, BYTE& ba, dcol fc, BYTE fa);

struct tile {
	int w = 0, h = 0;
	vector<dcol> cols;
	vector<BYTE> as;

	tile() = default;
	tile(int w, int h, dcol c = {}, BYTE a = 0);
	// src ²»ÄÜÎª¿Õ¡£
	tile(int w, int h, tile const& src, drect vp_src);
	void save(wstring const& nm) const;
	tile(wstring const& nm, bool* ok = NULL);

	int n() const;
	drect rect() const;
	void fill(dcol c, BYTE a = 255);
};
typedef vector<double> dbuf;
