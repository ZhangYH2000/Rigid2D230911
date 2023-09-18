#include "connection.h"
#include "body.h"
#include "cur.h" 
#include "draw_px_seg.h"
#include "bgr.h"
#include "connection.h"

#include "my_def.h"
#define found(s) (dic.find(s) != dic.end())
#define getv(nm) if (found(L#nm)) { nm = dic.at(L#nm)->num; }

Connection::Connection() {
	c_rope_normal = { 255, 255, 255 };
	c_rope_tight = { 255, 0, 0 };
	c_link = { 0, 0, 255 };
	c_spring_loose = { 0, 255, 0 };
	c_spring_tight = { 255, 255, 0 };
	c_cord_normal = { 0, 255, 255 };
	c_cord_tight = { 255, 0, 255 };
}
void Connection::save(Cur const& cur, FILE* f) const {
	int sz = 0;
	fwt(type); fwtv(cmd);
	auto pred0 = [this](ptr<Body> b) { return &*b == b0; };
	int ind0 = find_if(cur.bs.begin(), cur.bs.end(), pred0) - cur.bs.begin();
	auto pred1 = [this](ptr<Body> b) { return &*b == b1; };
	int ind1 = find_if(cur.bs.begin(), cur.bs.end(), pred1) - cur.bs.begin();
	fwt(ind0); fwt(ind1);
	fwt(p0_rel); fwt(p1_rel); fwt(len); fwt(hooke);
}
Connection::Connection(Cur& cur, FILE* f) : Connection() {
	int sz = 0;
	frd(type); frdv(cmd); tmp_cmd = cmd;
	int ind0 = 0, ind1 = 0;
	frd(ind0); frd(ind1);
	b0 = &*cur.bs[ind0]; b1 = &*cur.bs[ind1];
	frd(p0_rel); frd(p1_rel); frd(len); frd(hooke);
	refresh(cur); sign_up(); generate();
}

void Connection::read_cfg(Var const& cfg) {
	auto dic = cfg.dic;
	getv(len);  getv(hooke);
	if (found(L"type")) {
		auto md = dic.at(L"type");
		if (md->typ == L"str") {
			auto s = md->str;
			if (s == L"rope") { type = CON_ROPE; }
			else if (s == L"link") { type = CON_LINK; }
			else if (s == L"spring") { type = CON_SPRG; }
			else if (s == L"cord") { type = CON_CORD; }
		}
		else { type = md->num; }
	}
}
void Connection::sign_up() {
	b0->cons.push_back(this);
	if (b0 != b1) { b1->cons.push_back(this); }
}
void Connection::generate() {
	p0 = b0->o + b0->tsf * p0_rel;
	p1 = b1->o + b1->tsf * p1_rel;
}
void Connection::refresh(Cur& cur) {
	gl[L"conn"] = msh<Var>();
	Execute(gl, Compile(cmd));
	if (gl.find(L"conn") == gl.end()) { return; }
	read_cfg(*gl[L"conn"]);

	generate();
}

void Connection::resolve_rope_link(bool equal_repos) {
	generate();
	if (b0 == b1) { return; }
	if (b0->inv_m == 0 && b1->inv_m == 0) { return; }

	vec2 r0 = p0 - b0->o;
	vec2 r1 = p1 - b1->o;
	r0 = vec2(-r0.y, r0.x);
	r1 = vec2(-r1.y, r1.x);
	vec2 v0 = b0->v + b0->v_ang * r0;
	vec2 v1 = b1->v + b1->v_ang * r1;

	vec2 d = p1 - p0;
	double cur_len = d.len();
	d = d.unit();
	tight = cur_len > len;
	if (!tight && type == CON_ROPE) { return; }

	double rd0 = dot(r0, d);
	double rd1 = dot(r1, d);
	double inv_i0 = b0->inv_m + rd0 * rd0 * b0->inv_i;
	double inv_i1 = b1->inv_m + rd1 * rd1 * b1->inv_i;
	double jd = dot(v0 - v1, d);
	if (type == CON_ROPE) { jd = min(0.0, jd); }
	vec2 j = -jd / (inv_i0 + inv_i1) * d;

	b0->v += j * b0->inv_m;
	b1->v -= j * b1->inv_m;
	b0->v_ang += dot(j, r0) * b0->inv_i;
	b1->v_ang -= dot(j, r1) * b1->inv_i;

	if (equal_repos) {
		if (b0->inv_m == 0) { b1->o += (cur_len - len) * d; }
		else if (b1->inv_m == 0) { b0->o -= (cur_len - len) * d; }
		else {
			b0->o += (cur_len - len) * d / 2;
			b1->o -= (cur_len - len) * d / 2;
		} return;
	}
	double sum = b0->inv_m + b1->inv_m;
	b0->o += d * (cur_len - len) * b0->inv_m / sum;
	b1->o -= d * (cur_len - len) * b1->inv_m / sum;
}
void Connection::resolve_cord_spring(double sdt) {
	generate();
	if (b0 == b1) { return; }
	if (b0->inv_m == 0 && b1->inv_m == 0) { return; }

	vec2 r0 = p0 - b0->o;
	vec2 r1 = p1 - b1->o;
	r0 = vec2(-r0.y, r0.x);
	r1 = vec2(-r1.y, r1.x);
	vec2 v0 = b0->v + b0->v_ang * r0;
	vec2 v1 = b1->v + b1->v_ang * r1;

	vec2 d = p1 - p0;
	double cur_len = d.len();
	d = d.unit();
	if (d.zero()) { d = vec2(1, 0); }
	tight = cur_len > len;
	if (!tight && type == CON_CORD) { return; }

	double rd0 = dot(r0, d);
	double rd1 = dot(r1, d);
	vec2 j = (cur_len - len) * hooke * sdt * d;

	b0->v += j * b0->inv_m;
	b1->v -= j * b1->inv_m;
	b0->v_ang += dot(j, r0) * b0->inv_i;
	b1->v_ang -= dot(j, r1) * b1->inv_i;
}
void Connection::Resolve(double sdt, bool equal_repos) {
	switch (type) {
	case CON_ROPE: case CON_LINK: resolve_rope_link(equal_repos); return;
	case CON_CORD: case CON_SPRG: resolve_cord_spring(sdt); return;
	}
}
void Connection::Render(Cur& cur) const {
	dcol c;
	switch (type) {
	case CON_ROPE: c = tight ? c_rope_tight : c_rope_normal; break;
	case CON_LINK: c = c_link; break;
	case CON_CORD: c = tight ? c_cord_tight : c_cord_normal; break;
	case CON_SPRG: c = tight ? c_spring_tight : c_spring_loose; break;
	}
	draw_px_seg(scr, dscr, p0, p1, 2, bgr.vp(), c);
}

Body* the_other(Body* b, Connection const& con) {
	if (con.b0 == b && con.b1 == b) {
		return NULL;
	}
	if (con.b0 != b && con.b1 != b) {
		return NULL;
	}
	return con.b0 == b ? con.b1 : con.b0;
}
