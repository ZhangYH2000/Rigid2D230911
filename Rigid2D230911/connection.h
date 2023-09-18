#pragma once
#include "tile.h"

int constexpr CON_ROPE = 0;
int constexpr CON_LINK = 1;
int constexpr CON_SPRG = 3;
int constexpr CON_CORD = 4;

struct Var;
struct Cur;
struct Body;
struct Connection {
	bool del = false;
	int type = 0;
	wstring cmd, tmp_cmd;
	Body* b0 = NULL;
	Body* b1 = NULL;
	vec2 p0_rel, p1_rel;
	vec2 p0, p1;
	double len = 0;
	double hooke = 0;
	bool tight = false;
	dcol c_rope_tight, c_rope_normal;
	dcol c_link;
	dcol c_cord_tight, c_cord_normal;
	dcol c_spring_tight, c_spring_loose;

	Connection();
	void save(Cur const& cur, FILE* f) const;
	Connection(Cur& cur, FILE* f);

	void read_cfg(Var const& cfg);
	void sign_up();
	void generate();
	void refresh(Cur& cur);
	void resolve_rope_link(bool equal_repos);
	void resolve_cord_spring(double sdt);
	void Resolve(double sdt, bool equal_repos);
	void Render(Cur& cur) const;
};

Body* the_other(Body* b, Connection const& con);
