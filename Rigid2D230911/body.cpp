#include "body.h"
#include "cur.h"
#include "bgr.h"
#include "cur.h"
#include "creator.h"
#include "shape.h"
#include "draw_tri.h"
#include "draw_geo.h"
#include "draw_curve.h"
#include "draw_px_seg.h"
#include "connection.h"

#include "my_def.h"
#define tls (cur.rect_scene.tl)
#define found(s) (dic.find(s) != dic.end())
#define getv(nm) if (found(L#nm)) { nm = dic.at(L#nm)->num; }

// 这些构造函数真的很混乱。
Body::Body() {
	density = 1;
	damp_v = 0.1; damp_v_ang = 0.1;
	e = 0.7; mu_s = 0.5; mu_d = 0.3;

	dep_shape = 0; dep_point = 1;
	r_point = 8;
	c_border = { 255, 255, 255 };
	c_hovered = { 0, 0, 255 };
	c_dragged = { 255, 0, 0 };
	c_selected = { 255, 0, 255 };
	c_selected_con = { 255, 255, 0 };
}
Body::Body(vec2 const& _o) : Body() {
	point = true;
	o = _o;
}
Body::Body(ptr<Shape> sh) : Body() {
	shs = { sh };
}
Body::Body(vector<ptr<Shape>> const& _shs) : Body() {
	shs = _shs;
}
void Body::save(FILE* f) const {
	int sz = 0;
	fwt(point); fwt(fixed);
	sz = shs.size();
	fwt(sz);
	for (auto sh : shs) { sh->save(f); }
	fwt(o); fwt(v); fwtv(cmd);
	fwt(ang); fwt(v_ang); fwt(density); 
	fwt(e); fwt(mu_s); fwt(mu_d); 
	fwt(damp_v); fwt(damp_v_ang);
	fwt(charge_density); fwt(c_inner);
}
Body::Body(Cur& cur, FILE* f) : Body() {
	int sz = 0;
	frd(point); frd(fixed);
	frd(sz);
	rep(i, 0, sz) {
		shs.push_back(msh<Shape>(f));
	}
	frd(o); frd(v); frdv(cmd); tmp_cmd = cmd;
	frd(ang); frd(v_ang); frd(density);
	frd(e); frd(mu_s); frd(mu_d);
	frd(damp_v); frd(damp_v_ang);
	frd(charge_density); frd(c_inner);

	refresh(cur);
}

double Body::dep() const {
	return point ? dep_point : dep_shape;
}
bool Body::inside(vec2 p) const {
	if (point) { return (p - o).lensqr() < r_point * r_point; }

	for (auto sh : shs) {
		if (sh->inside(p)) { return true; }
	} return false;
}

