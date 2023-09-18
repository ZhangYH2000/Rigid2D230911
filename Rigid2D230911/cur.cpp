#include "cur.h"
#include "ui.h"
#include "bgr.h"
#include "par.h"
#include "shape.h"
#include "body.h"
#include "comp.h"
#include "clip.h"
#include "group.h"
#include "creator.h"
#include "collision.h"
#include "connection.h"

Cur::Cur() {
	w = 1800; h = 900; set_locale(); wv.len_block = 4 * 5e-4;
	print_console(L"正在加载字体...");
	App::Init();
	print_console(L"字体加载完成.", true);
	print_console(L"正在加载控件...");
	mkp(ui)(*this); ui->Init(*this);
	mkp(bgr)(*this); Reset();
	print_console(L"控件加载完成.", true);
	print_console(L"正在加载音乐...");
	bool ok = false;
	mkp(cl0)(L"sound/bgm0.clip", &ok);
	if (!ok) { cl0 = NULL; }
	mkp(cl1)(L"sound/bgm1.clip", &ok);
	if (!ok) { cl1 = NULL; }
	print_console(L"音乐加载完成.", true);
	vol = 1;
	cl = frnd(1) < 0.5 ? &*cl0 : &*cl1;
}

#define cur (*this)
#include "my_def.h"
#define found(s) (dic.find(s) != dic.end())
#define getv(nm) if (found(L#nm)) { nm = dic.at(L#nm)->num; }

void Cur::Save(wstring const& nm) const {
	FILE* f = wfopen(nm, L"wb");
	if (!f) { return; }
	int sz = 0; fwtv(cmd); save_par(f);

	// 可能会保存一些即将被删除的元素，但是也无所谓。
	sz = bs.size(); fwt(sz); 
	for (auto b : bs) { b->save(f); }
	sz = cons.size(); fwt(sz);
	for (auto c : cons) { c->save(*this, f); }

	fclose(f);
}
void Cur::Load(wstring const& nm) {
	FILE* f = wfopen(nm, L"rb");
	if (!f) { return; } Reset();
	int sz = 0; frdv(cmd); 
	tmp_cmd = cmd; load_par(f);

	frd(sz);
	rep(i, 0, sz) { bs.push_back(msh<Body>(*this, f)); }
	frd(sz);
	rep(i, 0, sz) { cons.push_back(msh<Connection>(*this, f)); }
	scene_changed = true;

	Execute(gl, Compile(cmd)); fclose(f);
}
void Cur::Reset() {
	gl.clear(); init_def_fun(); pars.clear();

	bs.clear();
	grps.clear();
	cols.clear();
	cons.clear();
	dbstr.clear();

	gl[L"left_bgr"] = msh<Var>(bgr.tl.x);
	gl[L"top_bgr"] = msh<Var>(bgr.tl.y);
	gl[L"w_bgr"] = msh<Var>(bgr.w);
	gl[L"h_bgr"] = msh<Var>(bgr.h);

	body_sel = NULL;
	con_sel = NULL;
	scene_changed = false;
	gravity = vec2();
	max_real_dt = 0.05;
	t = 0; n_step = 60;
	rect_scene.tl = bgr.tl;
	rect_scene.w = bgr.w;
	rect_scene.h = bgr.h;
	s_grid = 50;
	energy_mul = 4e-6;
	charge_mul = 1;
	show_track = false;

	electrostatic = false;
	coulomb = 1e5;

	eps_paralell = 1e-3;
	equal_repos = false;
	v_ang_drag = 2;

	mkp(crt)();
}
void Cur::Update() {
	bgr.PreUpdate(*this);
	if (mode == MODE_CRT) { crt->PreUpdate(*this); }
	for (auto b : bs) { b->PreUpdate(*this); }
	ui.PreUpdate(*this);
	basic_update();

	if (gl.find(L"update") != gl.end()) {
		auto tmp = *gl[L"update"];
		Execute(gl, tmp.procs);
	}

	if (cl0 && cl1) {
		if (cl->csp >= cl->n()) {
			cl->csp = 0;
			cl = (cl == &*cl0) ? &*cl1 : &*cl0;
		}
		if (!mute) { cl->play(wv.wvin); }
	}

	if (!kb) {
		if (kbc(L' ')) { paused = !paused; }
	}
	
	for (auto b : bs) if (b->del) for (auto c : b->cons) { c->del = true; }
	if (body_sel && body_sel->del) { body_sel = NULL; }
	if (con_sel && con_sel->del) { con_sel = NULL; }
	for (auto c : cons) if (c->del) {
		auto& cons0 = c->b0->cons;
		cons0.erase(remove(cons0.begin(), cons0.end(), &*c), cons0.end());
		auto& cons1 = c->b1->cons;
		cons1.erase(remove(cons1.begin(), cons1.end(), &*c), cons1.end());
	}
	cons.erase(remove_if(cons.begin(), cons.end(), 
		[](ptr<Connection> c) { return c->del; }), cons.end());
	bs.erase(remove_if(bs.begin(), bs.end(),
		[](ptr<Body> b) { return b->del; }), bs.end());

	bgr.Update(*this);
	real_dt = min(max_real_dt, dt);
	gl[L"dt"] = msh<Var>(real_dt);
	nx_grid = ceil(rect_scene.w / s_grid);
	ny_grid = ceil(rect_scene.h / s_grid);
	if (scene_changed) { scene_changed = false; grps = FormGroups(bs); }
	for (auto b : bs) { b->Update(*this); }
	if (!paused && real_dt != 0) {
		double sdt = real_dt / n_step;

		rep(i, 0, n_step) {
			gl[L"t"] = msh<Var>(t);
			for (auto b : bs) { b->Step(*this, sdt); }
			// for (auto b : bs) { b->warp(rect_scene); }
			for (auto g : grps) { g->Warp(rect_scene); }

			if (electrostatic) rep(i, 0, bs.size() - 1) rep(j, i + 1, bs.size()) {
				Electrostatic(*bs[i], *bs[j], sdt, coulomb);
			}
				
			out_grid.clear();
			grid.clear();
			grid.resize(nx_grid * ny_grid);
			for (auto b : bs) { b->register_grid(*this); }

			cols.clear();
			CollideBodies();
			for (auto c : cols) { c->Resolve(equal_repos); }
			// for (auto c : cols) { c->Render(*this); }
			for (auto c : cons) { c->Resolve(sdt, equal_repos); }
			t += sdt;
		}
	}

	for (auto b : bs) { b->generate(); b->Render(*this); }
	for (auto c : cons) { c->generate(); c->Render(*this); }

	if (mode == MODE_CRT) { crt->Update(*this); }
	ui.Update(*this);
}

