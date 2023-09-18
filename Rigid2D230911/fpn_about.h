#pragma once
#include "ctrl_list.h"
#include "rich_label.h"
#include "float_panel.h"

struct FPnAbout : FPnLan {
	ptr<RLbLan> rtb_about;
	ptr<CtrlListY> cly;
	FPnAbout(App& app);
};
