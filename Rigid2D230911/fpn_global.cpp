#include "fpn_global.h"
#include "ui.h"
#include "cur.h"
#include "creator.h"

#define cur (*(Cur*)&app)
#include "my_def.h"

BtSave::BtSave() : BtLan(80, L"bt_save") {}
void BtSave::OnClick(App& app) { cur.Save(ui.fpn_global->tb_save->str); }
BtLoad::BtLoad() : BtLan(80, L"bt_load") {}
void BtLoad::OnClick(App& app) { cur.Load(ui.fpn_global->tb_load->str); }

void RLbGlobal::Sync(App& app) {
	txt = L"";
	txt += loc(L"vol") + tw2(cur.vol) + L"\n";
	txt += loc(L"n_step") + tw(cur.n_step) + L"\n";
	txt += loc(L"s_grid") + tw2(cur.s_grid) + L"\n";
	txt += loc(L"max_real_dt") + tw2(cur.max_real_dt) + L"\n";
	txt += loc(L"tl_scene") + tw(cur.rect_scene.tl) + L"\n";
	txt += loc(L"w_scene") + tw(cur.rect_scene.w) + L"\n";
	txt += loc(L"h_scene") + tw(cur.rect_scene.h) + L"\n";
	txt += loc(L"gravity") + tw2(cur.gravity) + L"\n";
	txt += loc(L"electrostatic") + tw(cur.electrostatic) + L"\n";
	txt += loc(L"coulomb") + tw2(cur.coulomb) + L"\n";

	wstring s;
	switch (cur.crt->mode) {
	case CRT_BALL: s = loc(L"crt_ball"); break;
	case CRT_BOX: s = loc(L"crt_box"); break;
	case CRT_CONN: s = loc(L"crt_conn"); break;
	case CRT_NAIL: s = loc(L"crt_nail"); break;
	case CRT_PARTICLE: s = loc(L"crt_particle"); break;
	case CRT_PLATE: s = loc(L"crt_plate"); break;
	case CRT_POINT: s = loc(L"crt_point"); break;
	}
	txt += loc(L"creator_mode") + s + L"\n";
}

FPnGlobal::FPnGlobal(App& app) : FPnLan(app, 400, 600, L"fpn_global") {
	vector<Control*> tmp;
	mkp(tb_save)(270); mkp(bt_save)();
	tmp = { &*tb_save, &*bt_save }; mkcl(clx_save);
	mkp(tb_load)(270); mkp(bt_load)();
	tmp = { &*tb_load, &*bt_load }; mkcl(clx_load);
	mkp(spy)(400); mkp(rlb_global)();
	tmp = { &*clx_save, &*clx_load, &*spy, &*rlb_global };
	mkcl(cly); c = &*cly; Init(app);
}