void Body::refresh(Cur &cur) {
	gl[L"body"] = msh<Var>();
	Execute(gl, Compile(cmd));
	if (gl.find(L"body") == gl.end()) { return; }
	read_cfg(*gl[L"body"]);

	// 这些单独放是因为它们无法只能通过 cmd 获得，无法保存。
	auto dic = gl[L"body"]->dic;
	getv(preset_o); getv(preset_ang); getv(preset_v_ang);
	if (found(L"o_prog")) { mkp(o_prog)(dic.at(L"o_prog")->procs); }
	if (found(L"ang_prog")) { mkp(ang_prog)(dic.at(L"ang_prog")->procs); }
	if (found(L"v_ang_prog")) { mkp(v_ang_prog)(dic.at(L"v_ang_prog")->procs); }
	Init();
}
void Body::read_cfg(Var const& cfg) {
	auto dic = cfg.dic;
	if (found(L"o")) { o = tv2(*dic[L"o"]); }
	if (found(L"v")) { v = tv2(*dic[L"v"]); }
	if (found(L"col")) { c_inner = (dcol)tv3(*dic[L"col"]); }
	getv(ang); getv(v_ang); getv(e); getv(mu_s); getv(mu_d);
	getv(damp_v); getv(damp_v_ang); getv(fixed);
	getv(density); getv(charge_density); 
}
vec2 Body::rnd_rel() const {
	if (point) { return {}; }

	double p = frnd(area);
	for (auto sh : shs) {
		p -= sh->area;
		if (p < 0) {
			return sh->rnd_rel();
		}
	} return shs.back()->rnd_rel();
}
void Body::generate() {
	tsf = mat2::rot(ang);
	for (auto sh : shs) {
		sh->o = o + tsf * sh->o_rel;
		sh->ang = ang;
		rep(i, 0, sh->vs.size()) {
			sh->vs[i] = o + tsf * sh->vs_rel[i];
		}
	} 
	update_box();
}
void Body::update_box() {
	box = aabb();
	for (auto sh : shs) {
		box = aabb(box, sh->bounding_box());
	}
}
void Body::warp(drect rc) {
	// 这个方法已经过时了。
	if (inv_m == 0 || dragged) { return; }

	if (o.x < rc.left()) { o.x += rc.w; }
	else if (o.x > rc.right()) { o.x -= rc.w; }
	if (o.y < rc.top()) { o.y += rc.h; }
	else if (o.y > rc.bottom()) { o.y -= rc.h; }
}
void Body::register_grid(Cur& cur) {
	if (point) { return; }

	int x0 = floor((box.x0 - tls.x) / cur.s_grid);
	int y0 = floor((box.y0 - tls.y) / cur.s_grid);
	int x1 = floor((box.x1 - tls.x) / cur.s_grid);
	int y1 = floor((box.y1 - tls.y) / cur.s_grid);
	x0 = clmp(x0, 0, cur.nx_grid - 1);
	x1 = clmp(x1, 0, cur.nx_grid - 1);
	y0 = clmp(y0, 0, cur.ny_grid - 1);
	y1 = clmp(y1, 0, cur.ny_grid - 1);
	if (x1 > x0 + 1 || y1 > y0 + 1) {
		cur.out_grid.push_back(this); return;
	}
	cur.grid[y0 * cur.nx_grid + x0].push_back(this);
}

