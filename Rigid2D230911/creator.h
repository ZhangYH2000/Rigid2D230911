#pragma once
#include "vec2.h"

int constexpr CRT_BOX = 0;
int constexpr CRT_BALL = 1;
int constexpr CRT_PLATE = 2;
int constexpr CRT_PARTICLE = 3;
int constexpr CRT_CONN = 4;
int constexpr CRT_POINT = 5;
int constexpr CRT_NAIL = 6;

struct Var;
struct Cur;
struct Body;
struct Connection;
struct Creator {
	int mode = 0;
	double dep_body = 0;
	double dep_con = 0;
	double thick = 0;
	double rad = 0;
	dvec p_rec;
	Body* b0 = NULL;
	Body* b1 = NULL;
	ptr<Var> cfg_body, cfg_conn;
	vec2 p0_rel, p1_rel;

	ptr<Body> bo;
	ptr<Connection> con;
	bool active = false;
	bool hovered = false;

	Creator();
	Creator(Var const& v);
	double dep() const;
	void update_box(Cur& cur);
	void update_ball(Cur& cur);
	void update_plate(Cur& cur);
	void update_con(Cur& cur);
	void update_point(Cur& cur);
	void update_nail(Cur& cur);
	void update_particle(Cur& cur);
	void Update(Cur& cur);
	void Discard(Cur& cur);
	void PreUpdate(Cur& cur);
};
