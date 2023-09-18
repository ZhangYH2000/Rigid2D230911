#pragma once
#include "font.h"
#include "app_wv.h"
#include "app_item.h"

int constexpr LAN_CHI = 0;
int constexpr LAN_ENG = 1;

LRESULT CALLBACK wnd_proc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);
struct App {
	int w = 0, h = 0;
	tile scr;
	dbuf dscr, ini_dscr;

	font ft;
	int lan = 0;
	map<wstring, vector<wstring>> dict;

	app_wv wv;
	app_fps fps;
	app_input in;
	app_owners own;

	HDC hdc = NULL;
	HWND hwnd = NULL;
	BITMAPINFO bmi{};
	HCURSOR sys_hcs{};
	HINSTANCE hinst = NULL;

	wstring title;
	bool use_sys_cs = true;
	wstring class_name;

	App();
	void Init();
	virtual void Update() {}
	void Run(bool show_console = false);

	void reset_dscr();
	void present_scr();
	void addlog(wstring const& s);
	wstring loc(wstring const& id) const;

	void init_bmi();
	void load_localization();
	void register_window_class();
	void create_window();
};