void Body::Init(bool repos_o) {
	// 这里假设是之前没有乱改成员。
	if (point) { return; }

	area = 0; vec2 center;
	inertia = 0; 
	for (auto sh : shs) {
		sh->area = 0; vec2 sub_center;
		if (sh->ball) {
			sh->area = PI * sh->r * sh->r;
			sub_center = sh->area * sh->o_rel;
			inertia += sh->area * (sh->r * sh->r) / 2;
		} else {
			rep(i, 0, sh->vs_rel.size()) {
				int j = (i + 1) % sh->vs_rel.size();
				auto p = sh->vs_rel[i], q = sh->vs_rel[j];
				double tri_area = mat2(p, q).det() / 2;
				double tri_inertia = dot(p, p) + dot(p, q) + dot(q, q);
				tri_inertia *= tri_area / 6;

				sub_center += tri_area * (p + q) / 3;
				sh->area += tri_area;
				inertia += tri_inertia;
			}
		}

		area += sh->area;
		center += sub_center;
		sh->o_rel = sub_center / sh->area;
	}

	center /= area;
	charge = charge_density * area;
	inv_m = fixed ? 0 : 1 / (density * area);
	inv_i = fixed ? 0 : 1 / (density * inertia);
	if (repos_o) { o = center; }
	for (auto sh : shs) {
		sh->o_rel -= center;
		for (auto& v : sh->vs_rel) { v -= center; }
	}
	generate();
}
void Body::Render(Cur& cur) const {
	bool selected_con = cur.con_sel && 
		(cur.con_sel->b0 == this || cur.con_sel->b1 == this);
	dcol c_normal;
	switch (cur.display) {
	case DISP_COL: c_normal = c_inner; break;
	if (point) { break; }
	case DISP_ENERGY: {
		double energy = v.lensqr() + v_ang * v_ang * inertia / area;
		c_normal = (dcol)col3(energy * cur.energy_mul, 0, 0);
	} break;
	case DISP_CHARGE: {
		c_normal = charge_density > 0 ?
			(dcol)col3(charge_density * cur.charge_mul, 0, 0) :
			(dcol)col3(0, 0, -charge_density * cur.charge_mul);
	} break;
	}

	dcol c =
		dragged ? c_dragged :
		(cur.body_sel == this) ? c_selected :
		hovered ? c_hovered : 
		selected_con ? c_selected_con : c_normal;
	if (point) {
		draw_eclipse(scr, dscr, dep(), o, r_point, r_point, bgr.vp(), c);
		draw_eclipse_frame(scr, dscr, dep(), o, r_point, r_point, bgr.vp(), c_border, 20);
	}
	for (auto sh : shs) {
		if (sh->ball) {
			draw_eclipse(scr, dscr, dep(), sh->o, sh->r, sh->r, bgr.vp(), c);
			draw_eclipse_frame(scr, dscr, dep(), 
				sh->o, sh->r, sh->r, bgr.vp(), c_border, 30); 
			auto tip = vec2(cos(sh->ang), sin(sh->ang)) * sh->r + sh->o;
			draw_px_seg(scr, dscr, sh->o, tip, dep(), bgr.vp(), c_border); continue;
		}

		rep(i, 0, sh->vs.size()) {
			int j = (i + 1) % sh->vs.size();
			draw_tri(scr, dscr, bgr.vp(), sh->o, sh->vs[i], sh->vs[j], dep(), c);
		}
		rep(i, 0, sh->vs.size()) {
			int j = (i + 1) % sh->vs.size();
			draw_px_seg(scr, dscr, sh->vs[i], sh->vs[j], dep(), bgr.vp(), c_border);
		}
	}

	if (dragged) {
		vec2 p1 = vec2(msp);
		vec2 p0 = o + tsf * p_drag_rel;
		draw_px_seg(scr, dscr, p0, p1, dep(), bgr.vp(), dcol(0, 255, 255)); 
	}

	if (!track.empty()) {
		vec2 p0 = track.front();
		for (auto p1 : track) {
			draw_px_seg(scr, dscr, p0, p1, 10, bgr.vp(), dcol(255, 0, 255));
			p0 = p1;
		}
	}
}
void Body::Step(Cur& cur, double sdt) {
	if (inv_m) {
		v -= damp_v * v * sdt;
		v += cur.gravity * sdt;
		v_ang -= damp_v_ang * v_ang * sdt;
		if (preset_v_ang) { follow_preset_v_ang(cur); }
		o += v * sdt;
		ang += v_ang * sdt;
	} else {
		v = {}; v_ang = 0;
		if (preset_o) { follow_preset_o(cur, sdt); }
		if (preset_ang) { follow_preset_ang(cur, sdt); }
	}

	if (dragged) {
		if (msd[2]) { hdl_dragged_whole(cur); }
		else if (!kb && kbd[L'F']) { hdl_dragged_force(cur, sdt); }
		else if (inv_m == 0) { hdl_dragged_whole(cur); }
		else { hdl_dragged_point(cur); }
	}

	generate();
}
void Body::follow_preset_o(Cur& cur, double sdt) {
	if (!o_prog) { return; }
	Scope sc; bool ret = false;
	sc[L"t"] = msh<Var>(cur.t);
	auto p1_old = tv2(Execute(ret, gl, sc, *o_prog)->vec);
	sc.clear(); sc[L"t"] = msh<Var>(cur.t + sdt); ret = false;
	auto p1 = tv2(Execute(ret, gl, sc, *o_prog)->vec);
	auto v1 = (p1 - p1_old) / sdt;

	// 其实我觉得应当选用时间较小的值。
	o = p1; v = v1;
}
void Body::follow_preset_ang(Cur& cur, double sdt) {
	if (!ang_prog) { return; }
	Scope sc; bool ret = false;
	sc[L"t"] = msh<Var>(cur.t);
	auto ang1_old = Execute(ret, gl, sc, *ang_prog)->num;
	sc.clear(); sc[L"t"] = msh<Var>(cur.t + sdt); ret = false;
	auto ang1 = Execute(ret, gl, sc, *ang_prog)->num;
	auto v_ang1 = (ang1 - ang1_old) / sdt;

	ang = ang1; v_ang = v_ang1;
}
void Body::follow_preset_v_ang(Cur& cur) {
	if (!v_ang_prog) { return; }
	Scope sc; bool ret = false;
	sc[L"t"] = msh<Var>(cur.t);
	auto v_ang1 = Execute(ret, gl, sc, *v_ang_prog)->num;

	v_ang = v_ang1;
}
void Body::update_ang_drag(Cur& cur) {
	v_ang_drag = 0;
	if (!kb) {
		if (kbd[L'Q']) { v_ang_drag = -cur.v_ang_drag; }
		if (kbd[L'E']) { v_ang_drag = +cur.v_ang_drag; }
	}
	ang_drag = ang + v_ang_drag * cur.real_dt;
}
void Body::hdl_dragged_point(Cur& cur) {
	vec2 p1 = vec2(msp);
	vec2 p0 = o + tsf * p_drag_rel;
	vec2 v1 = vec2(msp - msp_old) / cur.real_dt;
	vec2 d = (p1 - p0).unit();

	vec2 r0 = p0 - o;
	r0 = vec2(-r0.y, r0.x);
	vec2 v0 = v + v_ang * r0;
	double rd0 = dot(r0, d);
	double inv_i0 = inv_m + rd0 * rd0 * inv_i;
	vec2 j = dot(v1 - v0, d) * d / inv_i0;

	v += j * inv_m;
	v_ang += dot(j, r0) * inv_i;
	o += p1 - p0;
}
void Body::hdl_dragged_whole(Cur& cur) {
	vec2 p1 = vec2(msp);
	vec2 p0 = o + tsf * p_drag_rel;
	vec2 v1 = vec2(msp - msp_old) / cur.real_dt;

	v = v1;
	ang = ang_drag;
	v_ang = v_ang_drag;
	o += p1 - p0;
}
void Body::hdl_dragged_force(Cur& cur, double sdt) {
	vec2 p1 = vec2(msp);
	vec2 p0 = o + tsf * p_drag_rel;
	vec2 d = (p1 - p0).unit();

	vec2 r0 = p0 - o;
	r0 = vec2(-r0.y, r0.x);
	double hooke = 1e6;
	vec2 j = hooke * (p1 - p0).len() * sdt * d;

	v += j * inv_m;
	v_ang += dot(j, r0) * inv_i;
}
void Body::Update(Cur& cur) {
	hovered = (hvd == this);

	if (cur.mode == MODE_DEL && hovered && msc(0)) { del = true; }
	if (cur.mode == MODE_SEL && hovered && msc(0)) { cur.body_sel = this; }
	if (dragged) {
		update_ang_drag(cur);
		if (msd[2]) { hdl_dragged_whole(cur); }
		else if (!kb && kbd[L'F']) {}
		else if (inv_m == 0) { hdl_dragged_whole(cur); }
		else { hdl_dragged_point(cur); }
		dragged = msd[0] && (cur.mode == MODE_DRG);
	} else {
		dragged = hovered && msc(0) && (cur.mode == MODE_DRG);
		if (dragged) {
			p_drag_rel = tsf.inv() * ((vec2)msp - o);
			// 下面这个还是很重要的。
			update_ang_drag(cur);
		}
	}

	if (this == cur.body_sel && cur.show_track && !cur.paused) {
		track.push_back(o);
	}
	if (this != cur.body_sel || !cur.show_track) {
		track.clear();
	}


	bool crt_ok = hovered && cur.mode == MODE_CRT &&
		(cur.crt->mode == CRT_CONN || cur.crt->mode == CRT_NAIL);
	if (crt_ok) {
		if (msc(0) && !cur.crt->active) {
			cur.crt->b0 = this;
			cur.crt->p0_rel = tsf.inv() * ((vec2)msp - o);
		}
		else if (!msd[0] && msd_old[0] && cur.crt->active) {
			cur.crt->b1 = this;
			cur.crt->p1_rel = tsf.inv() * ((vec2)msp - o);
		}
	}
}
void Body::PreUpdate(Cur& cur) {
	bool ok = dhv <= dep() &&
		inside((vec2)msp) && insd(msp, bgr.vp());
	if (ok) { dhv = dep(); hvd = this; }
}

void Electrostatic(Body& b0, Body& b1, double sdt, double coulomb) {
	double dsqr = (b1.o - b0.o).lensqr();
	vec2 d = (b1.o - b0.o).unit();
	vec2 je = -b0.charge * b1.charge * d / dsqr * sdt * coulomb;
	b0.v += je * b0.inv_m;
	b1.v -= je * b1.inv_m;
}
