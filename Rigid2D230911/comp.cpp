#include "comp.h"
#include "cur.h"
#include "var.h"
#include "bgr.h"
#include "body.h"
#include "shape.h"
#include "connection.h"

#include "my_def.h"
#define bs (cur.bs)
#define found(s) (dic.find(s) != dic.end())
#define getv(nm) if (found(L#nm)) { nm = dic.at(L#nm)->num; }

ptr<Var> Ball(Var const& r) {
	vector<ptr<Var>> vs;
	vector<ptr<Var>> v;
	// 应该有个快捷创建 Var 的方法才好。
	v.push_back(msh<Var>(1));
	v.push_back(msh<Var>(r));
	vs.push_back(msh<Var>(v));
	return msh<Var>(vs);
}
ptr<Var> Box(Var const& w, Var const& h) {
	vector<ptr<Var>> vs;
	vector<ptr<Var>> v;
	// 应该有个快捷创建 Var 的方法才好。
	v.push_back(msh<Var>(0));
	vector<ptr<Var>> tmp;
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(+w.num / 2), msh<Var>(+h.num / 2) }));
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(-w.num / 2), msh<Var>(+h.num / 2) }));
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(-w.num / 2), msh<Var>(-h.num / 2) }));
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(+w.num / 2), msh<Var>(-h.num / 2) }));
	v.push_back(msh<Var>(tmp));
	vs.push_back(msh<Var>(v));
	return msh<Var>(vs);
}
ptr<Var> RegularPoly(Var const& n, Var const& r) {
	vector<ptr<Var>> vs;
	vector<ptr<Var>> v;
	v.push_back(msh<Var>(0));
	vector<ptr<Var>> tmp;

	auto m = max(3, (int)n.num);
	rep(i, 0, m) {
		double phi = 2 * PI * i / m;
		tmp.push_back(msh<Var>(
			vector<ptr<Var>>{ 
			msh<Var>(r.num * cos(phi)), 
				msh<Var>(r.num * sin(phi)) }));
	} 
	v.push_back(msh<Var>(tmp));
	vs.push_back(msh<Var>(v));
	return msh<Var>(vs);
}
ptr<Var> Parallelogram(Var const& w, Var const& h, Var const& s) {
	vector<ptr<Var>> vs;
	vector<ptr<Var>> v;
	v.push_back(msh<Var>(0));
	vector<ptr<Var>> tmp;
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(+w.num / 2 + s.num), msh<Var>(+h.num / 2) }));
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(-w.num / 2 + s.num), msh<Var>(+h.num / 2) }));
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(-w.num / 2 - s.num), msh<Var>(-h.num / 2) }));
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(+w.num / 2 - s.num), msh<Var>(-h.num / 2) }));
	v.push_back(msh<Var>(tmp));
	vs.push_back(msh<Var>(v));
	return msh<Var>(vs);
}
ptr<Var> Trapezoid(Var const& t, Var const& b, Var const& h) {
	vector<ptr<Var>> vs;
	vector<ptr<Var>> v;
	v.push_back(msh<Var>(0));
	vector<ptr<Var>> tmp;
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(+t.num / 2), msh<Var>(+h.num / 2) }));
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(-t.num / 2), msh<Var>(+h.num / 2) }));
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(-b.num / 2), msh<Var>(-h.num / 2) }));
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(+b.num / 2), msh<Var>(-h.num / 2) }));
	v.push_back(msh<Var>(tmp));
	vs.push_back(msh<Var>(v));
	return msh<Var>(vs);
}
ptr<Var> Cross(Var const& s, Var const& thk) {
	vector<ptr<Var>> vs;
	vector<ptr<Var>> v;
	v.push_back(msh<Var>(0));
	vector<ptr<Var>> tmp;

	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(+s.num / 2), msh<Var>(+thk.num / 2) }));
	tmp.push_back(msh<Var>(								  
		vector<ptr<Var>>{ msh<Var>(-s.num / 2), msh<Var>(+thk.num / 2) }));
	tmp.push_back(msh<Var>(								  
		vector<ptr<Var>>{ msh<Var>(-s.num / 2), msh<Var>(-thk.num / 2) }));
	tmp.push_back(msh<Var>(								  
		vector<ptr<Var>>{ msh<Var>(+s.num / 2), msh<Var>(-thk.num / 2) }));
	v.push_back(msh<Var>(tmp));
	vs.push_back(msh<Var>(v));

	v.clear();
	v.push_back(msh<Var>(0));
	tmp.clear();
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(+thk.num / 2), msh<Var>(+s.num / 2) }));
	tmp.push_back(msh<Var>(									
		vector<ptr<Var>>{ msh<Var>(-thk.num / 2), msh<Var>(+s.num / 2) }));
	tmp.push_back(msh<Var>(									
		vector<ptr<Var>>{ msh<Var>(-thk.num / 2), msh<Var>(-s.num / 2) }));
	tmp.push_back(msh<Var>(									
		vector<ptr<Var>>{ msh<Var>(+thk.num / 2), msh<Var>(-s.num / 2) }));
	v.push_back(msh<Var>(tmp));
	vs.push_back(msh<Var>(v));
	
	return msh<Var>(vs);
}
ptr<Var> FramedBox(Var const& w, Var const& h, Var const& thk) {
		double hw = 60, hh = 5;
		vec2 a0(+w.num / 2 + thk.num / 2, +h.num / 2 + thk.num / 2);
		vec2 a1(-w.num / 2 - thk.num / 2, +h.num / 2 + thk.num / 2);
		vec2 a2(-w.num / 2 - thk.num / 2, +h.num / 2 - thk.num / 2);
		vec2 a3(+w.num / 2 + thk.num / 2, +h.num / 2 - thk.num / 2);
		vec2 b0(+w.num / 2 + thk.num / 2, -h.num / 2 + thk.num / 2);
		vec2 b1(-w.num / 2 - thk.num / 2, -h.num / 2 + thk.num / 2);
		vec2 b2(-w.num / 2 - thk.num / 2, -h.num / 2 - thk.num / 2);
		vec2 b3(+w.num / 2 + thk.num / 2, -h.num / 2 - thk.num / 2);
		vec2 c0(+w.num / 2 + thk.num / 2, +h.num / 2 + thk.num / 2);
		vec2 c1(+w.num / 2 + thk.num / 2, -h.num / 2 - thk.num / 2);
		vec2 c2(+w.num / 2 - thk.num / 2, -h.num / 2 - thk.num / 2);
		vec2 c3(+w.num / 2 - thk.num / 2, +h.num / 2 + thk.num / 2);
		vec2 d0(-w.num / 2 + thk.num / 2, +h.num / 2 + thk.num / 2);
		vec2 d1(-w.num / 2 + thk.num / 2, -h.num / 2 - thk.num / 2);
		vec2 d2(-w.num / 2 - thk.num / 2, -h.num / 2 - thk.num / 2);
		vec2 d3(-w.num / 2 - thk.num / 2, +h.num / 2 + thk.num / 2);

	vector<ptr<Shape>> shapes;
	shapes.push_back(msh<Shape>(vector<vec2>{ a0, a1, a2, a3 }));
	shapes.push_back(msh<Shape>(vector<vec2>{ b0, b1, b2, b3 }));
	shapes.push_back(msh<Shape>(vector<vec2>{ c3, c2, c1, c0 }));
	shapes.push_back(msh<Shape>(vector<vec2>{ d3, d2, d1, d0 }));

	vector<ptr<Var>> vs;
	vector<ptr<Var>> v;
	v.push_back(msh<Var>(0));
	vector<ptr<Var>> tmp;

	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(a0.x), msh<Var>(a0.y) }));
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(a1.x), msh<Var>(a1.y) }));
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(a2.x), msh<Var>(a2.y) }));
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(a3.x), msh<Var>(a3.y) }));
	v.push_back(msh<Var>(tmp));
	vs.push_back(msh<Var>(v));

	v.clear();
	v.push_back(msh<Var>(0));
	tmp.clear();
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(b0.x), msh<Var>(b0.y) }));
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(b1.x), msh<Var>(b1.y) }));
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(b2.x), msh<Var>(b2.y) }));
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(b3.x), msh<Var>(b3.y) }));
	v.push_back(msh<Var>(tmp));
	vs.push_back(msh<Var>(v));

	v.clear();
	v.push_back(msh<Var>(0));
	tmp.clear();
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(c3.x), msh<Var>(c3.y) }));
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(c2.x), msh<Var>(c2.y) }));
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(c1.x), msh<Var>(c1.y) }));
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(c0.x), msh<Var>(c0.y) }));
	v.push_back(msh<Var>(tmp));
	vs.push_back(msh<Var>(v));

	v.clear();
	v.push_back(msh<Var>(0));
	tmp.clear();
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(d3.x), msh<Var>(d3.y) }));
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(d2.x), msh<Var>(d2.y) }));
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(d1.x), msh<Var>(d1.y) }));
	tmp.push_back(msh<Var>(
		vector<ptr<Var>>{ msh<Var>(d0.x), msh<Var>(d0.y) }));
	v.push_back(msh<Var>(tmp));
	vs.push_back(msh<Var>(v));

	return msh<Var>(vs);
}

