#include "collision.h"
#include "cur.h"
#include "bgr.h"
#include "body.h"
#include "shape.h"
#include "draw_geo.h"

#include "my_def.h"

void Collision::Render(Cur& cur) const {
	draw_eclipse(scr, dscr, 100, c, 5, 5, bgr.vp(), dcol(255, 255, 0));
}
void Collision::Resolve(bool equal_repos) {
	double e = min(b0->e, b1->e);
	double mu_d = min(b0->mu_d, b1->mu_d);
	double mu_s = min(b0->mu_s, b1->mu_s);
	
	vec2 r0 = c - b0->o;
	r0 = vec2(-r0.y, r0.x);
	vec2 r1 = c - b1->o;
	r1 = vec2(-r1.y, r1.x);
	vec2 v0 = b0->v + b0->v_ang * r0;
	vec2 v1 = b1->v + b1->v_ang * r1;

	vec2 j;
	double rn0 = dot(r0, n);
	double rn1 = dot(r1, n);
	double inv_i0 = b0->inv_m + rn0 * rn0 * b0->inv_i;
	double inv_i1 = b1->inv_m + rn1 * rn1 * b1->inv_i;
	double jn = (1 + e) * max(0.0, dot(v0 - v1, n)) / (inv_i0 + inv_i1);

	j = -jn * n;
	b0->v += j * b0->inv_m;
	b1->v -= j * b1->inv_m;
	b0->v_ang += dot(j, r0) * b0->inv_i;
	b1->v_ang -= dot(j, r1) * b1->inv_i;

	vec2 u0 = b0->v + b0->v_ang * r0;
	vec2 u1 = b1->v + b1->v_ang * r1;

	double js = mu_s * jn;
	double jd = mu_d * jn;
	vec2 t = vec2(-n.y, n.x);
	double jt = dot(u0 - u1, t);
	if (jt < 0) { jt = -jt; t = -t; }
	double rt0 = dot(r0, t);
	double rt1 = dot(r1, t);
	double inv_it0 = b0->inv_m + rt0 * rt0 * b0->inv_i;
	double inv_it1 = b1->inv_m + rt1 * rt1 * b1->inv_i;
	jt /= inv_it0 + inv_it1;

	if (jt < js) { j = -jt * t; }
	else { j = -jd * t; }

	b0->v += j * b0->inv_m;
	b1->v -= j * b1->inv_m;
	b0->v_ang += dot(j, r0) * b0->inv_i;
	b1->v_ang -= dot(j, r1) * b1->inv_i;

	if (equal_repos) {
		if (b0->inv_m == 0) { b1->o += n * d; }
		else if (b1->inv_m == 0) { b0->o -= n * d; }
		else {
			b0->o -= n * d / 2;
			b1->o += n * d / 2;
		} return;
	}
	// 我是真的不知道这个有没有道理。
	double sum = b0->inv_m + b1->inv_m;
	b0->o -= n * d * b0->inv_m / sum;
	b1->o += n * d * b1->inv_m / sum;
}

void Collide(Body& b0, Body& b1, vector<ptr<Collision>>& out, double eps_paralell) {
	if (b0.inv_m == 0 && b1.inv_m == 0) { return; }
	if (!overlap(b0.box, b1.box)) { return; }

	for (auto sh0 : b0.shs) for (auto sh1 : b1.shs) {
		Collision col;
		col.b0 = &b0; col.b1 = &b1;
		if (sh0->ball && sh1->ball) {
			if (!collide_balls(*sh0, *sh1, col)) { continue; }
		} else {
			if (!collide(*sh0, *sh1, col, false)) { continue; }
			if (!collide(*sh1, *sh0, col, true)) { continue; }
		}
		
		// 下面的小于号有一点重要。
		if (!sh0->ball && !sh1->ball && col.diff < eps_paralell) {
			double dsqr_close = DBL_MAX;
			find_contact(*sh0, *sh1, col.c, dsqr_close);
			find_contact(*sh1, *sh0, col.c, dsqr_close);
		}
		out.push_back(msh<Collision>(col));
	}
}
bool collide(Shape& sh0, Shape& sh1, Collision& out, bool reverse) {
	vec2 c;
	if (sh0.ball) {
		rep(i, 0, sh1.vs.size()) {
			vec2 n = (sh1.vs[i] - sh0.o).unit();
			if (n.zero()) { n = vec2(0, 1); }
			auto d = sub_collide_poly(n, sh0.o + n * sh0.r, sh1, c);
			if (d < 0) { return false; }
			if (d < out.d) {
				out.diff = out.d - d;
				out.d = d;
				out.c = c;
				out.n = reverse ? -n : n;
			}
		} return true;
	}

	rep(i, 0, sh0.vs.size()) {
		int j = (i + 1) % sh0.vs.size();
		vec2 n = sh0.vs[j] - sh0.vs[i];
		n = vec2(n.y, -n.x).unit();
		auto d = sh1.ball ?
			sub_collide_ball(n, sh0.vs[i], sh1, c) :
			sub_collide_poly(n, sh0.vs[i], sh1, c);
		if (d < 0) { return false; }
		if (d < out.d) {
			out.diff = out.d - d;
			out.d = d;
			out.c = c;
			out.n = reverse ? -n : n;
		}
	} return true;
}
void find_contact(Shape& sh0, Shape& sh1, vec2 &c, double &dsqr_close) {
	rep(i, 0, sh0.vs.size()) {
		int j = (i + 1) % sh0.vs.size();
		for (auto v : sh1.vs) {
			double dsqr = dist_sqr(sh0.vs[i], sh0.vs[j], v);
			// 这里的小于号很重要。
			if (dsqr < dsqr_close) {
				dsqr_close = dsqr; c = v;
			}
		}
	}
}
bool collide_balls(Shape& sh0, Shape& sh1, Collision& out) {
	vec2 o10 = sh1.o - sh0.o;
	double d = sh0.r + sh1.r - o10.len();
	if (d < 0) { return false; }
	out.d = d;
	out.n = o10.zero() ? vec2(0, 1) : o10.unit();
	if (sh0.r < sh1.r) {
		out.c = sh0.o + sh0.r * out.n;
	} else {
		out.c = sh1.o - sh1.r * out.n;
	} return true;
}
double sub_collide_ball(vec2 n, vec2 o, Shape const& sh, vec2& c) {
	auto tmp = dot(sh.o - o, n) - sh.r;
	c = sh.o - sh.r * n; return -tmp;
}
double sub_collide_poly(vec2 n, vec2 o, Shape const& sh, vec2& c) {
	double out = DBL_MAX;
	for (auto v : sh.vs) {
		auto tmp = dot(v - o, n);
		if (tmp < out) {
			out = tmp; c = v;
		}
	} return -out;
}
