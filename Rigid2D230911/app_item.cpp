#include "app_item.h"

void app_input::track_mouse() {
	if (!mouse_tracked) {
		mouse_tracked = true;
		TrackMouseEvent(&mouse_track);
	}
}
bool app_input::sync_mouse(HWND hwnd) {
	POINT pnt{}; BOOL ret = GetCursorPos(&pnt);
	if (!ret) { return false; }
	ret = ScreenToClient(hwnd, &pnt);
	if (!ret) { return false; }

	msp_old = msp; msp = { pnt.x, pnt.y };
	if (!mouse_owned) {
		bool hold = HIWORD(GetAsyncKeyState(VK_LBUTTON));
		if (!hold) { msd[0] = false; }
		hold = HIWORD(GetAsyncKeyState(VK_MBUTTON));
		if (!hold) { msd[1] = false; }
		hold = HIWORD(GetAsyncKeyState(VK_RBUTTON));
		if (!hold) { msd[2] = false; }
	} return true;
}

void app_input::init(HWND hwnd) {
	mouse_track.cbSize = sizeof(mouse_track);
	mouse_track.dwFlags = TME_LEAVE;
	mouse_track.hwndTrack = hwnd;
}
void app_input::reset() {
	msw = 0; chs.clear();
	// msp 在 App 中已经做过了。
	msd_old = msd; kbd_old = kbd;
	kbmsg.fill(0); bar_clicked = false;
}

app_fps::app_fps() { frm_cnt_max = 10; }
void app_fps::run() { tm = std::chrono::high_resolution_clock::now(); }
void app_fps::update() {
	auto now = std::chrono::high_resolution_clock::now();
	real_dur += real_dt = (now - tm).count();
	tm = now; frm_cnt++; ns_cnt += real_dt;

	if (frm_cnt == frm_cnt_max) {
		frm_time = (double)ns_cnt / frm_cnt * 1e-9;
		fps = 1 / frm_time;
		frm_cnt = ns_cnt = 0;
	}
}

void app_owners::reset() {
	dhv = dwh = -DBL_MAX; kb = hvd = whd = NULL;
}
void app_owners::remove(void* o) {
	if (kb == o) { kb = NULL; }
	if (hvd == o) { hvd = NULL; }
	if (whd == o) { whd = NULL; }
}
void app_owners::free_kb(void* o) {
	if (kb == o) { kb = NULL; }
}
