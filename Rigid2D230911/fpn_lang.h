#pragma once
#include "button.h"
#include "ctrl_list.h"
#include "float_panel.h"

struct BtChi : Button {
	BtChi();
	void OnClick(App& app) override;
};
struct BtEng : Button {
	BtEng();
	void OnClick(App& app) override;
};

struct FPnLang : FPnLan {
	ptr<BtChi> bt_chi;
	ptr<BtEng> bt_eng;
	ptr<CtrlListY> cly;
	FPnLang(App& app);
};
