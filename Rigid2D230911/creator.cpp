#include "creator.h"
#include "cur.h"
#include "bgr.h"
#include "var.h"
#include "body.h"
#include "shape.h"
#include "connection.h"

#include "my_def.h"
#define found(s) (dic.find(s) != dic.end())
#define getv(nm) if (found(L#nm)) { nm = dic.at(L#nm)->num; }

Creator::Creator() {
	dep_body = 10;
	dep_con = -1;
	thick = 10;
	rad = 8;
}
Creator::Creator(Var const& v) : Creator() {
	auto dic = v.dic;
	if (found(L"mode")) {
		auto md = dic.at(L"mode");
		if (md->typ == L"str") {
			auto s = md->str;
			if (s == L"box") { mode = CRT_BOX; }
			else if (s == L"ball") { mode = CRT_BALL; }
			else if (s == L"plate") { mode = CRT_PLATE; }
			else if (s == L"conn") { mode = CRT_CONN; }
			else if (s == L"point") { mode = CRT_POINT; }
			else if (s == L"nail") { mode = CRT_NAIL; }
			else if (s == L"particle") { mode = CRT_PARTICLE; }
		} 
		else { mode = md->num; }
	}
	getv(thick); getv(rad);
	if (found(L"cfg_body")) { mkp(cfg_body)(*dic[L"cfg_body"]); }
	if (found(L"cfg_conn")) { mkp(cfg_conn)(*dic[L"cfg_conn"]); }
}

double Creator::dep() const {
	switch (mode) {
	case CRT_CONN: case CRT_NAIL: return dep_con;
	} return dep_body;
}

void Creator::update_box(Cur& cur) {
	if (active) {
		double x0 = p_rec.x, y0 = p_rec.y;
		double x1 = msp.x, y1 = msp.y;
		double min_wh = 10;
		if (x1 - x0 >= min_wh && y1 - y0 >= min_wh) {
			mkp(bo)(msh<Shape>(rect(x1 - x0, y1 - y0)));
			bo->o = vec2(x0 + x1, y0 + y1) / 2;
			bo->c_inner = dcol(drnd(256), drnd(256), drnd(256));
			if (cfg_body) { bo->read_cfg(*cfg_body); }
			bo->Init();
		}

		if (!msd[0]) {
			active = false;
			if (bo) { 
				cur.bs.push_back(bo); 
				bo = NULL; cur.scene_changed = true;
			}
		}
	} else {
		if (hovered && msc(0)) {
			active = true; p_rec = msp;
		}
	}
}
void Creator::update_ball(Cur& cur) {
	if (active) {
		double r = sqrt((p_rec - msp).lensqr());
		double min_r = 7.5;
		if (r > min_r) {
			mkp(bo)(msh<Shape>(r));
			bo->o = vec2(p_rec);
			bo->c_inner = dcol(drnd(256), drnd(256), drnd(256));
			if (cfg_body) { bo->read_cfg(*cfg_body); }
			bo->Init();
		}

		if (!msd[0]) {
			active = false;
			if (bo) { 
				cur.bs.push_back(bo); 
				bo = NULL; cur.scene_changed = true;
			}
		}
	} else {
		if (hovered && msc(0)) {
			active = true; p_rec = msp;
		}
	}
}
void Creator::update_plate(Cur& cur) {
	if (active) {
		double thk = thick;
		double min_len = 5;
		dvec v = p_rec - msp;
		double len = sqrt(v.lensqr());
		if (len > min_len) {
			mkp(bo)(msh<Shape>(rect(len + thk, thk)));
			bo->o = vec2(p_rec + msp) / 2;
			bo->ang = atan2(v.y, v.x);
			bo->c_inner = dcol(drnd(256), drnd(256), drnd(256));
			if (cfg_body) { bo->read_cfg(*cfg_body); }
			bo->Init();
		}

		if (!msd[0]) {
			active = false;
			if (bo) { 
				cur.bs.push_back(bo); 
				bo = NULL; cur.scene_changed = true;
			}
		}
	} else {
		if (hovered && msc(0)) {
			active = true; p_rec = msp;
		}
	}
}
void Creator::update_con(Cur& cur) {
	if (active) {
		mkp(con)();
		if (cfg_conn) { con->read_cfg(*cfg_conn); }
		// 要求 creator 的更新在 body 后面对吧，是不是耦合太高了。
		// 但也没啥办法是吧。
		if (!msd[0]) {
			if (b1) {
				con->b0 = b0;
				con->b1 = b1;
				con->p0_rel = p0_rel;
				con->p1_rel = p1_rel;
				
				con->sign_up();
				con->generate();
				con->len = (con->p0 - con->p1).len();
				cur.cons.push_back(con);
				cur.scene_changed = true;
			}
			con = NULL;
			b0 = b1 = NULL; active = false;
		}
	} else {
		if (b0) { active = true; }
	}
}
void Creator::update_point(Cur& cur) {
	if (hovered && msc(0)) {
		mkp(bo)((vec2)msp);
		bo->c_inner = dcol(drnd(256), drnd(256), drnd(256));
		if (cfg_body) { bo->read_cfg(*cfg_body); }
		bo->Init();
		cur.bs.push_back(bo);
		bo = NULL; cur.scene_changed = true;
	}
}	 
void Creator::update_nail(Cur& cur) {
	if (b0) {
		mkp(bo)(b0->o + b0->tsf * p0_rel);
		bo->Init();
		cur.bs.push_back(bo);
		mkp(con)();
		con->type = CON_LINK;
		con->b0 = b0;
		con->b1 = &*bo;
		con->p0_rel = p0_rel;
		con->sign_up();
		con->generate();
		cur.cons.push_back(con);

		b0 = NULL; con = NULL;
		bo = NULL; cur.scene_changed = true;
	}
}
void Creator::update_particle(Cur& cur) {
	if (hovered && msc(0)) {
		mkp(bo)(msh<Shape>(rad));
		bo->o = (vec2)msp;
		bo->c_inner = dcol(drnd(256), drnd(256), drnd(256));
		if (cfg_body) { bo->read_cfg(*cfg_body); }
		bo->Init();
		cur.bs.push_back(bo);
		bo = NULL; cur.scene_changed = true;
	}
}
void Creator::Update(Cur& cur) {
	hovered = (hvd == this);
	bo = NULL; con = NULL;

	switch (mode) {
	case CRT_BOX: update_box(cur); break;
	case CRT_BALL: update_ball(cur); break;
	case CRT_PLATE: update_plate(cur); break;
	case CRT_CONN: update_con(cur); break;
	case CRT_POINT: update_point(cur); break;
	case CRT_NAIL: update_nail(cur); break;
	case CRT_PARTICLE: update_particle(cur); break;
	}


	if (bo) { bo->Render(cur); }
	if (con && b0) {
		con->p0 = b0->o + b0->tsf * p0_rel;
		con->p1 = (vec2)msp;
		con->Render(cur);
	}
}
void Creator::Discard(Cur& cur) {
	rmv; b0 = b1 = NULL;
	bo = NULL; con = NULL;
	hovered = active = false;
}
void Creator::PreUpdate(Cur& cur) {
	bool ok = dhv <= dep() && insd(msp, bgr.vp());
	if (ok) { dhv = dep(); hvd = this; }
}