void CreateBody(Cur& cur, Var const& shs_cfg, Var const& cfg) {
	bool point = false;
	auto dic = cfg.dic;
	// point 变量在 Body 之后不能变动，所以要特殊考虑。
	getv(point);
	ptr<Body> bo;
	vector<ptr<Shape>> shs;
	// 把这些抽象成 vec 的一个方法我觉得会更好。
	for (auto v : shs_cfg.vec) {
		if (v->vec.empty()) { continue; }
		ptr<Shape> sh;
		if (v->vec[0]->num) {
			if (v->vec.size() < 2) { continue; }
			vec2 o_rel;
			if (v->vec.size() >= 3) { o_rel = tv2(*v->vec[2]); }
			mkp(sh)(v->vec[1]->num, o_rel);
		} else {
			if (v->vec.size() < 2) { continue; }
			vector<vec2> vs;
			for (auto w : v->vec[1]->vec) {
				vs.push_back(tv2(*w));
			}
			mkp(sh)(vs);
		}
		shs.push_back(sh);
	}
	if (point) { mkp(bo)(vec2()); }
	else { mkp(bo)(shs); }

	bo->c_inner = dcol(drnd(256), drnd(256), drnd(256));
	bo->read_cfg(cfg); 
	bool repos_o = false;
	getv(repos_o);
	bo->Init(repos_o);
	bs.push_back(bo);
	cur.scene_changed = true;
}
void CreateConn(Cur& cur, Var const& cfg) {
	int idx0 = drnd(bs.size());
	int idx1 = drnd(bs.size());
	auto dic = cfg.dic;
	getv(idx0); getv(idx1);
	idx0 = clmp(idx0, 0, (int)bs.size());
	idx1 = clmp(idx1, 0, (int)bs.size());
	
	ptr<Connection> con;
	mkp(con)();
	con->b0 = &*bs[idx0];
	con->b1 = &*bs[idx1];
	con->p0_rel = con->b0->rnd_rel();
	con->p1_rel = con->b1->rnd_rel();
	bool absolute = false;
	getv(absolute);
	if (found(L"p0")) {
		con->p0_rel = tv2(*dic[L"p0"]);
		if (absolute) { 
			con->p0_rel -= con->b0->o; 
			con->p0_rel = con->b0->tsf.inv() * con->p0_rel;
		}
	}
	if (found(L"p1")) {
		con->p1_rel = tv2(*dic[L"p1"]);
		if (absolute) {
			con->p1_rel -= con->b1->o; 
			con->p1_rel = con->b1->tsf.inv() * con->p1_rel;
		}
	}
	con->sign_up();
	con->generate();
	con->len = (con->p0 - con->p1).len();
	con->read_cfg(cfg);
	cur.cons.push_back(con);
	cur.scene_changed = true;
}

