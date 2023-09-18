#include "fpn_con.h"
#include "cur.h"
#include "connection.h"

#define cur (*(Cur*)&app)
#include "my_def.h"
#define pcsel (cur.con_sel)
#define csel (*pcsel)

RTbCon::RTbCon() : RichTextbox(360, 400) {}
void RTbCon::Sync(App& app) { str = csel.tmp_cmd; }
void RTbCon::Upload(App& app) const { csel.tmp_cmd = str; }

BtConOK::BtConOK() : BtLan(70, L"bt_con_ok") {}
void BtConOK::OnClick(App& app) {
	csel.cmd = csel.tmp_cmd; csel.refresh(cur);
}
BtConDel::BtConDel() : BtLan(70, L"bt_con_del") {}
void BtConDel::OnClick(App& app) { csel.del = true; cur.scene_changed = true; }

ClyCon::ClyCon() {
	vector<Control*> tmp;
	mkp(rtb)(); 
	mkp(bt_ok)(); mkp(bt_del)();
	tmp = { &*bt_ok, &*bt_del };
	mkcl(clx_bt);
	mkp(spy)(400);
}
void ClyCon::Update(App& app) {
	if (con != pcsel) {
		con = pcsel;
		for (auto c : cs) { c->Discard(app); }
		if (con) { cs = { &*rtb, &*clx_bt, &*spy }; }
		else { cs = {}; }
	}
	CtrlListY::Update(app);
}
void ClyCon::Discard(App& app) {
	CtrlListY::Discard(app);
	con = NULL; cs = {};
}

void RLbCon::Sync(App& app) {
	if (!pcsel) { txt = loc(L"conn_not_sel"); return; }
	txt.clear();

	wstring type;
	switch (csel.type) {
	case CON_ROPE: type = loc(L"conn_rope"); break;
	case CON_LINK: type = loc(L"conn_link"); break;
	case CON_SPRG: type = loc(L"conn_spring"); break;
	case CON_CORD: type = loc(L"conn_cord"); break;
	}
	txt += loc(L"conn_type") + type + L"\n";
	txt += loc(L"conn_len") + tw2(csel.len) + L"\n";
	txt += loc(L"conn_hooke") + tw2(csel.hooke) + L"\n";
}

FPnCon::FPnCon(App& app) : FPnLan(app, 400, 600, L"fpn_con") {
	vector<Control*> tmp;
	mkp(cly_con)(); mkp(rlb_con)();
	tmp = { &*cly_con, &*rlb_con };
	mkcl(cly); c = &*cly; Init(app);
}