void Cur::set_cfg(Var const& v) {
	auto dic = v.dic;
	getv(vol);
	getv(energy_mul); getv(charge_mul);
	getv(eps_paralell); getv(max_real_dt);
	getv(electrostatic); getv(coulomb);
	getv(n_step); n_step = max(1, n_step);
	getv(s_grid);  getv(equal_repos); getv(t); getv(show_track);
	if (found(L"left_scene")) { rect_scene.tl.x = dic[L"left_scene"]->num; }
	if (found(L"top_scene")) { rect_scene.tl.y = dic[L"top_scene"]->num; }
	if (found(L"w_scene")) { rect_scene.w = dic[L"w_scene"]->num; }
	if (found(L"h_scene")) { rect_scene.h = dic[L"h_scene"]->num; }
	if (found(L"gravity")) { gravity = tv2(*dic[L"gravity"]); }
	
	scene_changed = true;
}

#define colpars cols, eps_paralell
void Cur::CollideBodies() {
	rep(i, 0, nx_grid) rep(j, 0, ny_grid) {
		auto& gp = grid[j * nx_grid + i];
		rep(k, 0, gp.size()) {
			auto& b0 = *gp[k];
			rep(m, k + 1, gp.size()) { Collide(b0, *gp[m], colpars); }
			for (auto pb1 : out_grid) { Collide(b0, *pb1, colpars); }

			if (i + 1 < nx_grid) {
				int a = i + 1, b = j;
				for (auto pb1 : grid[b * nx_grid + a]) { Collide(b0, *pb1, colpars); }
			}
			if (j + 1 >= ny_grid) { continue; }

			if (i - 1 >= 0) {
				int a = i - 1, b = j + 1;
				for (auto pb1 : grid[b * nx_grid + a]) { Collide(b0, *pb1, colpars); }
			}
			if (i + 1 < nx_grid) {
				int a = i + 1, b = j + 1;
				for (auto pb1 : grid[b * nx_grid + a]) { Collide(b0, *pb1, colpars); }
			}
			int a = i, b = j + 1;
			for (auto pb1 : grid[b * nx_grid + a]) { Collide(b0, *pb1, colpars); }
		}
	}

	rep(i, 0, out_grid.size() - 1) rep(j, i + 1, out_grid.size()) {
		Collide(*out_grid[i], *out_grid[j], colpars);
	}
}

