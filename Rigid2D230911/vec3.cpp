#include "vec3.h"

vec3::vec3(wstring const& s) {
	int i = 0;
	x = fetch_num(i, s);
	y = fetch_num(i, s);
	z = fetch_num(i, s);
}
wstring tw2(vec3 v) {
	return L"(" + tw2(v.x) + L"," + tw2(v.y) + L"," + tw2(v.z) + L")";
}

cam::cam(drect vp) : vp(vp) {
	r = 1000; scl = 500;
	max_d = 1e6; min_d = 1; calc();
}
void cam::save(FILE* f) {
	fwt(vp); fwt(p); fwt(look);
	fwt(scl); fwt(r);
	fwt(min_d); fwt(max_d);
	fwt(theta); fwt(phi); fwt(psi);
}
cam::cam(FILE* f) {
	frd(vp); frd(p); frd(look);
	frd(scl); frd(r);
	frd(min_d); frd(max_d);
	frd(theta); frd(phi); frd(psi); calc();
}

void cam::calc() {
	p.y = -cos(theta) * cos(phi);
	p.x = sin(theta) * cos(phi);
	p.z = sin(phi);
	p = p * r + look;

	// 下面可能会产生奇异的情况。
	vec3 vy = (look - p).unit();
	vec3 vx = cross(vy, vec3(0, 0, 1)).unit();
	vec3 vz = cross(vx, vy);
	mt = mat3(vx, vy, vz).tsp();
	mt = mat3::rot(vec3(0, 1, 0), psi) * mt;
	ct = vp.tl + dvec(vp.w, vp.h) / 2;
}
void cam::calc_fp() {
	// 第一人称模式，舍弃了 look, r。
	vec3 vy;
	vy.z = sin(phi);
	vy.x = cos(phi) * sin(theta);
	vy.y = cos(phi) * cos(theta);
	vec3 vx = cross(vy, vec3(0, 0, 1)).unit();
	vec3 vz = cross(vx, vy);

	mt = mat3(vx, vy, vz).tsp();
	mt = mat3::rot(vec3(0, 1, 0), psi) * mt;
	ct = vp.tl + dvec(vp.w, vp.h) / 2;
}
