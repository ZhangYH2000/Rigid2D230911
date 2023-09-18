#pragma once
#include "panel.h"
#include "label.h"
#include "ctrl_list.h"

struct LbFPS : Label {
	LbFPS();
	void Sync(App& app) override;
};

struct LbTime : Label {
	LbTime();
	void Sync(App& app) override;
};
struct LbMouse : Label {
	LbMouse();
	void Sync(App& app) override;
};

struct PnBottom : Panel {
	ptr<LbFPS> lb_fps;
	ptr<LbTime> lb_time;
	ptr<LbMouse> lb_mouse;
	ptr<CtrlListX> clx;
	PnBottom(App& app);
};
