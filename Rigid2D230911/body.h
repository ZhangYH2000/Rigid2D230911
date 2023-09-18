#pragma once
#include "tile.h"
#include "aabb.h"

struct proc;
typedef vector<proc> Program;

struct Var;
struct Cur;
struct Shape;
struct Connection;
struct Body {
	bool del = false;
	bool point = false;
	bool fixed = false;
	ptr<Program> o_prog, ang_prog, v_ang_prog;
	double inertia = 0;
	bool preset_o = false;
	bool preset_ang = false;
	bool preset_v_ang = false;
	vector<vec2> track;
	vector<ptr<Shape>> shs;
	vector<Connection*> cons;
	vec2 o, v;
	aabb box;
	mat2 tsf = mat2::id();
	wstring cmd, tmp_cmd;
	double ang = 0, v_ang = 0;
	double inv_m = 0, inv_i = 0;
	double area = 0, density = 0;
	double e = 0, mu_s = 0, mu_d = 0;
	double damp_v = 0, damp_v_ang = 0;
	double charge_density = 0, charge = 0;

	bool visited = false;
	bool hovered = false;
	bool dragged = false;
	double ang_drag = 0, v_ang_drag = 0;
	vec2 p_drag_rel;
	double dep_shape = 0, dep_point = 0;
	double r_point = 0;
	dcol c_inner, c_border, c_hovered, c_dragged, c_selected, c_selected_con;

	Body();
	Body(vec2 const& _o);
	Body(ptr<Shape> sh);
	Body(vector<ptr<Shape>> const& _shs);
	void save(FILE* f) const;
	Body(Cur& cur, FILE* f);
	
	double dep() const;
	vec2 rnd_rel() const;
	bool inside(vec2 p) const;
	void refresh(Cur &cur);
	void read_cfg(Var const& cfg);
	void generate();
	void update_box();
	void warp(drect rc);
	void register_grid(Cur& cur);

	void Init(bool repos_o = false);
	void Render(Cur& cur) const;
	void Step(Cur &cur, double sdt);
	void follow_preset_o(Cur& cur, double sdt);
	void follow_preset_ang(Cur& cur, double sdt);
	void follow_preset_v_ang(Cur& cur);
	void update_ang_drag(Cur& cur);
	void hdl_dragged_point(Cur& cur);
	void hdl_dragged_whole(Cur& cur);
	void hdl_dragged_force(Cur& cur, double sdt);
	void Update(Cur& cur);
	void PreUpdate(Cur& cur);
};

void Electrostatic(Body& b0, Body& b1, double sdt, double coulomb);
