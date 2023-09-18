#pragma once
#include "dvec.h"

struct app_input {
	double msw = 0;
	wstring chs;
	dvec msp, msp_old;
	array<BYTE, 3> msd{}, msd_old{};
	array<int, 256> kbmsg{};
	array<BYTE, 256> kbd{}, kbd_old{};
	// 比如说播音乐, 在窗口移动时可以使音乐暂停。
	bool bar_clicked = false;

	// 我觉得这个鼠标逻辑还是比较奇怪，可能有多余之处。
	// 以后要改一下，但请注意这里各种不合理的布置都是有理由的。
	bool mouse_owned = false;
	bool mouse_tracked = false;
	TRACKMOUSEEVENT mouse_track{};

	bool msc(int m) { return msd[m] && !msd_old[m]; }
	bool kbc(int k) { return kbd[k] && !kbd_old[k]; }

	void track_mouse();
	bool sync_mouse(HWND hwnd);

	void init(HWND hwnd);
	void reset();
};

struct app_fps {
	int frm_cnt = 0;
	int frm_cnt_max = 0;
	long long ns_cnt = 0;
	double fps = 0, frm_time = 0;
	long long real_dt = 0, real_dur = 0;
	std::chrono::high_resolution_clock::time_point tm;

	app_fps();
	void run();
	void update();

	double dt() const { return real_dt / 1e9; }
	double dur() const { return real_dur / 1e9; }
};

struct app_owners {
	void* kb = NULL;
	void* hvd = NULL;
	void* whd = NULL;
	double dhv = -DBL_MAX, dwh = -DBL_MAX;

	void reset();
	void remove(void* o);
	void free_kb(void* o);
};
