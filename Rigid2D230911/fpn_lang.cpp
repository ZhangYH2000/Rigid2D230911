#include "fpn_lang.h"
#include "ui.h"
#include "cur.h"

#define cur (*(Cur*)&app)
#include "my_def.h"

BtChi::BtChi() : Button(150) { txt = L"ÖÐÎÄ"; }
void BtChi::OnClick(App& app) { cur.lan = LAN_CHI; ui.fpn_lang->hide(app); }
BtEng::BtEng() : Button(150) { txt = L"English"; }
void BtEng::OnClick(App& app) { cur.lan = LAN_ENG; ui.fpn_lang->hide(app); }

FPnLang::FPnLang(App& app) : FPnLan(app, 200, 200, L"fpn_lang") {
	vector<Control*> tmp; shown = true;
	mkp(bt_chi)();
	mkp(bt_eng)();
	tmp = { &*bt_chi, &*bt_eng };
	mkcl(cly); c = &*cly; Init(app);
}
