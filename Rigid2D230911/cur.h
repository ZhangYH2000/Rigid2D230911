#pragma once
#include "app.h"
#include "var.h"

int constexpr MODE_DRG = 0;
int constexpr MODE_SEL = 1;
int constexpr MODE_CRT = 2;
int constexpr MODE_DEL = 3;

int constexpr DISP_COL = 0;
int constexpr DISP_ENERGY = 1;
int constexpr DISP_CHARGE = 2;
int constexpr DISP_NONE = 3;


struct Body;
struct Group;
struct Creator;
struct Collision;
struct Connection;

struct UI;
struct Bgr;
struct clip;
struct param;
struct Cur : App {
	ptr<UI> ui;
	ptr<Bgr> bgr;

	Scope gl;
	wstring dbstr, cmd, tmp_cmd;
	vector<ptr<param>> pars;

	ptr<clip> cl0, cl1;
	clip* cl = NULL;
	double vol = 0;

	int mode = 0;
	int display = 0;
	double energy_mul = 0;
	double charge_mul = 0;
	bool show_track = false;
	bool mute = false;
	bool paused = false;
	bool scene_changed = false;
	vec2 gravity;
	double coulomb = 0;
	bool electrostatic = false;
	double max_real_dt = 0;
	double real_dt = 0, t = 0;
	int n_step = 0;
	drect rect_scene;
	ptr<Creator> crt;
	Body *body_sel = NULL;
	Connection* con_sel = NULL;
	vector<ptr<Body>> bs;
	vector<ptr<Group>> grps;
	vector<ptr<Collision>> cols;
	vector<ptr<Connection>> cons;

	double eps_paralell = 0;
	bool equal_repos = false;
	double v_ang_drag = 0;

	double s_grid = 0;
	int nx_grid = 0, ny_grid = 0;
	vector<Body*> out_grid;
	vector<vector<Body*>> grid;

	Cur();
	void Save(wstring const& nm) const;
	void Load(wstring const& nm);
	void Reset();
	void Update() override;

	void set_cfg(Var const& v);
	void CollideBodies();

	void save_par(FILE* f) const;
	void load_par(FILE* f);
	void init_def_fun();
	void basic_update();
};
