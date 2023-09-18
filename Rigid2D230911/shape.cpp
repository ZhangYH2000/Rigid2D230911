#include "shape.h"

Shape::Shape(double _r, vec2 _o_rel) {
	ball = true;
	r = _r; o_rel = _o_rel;
}
Shape::Shape(vector<vec2> const& _vs_rel) {
	vs_rel = _vs_rel;
	vs.resize(vs_rel.size());
}
void Shape::save(FILE* f) const {
	int sz = 0;
	fwt(ball); fwt(r); fwt(ang); fwt(o_rel);
	fwtv(vs_rel);
}
Shape::Shape(FILE* f) {
	int sz = 0;
	frd(ball); frd(r); frd(ang); frd(o_rel);
	frdv(vs_rel); vs.resize(vs_rel.size());
}

vec2 Shape::rnd_rel() const {
	if (ball) {
		double phi = frnd(2 * PI);
		// 还没验证过。
		double rad = sqrt(frnd(1)) * r;
		return vec2(cos(phi), sin(phi)) * rad;
	}

	double prob = frnd(area);
	rep(i, 0, vs.size()) {
		int j = (i + 1) % vs.size();
		auto p = vs_rel[i], q = vs_rel[j];
		prob -= mat2(p - o_rel, q - o_rel).det() / 2;

		if (prob < 0 || i == vs.size() - 1) {
			return rnd_tri(o_rel, p, q);
		}
	}  return {};
}
bool Shape::inside(vec2 p) const {
	if (ball) { return (p - o).lensqr() < r * r; }

	rep(i, 0, vs.size()) {
		int j = (i + 1) % vs.size();
		if (inside_tri(p, o, vs[i], vs[j])) { return true; }
	} return false;
}
aabb Shape::bounding_box() const {
	if (!ball) {
		aabb b;
		for (auto v : vs) { b.expand(v); } return b;
	}
	return aabb(o.x - r, o.x + r, o.y - r, o.y + r); 
}

vec2 rnd_tri(vec2 v0, vec2 v1, vec2 v2) {
	double x0 = frnd(1), x1 = frnd(1);
	if (x0 + x1 > 1) {
		swap(x0, x1);
		x0 = 1 - x0;
		x1 = 1 - x1;
	}
	return x0 * v0 + x1 * v1 + (1 - x0 - x1) * v2;
}
bool inside_tri(vec2 v, vec2 v0, vec2 v1, vec2 v2) {
	vec3 u(v.x, v.y, 1);
	vec3 u0(v0.x, v0.y, 1);
	vec3 u1(v1.x, v1.y, 1);
	vec3 u2(v2.x, v2.y, 1);
	vec3 t = mat3(u0, u1, u2).inv() * u;
	return 
		insd(t.x, 0.0, 1.0) && 
		insd(t.y, 0.0, 1.0) && 
		insd(t.z, 0.0, 1.0);
}
vector<vec2> rect(double w, double h) {
	vector<vec2> sh;
	sh.push_back(vec2(+w, +h) / 2);
	sh.push_back(vec2(-w, +h) / 2);
	sh.push_back(vec2(-w, -h) / 2);
	sh.push_back(vec2(+w, -h) / 2); return sh;
}
