#pragma once
#include "vec2.h"

struct Var;
struct Cur;
struct Body;
struct Connection;

ptr<Var> Ball(Var const& r);
ptr<Var> Box(Var const& w, Var const &h);
ptr<Var> RegularPoly(Var const& n, Var const &r);
ptr<Var> Parallelogram(Var const& w, Var const& h, Var const &s);
ptr<Var> Trapezoid(Var const& t, Var const& b, Var const &h);
ptr<Var> Cross(Var const& s, Var const& thk);
ptr<Var> FramedBox(Var const& w, Var const& h, Var const& thk);

void CreateBody(Cur& cur, Var const& shs_cfg, Var const& cfg);
void CreateConn(Cur& cur, Var const& cfg);
void Boundary(Cur& cur, Var const& thk, Var const &cfg);
void Gear(Cur& cur, Var const& r, Var const& n, Var const& h, Var const& cfg);
void Strand(Cur& cur, Var const& _p0, Var const& _p1, 
	Var const& _n, Var const& ratio, Var const& cfg_body, Var const& cfg_conn);
void Necklace(Cur& cur, Var const& _o, Var const& rad,
	Var const& _n, Var const& ratio, Var const& cfg_body, Var const& cfg_conn);
