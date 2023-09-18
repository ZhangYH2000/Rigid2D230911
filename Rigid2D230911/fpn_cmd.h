#pragma once
#include "button.h"
#include "ctrl_list.h"
#include "float_panel.h"
#include "rich_textbox.h"

struct RTbCmd : RichTextbox {
	RTbCmd();
	void Sync(App& app) override;
	void OnDone(App& app) const override;
};
struct BtCompile : BtLan {
	BtCompile();
	void OnClick(App& app) override;
};

struct FPnCmd : FPnLan {
	ptr<RTbCmd> rtb;
	ptr<BtCompile> bt_compile;
	ptr<CtrlListY> cly;
	FPnCmd(App& app);
};
