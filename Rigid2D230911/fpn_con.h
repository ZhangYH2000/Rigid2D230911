#pragma once
#include "button.h"
#include "ctrl_list.h"
#include "float_panel.h"
#include "rich_textbox.h"
#include "rich_label.h"
#include "separator.h"

struct Connection;
struct RTbCon : RichTextbox {
	RTbCon();
	void Sync(App& app) override;
	void Upload(App& app) const override;
};
struct BtConOK : BtLan {
	BtConOK();
	void OnClick(App& app) override;
};
struct BtConDel : BtLan {
	BtConDel();
	void OnClick(App& app) override;
};

struct ClyCon : CtrlListY {
	Connection* con = NULL;
	ptr<RTbCon> rtb;
	ptr<BtConOK> bt_ok;
	ptr<BtConDel> bt_del;
	ptr<CtrlListX> clx_bt;
	ptr<SeparatorY> spy;
	ClyCon();
	void Update(App& app) override;
	void Discard(App& app) override;
};

struct RLbCon : RichLabel {
	void Sync(App& app) override;
};

struct FPnCon : FPnLan {
	ptr<ClyCon> cly_con;
	ptr<RLbCon> rlb_con;
	ptr<CtrlListY> cly;
	FPnCon(App& app);
};
