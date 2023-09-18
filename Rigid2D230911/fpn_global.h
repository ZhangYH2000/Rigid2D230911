#pragma once
#include "label.h"
#include "button.h"
#include "textbox.h"
#include "ctrl_list.h"
#include "float_panel.h"
#include "rich_label.h"
#include "separator.h"

struct BtSave : BtLan {
	BtSave();
	void OnClick(App& app) override;
};
struct BtLoad : BtLan {
	BtLoad();
	void OnClick(App& app) override;
};
struct RLbGlobal : RichLabel {
	void Sync(App& app) override;
};

struct FPnGlobal : FPnLan {
	ptr<Textbox> tb_save;
	ptr<BtSave> bt_save;
	ptr<CtrlListX> clx_save;
	ptr<Textbox> tb_load;
	ptr<BtLoad> bt_load;
	ptr<CtrlListX> clx_load;
	ptr<SeparatorY> spy;
	ptr<RLbGlobal> rlb_global;
	ptr<CtrlListY> cly;
	FPnGlobal(App& app);
};