void Cur::save_par(FILE* f) const {
	int sz = pars.size(); fwt(sz);
	for (auto p : pars) { p->save(f); }
}
void Cur::load_par(FILE* f) {
	int sz = 0; frd(sz);
	rep(i, 0, sz) { pars.push_back(msh<param>(f)); }
}
void Cur::init_def_fun() {
	auto f0 = [this](vector<ptr<Var>> const& in) {
		if (in.size() >= 1) { mkp(crt)(*in[0]); }
		return msh<Var>();
	};
	gl[L"set_creator"] = msh<Var>(f0);

	auto f1 = [this](vector<ptr<Var>> const& in) {
		if (in.size() >= 2) { CreateBody(*this, *in[0], *in[1]); }
		return msh<Var>();
	};
	gl[L"create_body"] = msh<Var>(f1);

	auto f2 = [this](vector<ptr<Var>> const& in) {
		if (in.size() >= 2) { return Box(*in[0], *in[1]); }
		return msh<Var>();
	};
	gl[L"box"] = msh<Var>(f2);

	auto f3 = [this](vector<ptr<Var>> const& in) {
		if (in.size() >= 1) { return Ball(*in[0]); }
		return msh<Var>();
	};
	gl[L"ball"] = msh<Var>(f3);

	auto f4 = [this](vector<ptr<Var>> const& in) {
		if (in.size() >= 2) { return Parallelogram(*in[0], *in[1], *in[2]); }
		return msh<Var>();
	};
	gl[L"parallelogram"] = msh<Var>(f4);

	auto f5 = [this](vector<ptr<Var>> const& in) {
		if (in.size() >= 3) { return Trapezoid(*in[0], *in[1], *in[2]); }
		return msh<Var>();
	};
	gl[L"trapezoid"] = msh<Var>(f5);

	auto f6 = [this](vector<ptr<Var>> const& in) {
		if (in.size() >= 2) { return RegularPoly(*in[0], *in[1]); }
		return msh<Var>();
	};
	gl[L"regular_poly"] = msh<Var>(f6);

	auto f7 = [this](vector<ptr<Var>> const& in) {
		if (in.size() >= 2) { return Cross(*in[0], *in[1]); }
		return msh<Var>();
	};
	gl[L"cross"] = msh<Var>(f7);

	auto f8 = [this](vector<ptr<Var>> const& in) {
		return msh<Var>(bs.size());
	};
	gl[L"n_bodies"] = msh<Var>(f8);

	auto f9 = [this](vector<ptr<Var>> const& in) {
		if (in.size() >= 1) { CreateConn(*this, *in[0]); }
		return msh<Var>();
	};
	gl[L"create_conn"] = msh<Var>(f9);

	auto f10 = [this](vector<ptr<Var>> const& in) {
		if (in.size() >= 1) { set_cfg(*in[0]); }
		return msh<Var>();
	};
	gl[L"set_cfg"] = msh<Var>(f10);

	auto f11 = [this](vector<ptr<Var>> const& in) {
		if (in.size() >= 2) { Boundary(*this, *in[0], *in[1]); }
		return msh<Var>();
	};
	gl[L"boundary"] = msh<Var>(f11);

	auto f12 = [this](vector<ptr<Var>> const& in) {
		if (in.size() >= 3) { return FramedBox(*in[0], *in[1], *in[2]); }
		return msh<Var>();
	};
	gl[L"framed_box"] = msh<Var>(f12);

	auto f13 = [this](vector<ptr<Var>> const& in) {
		if (in.size() >= 4) { Gear(*this, *in[0], *in[1], *in[2], *in[3]); }
		return msh<Var>();
	};
	gl[L"gear"] = msh<Var>(f13);

	auto f14 = [this](vector<ptr<Var>> const& in) {
		if (in.size() >= 6) { 
			Strand(*this, *in[0], *in[1], *in[2], *in[3], *in[4], *in[5]);
		}
		return msh<Var>();
	};
	gl[L"strand"] = msh<Var>(f14);

	auto f15 = [this](vector<ptr<Var>> const& in) {
		if (in.size() >= 6) {
			Necklace(*this, *in[0], *in[1], *in[2], *in[3], *in[4], *in[5]);
		}
		return msh<Var>();
	};
	gl[L"necklace"] = msh<Var>(f15);
}
void Cur::basic_update() {
	title = loc(L"title");
	if (gl.find(L"dbstr") != gl.end()) { dbstr = gl[L"dbstr"]->str; }
	if (gl.find(L"update") != gl.end()) {
		auto f = gl[L"update"];  Execute(gl, f->procs);
	}

	pars.erase(remove_if(pars.begin(), pars.end(),
		[](ptr<param> p) { return p->del; }), pars.end());
	for (auto& p : pars) { gl[p->nm] = msh<Var>(p->val); }
}