void Boundary(Cur& cur, Var const& thk, Var const &cfg) {
	double bw = bgr.w, bh = bgr.h;
	double bx0 = bgr.tl.x, bx1 = bx0 + bw;
	double by0 = bgr.tl.y, by1 = by0 + bh;
	ptr<Body> bo;
	dcol col = dcol(drnd(256), drnd(256), drnd(256));

	bo = msh<Body>(msh<Shape>(rect(thk.num, bh)));
	bo->o = vec2(bx0, (by0 + by1) / 2);
	bo->fixed = true;
	bo->c_inner = col;
	bo->read_cfg(cfg);
	bo->Init();
	bs.push_back(bo);

	bo = msh<Body>(msh<Shape>(rect(thk.num, bh)));
	bo->o = vec2(bx1, (by0 + by1) / 2);
	bo->fixed = true;
	bo->c_inner = col;
	bo->read_cfg(cfg);
	bo->Init();
	bs.push_back(bo);

	bo = msh<Body>(msh<Shape>(rect(bw, thk.num)));
	bo->o = vec2((bx0 + bx1) / 2, by0);
	bo->fixed = true;
	bo->c_inner = col;
	bo->read_cfg(cfg);
	bo->Init();
	bs.push_back(bo);

	bo = msh<Body>(msh<Shape>(rect(bw, thk.num)));
	bo->o = vec2((bx0 + bx1) / 2, by1);
	bo->fixed = true;
	bo->c_inner = col;
	bo->read_cfg(cfg);
	bo->Init();
	bs.push_back(bo);
	cur.scene_changed = true;
}
void Gear(Cur& cur, Var const& r, Var const& n, Var const& h, Var const& cfg) {
	vector<ptr<Shape>> shs;
	shs.push_back(msh<Shape>(r.num));
	rep(i, 0, n.num) {
		double phi0 = 2 * PI * i / (int)n.num;
		double phi1 = 2 * PI * (i + 1) / (int)n.num;
		double phi = (phi0 + phi1) / 2;
		vec2 v = vec2(cos(phi), sin(phi)) * (r.num + h.num);
		vec2 v0 = vec2(cos(phi0), sin(phi0)) * r.num;
		vec2 v1 = vec2(cos(phi1), sin(phi1)) * r.num;
		shs.push_back(msh<Shape>(vector<vec2>{ v0, v, v1 }));
	}
	auto wheel = msh<Body>(shs);
	wheel->c_inner = dcol(drnd(256), drnd(256), drnd(256));
	wheel->read_cfg(cfg);
	wheel->Init();
	bs.push_back(wheel);
	auto root = msh<Body>(wheel->o);
	root->c_inner = wheel->c_inner;
	root->Init();
	bs.push_back(root);

	auto con = msh<Connection>();
	con->type = CON_LINK;
	con->b0 = &*root;
	con->b1 = &*wheel;
	con->sign_up();
	con->generate();
	cur.cons.push_back(con);
	cur.scene_changed = true;
}
void Strand(Cur& cur, Var const& _p0, Var const& _p1,
	Var const& _n, Var const& ratio, Var const& cfg_body, Var const& cfg_conn) {
	
	vec2 p0 = tv2(_p0), p1 = tv2(_p1);
	int n = _n.num;
	ptr<Body> b0, b1;
	ptr<Connection> con;
	double len = (p1 - p0).len() / n;
	double rad = 0.5 * ratio.num * len;
	double gap = max(0.0, len - 2 * rad);
	dcol col = dcol(drnd(256), drnd(256), drnd(256));

	mkp(b0)(msh<Shape>(rad));
	b0->c_inner = col;
	b0->o = p0;
	b0->read_cfg(cfg_body);
	b0->Init();
	bs.push_back(b0);

	rep(i, 1, n + 1) {
		mkp(b1)(msh<Shape>(rad));
		b1->c_inner = col;
		b1->o = (p0 * (n - i) + p1 * i) / n;
		b1->read_cfg(cfg_body);
		b1->Init();
		bs.push_back(b1);

		mkp(con)();
		con->b0 = &*b0;
		con->b1 = &*b1;
		con->len = gap;
		con->p0_rel = +rad * (p1 - p0).unit();
		con->p1_rel = -rad * (p1 - p0).unit();
		con->read_cfg(cfg_conn);
		con->sign_up();
		con->generate();
		cur.cons.push_back(con);

		b0 = b1;
	}
	cur.scene_changed = true;
}
void Necklace(Cur& cur, Var const& _o, Var const& rad,
	Var const& _n, Var const& ratio, Var const& cfg_body, Var const& cfg_conn) {

	ptr<Body> beg, b0, b1;
	int n = _n.num;
	vec2 o = tv2(_o);
	ptr<Connection> con;
	double len = 2 * PI * rad.num / n;
	double ball_rad = 0.5 * ratio.num * len;
	double gap = max(0.0, len - 2 * ball_rad);
	dcol col = dcol(drnd(256), drnd(256), drnd(256));

	mkp(b0)(msh<Shape>(ball_rad));
	b0->c_inner = col;
	b0->o = o + vec2(rad.num, 0);
	b0->read_cfg(cfg_body);
	b0->Init();
	bs.push_back(b0);
	beg = b0;

	rep(i, 1, n) {
		double phi = 2 * PI * i / n;
		mkp(b1)(msh<Shape>(ball_rad));
		b1->c_inner = col;
		b1->o = o + vec2(cos(phi), sin(phi)) * rad.num;
		b1->read_cfg(cfg_body);
		b1->Init();
		bs.push_back(b1);

		mkp(con)();
		con->b0 = &*b0;
		con->b1 = &*b1;
		con->len = gap;
		con->p1_rel = ball_rad * vec2(+sin(phi), -cos(phi));
		phi = 2 * PI * (i - 1) / n;
		con->p0_rel = ball_rad * vec2(-sin(phi), +cos(phi));
		con->read_cfg(cfg_conn);
		con->sign_up();
		con->generate();
		cur.cons.push_back(con);

		b0 = b1;
	}
	double phi = 2 * PI * (n) / n;
	b1 = beg;
	mkp(con)();
	con->b0 = &*b0;
	con->b1 = &*b1;
	con->len = gap;
	con->p1_rel = ball_rad * vec2(+sin(phi), -cos(phi));
	phi = 2 * PI * (n - 1) / n;
	con->p0_rel = ball_rad * vec2(-sin(phi), +cos(phi));
	con->read_cfg(cfg_conn);
	con->sign_up();
	con->generate();
	cur.cons.push_back(con);
	cur.scene_changed = true;
}
