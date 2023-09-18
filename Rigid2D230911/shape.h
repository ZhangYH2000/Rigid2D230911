#pragma once
#include "tile.h"
#include "aabb.h"

struct Cur;
struct Shape {
	bool ball = false;
	double r = 0;
	double ang = 0;
	double area = 0;
	vec2 o_rel, o;
	vector<vec2> vs_rel, vs;

	Shape(double _r, vec2 _o_rel = {});
	Shape(vector<vec2> const& _vs_rel);
	void save(FILE* f) const;
	Shape(FILE* f);

	vec2 rnd_rel() const;
	bool inside(vec2 p) const;
	aabb bounding_box() const;
};

vec2 rnd_tri(vec2 v0, vec2 v1, vec2 v2);
bool inside_tri(vec2 v, vec2 v0, vec2 v1, vec2 v2);
vector<vec2> rect(double w, double h);
