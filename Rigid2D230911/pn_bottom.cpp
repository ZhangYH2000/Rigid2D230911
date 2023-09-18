#include "pn_bottom.h"
#include "cur.h"

#define cur (*(Cur*)&app)
#include "my_def.h"

LbFPS::LbFPS() { w = 450; fixed_w = true; }
void LbFPS::Sync(App& app) {
	txt = loc(L"frm_tm") + tw2(cur.fps.frm_time * 1000) +
		L", FPS: " + tw2(cur.fps.fps);
}

LbTime::LbTime() { w = 200; fixed_w = true; }
void LbTime::Sync(App& app) {
	txt = loc(L"lb_time") + tw2(cur.t);
}
LbMouse::LbMouse() { w = 300; fixed_w = true; }
void LbMouse::Sync(App& app) {
	txt = loc(L"lb_mouse") + tw(msp);
}

PnBottom::PnBottom(App& app) : Panel(Panel::bottom(app)) {
	vector<Control*> tmp;
	mkp(lb_fps)(); mkp(lb_time)(); mkp(lb_mouse)();
	tmp = { &*lb_fps, &*lb_time, &*lb_mouse };
	mkcl(clx); c = &*clx; Init(app);
}
