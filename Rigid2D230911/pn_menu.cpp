#include "pn_menu.h"
#include "ui.h"
#include "cur.h"
#include "fpn_global.h"
#include "fpn_body.h"
#include "fpn_con.h"
#include "fpn_cmd.h"
#include "fpn_par.h"
#include "fpn_lang.h"
#include "fpn_about.h"
#include "creator.h"

#define cur (*(Cur*)&app)
#include "my_def.h"
#define mkbt(nm) mkp(bt_##nm)(&*ui.fpn_##nm);

BtPause::BtPause() : Button(150) {}
void BtPause::Update(App& app) {
	txt = cur.paused ? loc(L"start") : loc(L"pause");
	Button::Update(app);
}
void BtPause::OnClick(App& app) { cur.paused = !cur.paused; }

BtReset::BtReset() : BtLan(150, L"bt_reset") {}
void BtReset::OnClick(App& app) { cur.Reset(); }

BtMute::BtMute() : Button(150) {}
void BtMute::Update(App& app) {
	txt = cur.mute ? loc(L"unmute") : loc(L"mute");
	Button::Update(app);
}
void BtMute::OnClick(App& app) { cur.mute = !cur.mute; }

BtMode::BtMode() : Button(150) {}
void BtMode::Update(App& app) {
	switch (cur.mode) {
	case MODE_DRG: txt = loc(L"mode_drag"); break;
	case MODE_SEL: txt = loc(L"mode_sel"); break;
	case MODE_CRT: txt = loc(L"mode_create"); break;
	case MODE_DEL: txt = loc(L"mode_del"); break;
	}
	Button::Update(app);
}
void BtMode::OnClick(App& app) {
	cur.crt->Discard(cur); 
	cur.mode = (cur.mode + 1) % 4;
}

BtDisplay::BtDisplay() : Button(150) {}
void BtDisplay::Update(App& app) {
	switch (cur.display) {
	case DISP_COL: txt = loc(L"disp_col"); break;
	case DISP_ENERGY: txt = loc(L"disp_energy"); break;
	case DISP_CHARGE: txt = loc(L"disp_charge"); break;
	case DISP_NONE: txt = loc(L"disp_none"); break;
	}
	Button::Update(app);
}
void BtDisplay::OnClick(App& app) {
	cur.display = (cur.display + 1) % 4;
}

PnMenu::PnMenu(App& app) : Panel(Panel::menu(app)) {
	vector<Control*> tmp;
	mkp(bt_pause)(); mkp(bt_reset)(); mkp(bt_mute)();
	mkbt(global); mkbt(body); mkbt(con);
	mkp(bt_mode)(); mkp(bt_display)();
	mkbt(cmd); mkbt(par);
	mkbt(lang); mkbt(about);
	tmp = { &*bt_pause, &*bt_reset, &*bt_mute, &*bt_global, 
		&*bt_body, &*bt_con, &*bt_mode, &*bt_display,
		&*bt_cmd, &*bt_par, &*bt_lang, &*bt_about };
	mkcl(cly); c = &*cly; Init(app);
}
