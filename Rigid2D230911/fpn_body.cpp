#include "fpn_body.h"
#include "cur.h"
#include "body.h"
#include "connection.h"

#define cur (*(Cur*)&app)
#include "my_def.h"
#define pbsel (cur.body_sel)
#define bsel (*pbsel)
#define pcsel (cur.con_sel)
#define pcon (bsel.cons[id])

RTbBody::RTbBody() : RichTextbox(360, 400) {}
void RTbBody::Sync(App& app) { str = bsel.tmp_cmd; }
void RTbBody::Upload(App& app) const { bsel.tmp_cmd = str; }

BtBodyOK::BtBodyOK() : BtLan(70, L"bt_body_ok") {}
void BtBodyOK::OnClick(App& app) {
	bsel.cmd = bsel.tmp_cmd; bsel.refresh(cur);
}
BtBodyDel::BtBodyDel() : BtLan(70, L"bt_body_del") {}
void BtBodyDel::OnClick(App& app) { bsel.del = true; cur.scene_changed = true; }


LbBodyCon::LbBodyCon(int id) : id(id), Label() {
	fixed_w = true; w = 270;
}
void LbBodyCon::Sync(App& app) {
	wstring type;
	switch (pcon->type) {
	case CON_ROPE: type = loc(L"conn_rope"); break;
	case CON_LINK: type = loc(L"conn_link"); break;
	case CON_SPRG: type = loc(L"conn_spring"); break;
	case CON_CORD: type = loc(L"conn_cord"); break;
	}
	txt = loc(L"conn") + tw(id) + L": " + type;
}

CbBodyCon::CbBodyCon(int id) : id(id), CbLan(L"cb_body_con") {}
void CbBodyCon::Sync(App& app) { checked = (pcsel == pcon); }
void CbBodyCon::Upload(App& app) const { pcsel = checked ? pcon : NULL; }

ClXBodyCon::ClXBodyCon(int id) {
	mkp(cb)(id); mkp(lb)(id);
	cs = { &*lb, &*cb };
}
void ClYBodyCons::Update(App& app) {
	rep(i, mbs.size(), bsel.cons.size()) {
		mbs.push_back(msh<ClXBodyCon>(i));
	}
	cs.clear();
	rep(i, 0, bsel.cons.size()) {
		cs.push_back(&*mbs[i]);
	}
	rep(i, bsel.cons.size(), mbs.size()) {
		mbs[i]->Discard(app);
	}
	CtrlListY::Update(app);
}

ClyBody::ClyBody() {
	vector<Control*> tmp;
	mkp(rtb)(); 
	mkp(bt_ok)(); mkp(bt_del)();
	tmp = { &*bt_ok, &*bt_del };
	mkcl(clx_bt);
	mkp(cly_cons)(); mkp(spy)(400);
}
void ClyBody::Update(App& app) {
	if (bo != pbsel) {
		bo = pbsel;
		for (auto c : cs) { c->Discard(app); }
		if (bo) { cs = { &*cly_cons, &*rtb, &*clx_bt, &*spy }; }
		else { cs = {}; }
	}
	CtrlListY::Update(app);
}
void ClyBody::Discard(App& app) {
	CtrlListY::Discard(app);
	bo = NULL; cs = {};
}

void RLbBody::Sync(App& app) {
	if (!pbsel) { txt = loc(L"body_not_sel"); return; }
	txt.clear();
	
	txt += loc(L"body_o") + tw2(bsel.o) + L"\n";
	txt += loc(L"body_v") + tw2(bsel.v) + L"\n";
	txt += loc(L"body_ang") + tw2(bsel.ang) + L"\n";
	txt += loc(L"body_v_ang") + tw2(bsel.v_ang) + L"\n";
	txt += loc(L"body_fixed") + tw(bsel.fixed) + L"\n";
	txt += loc(L"body_density") + tw2(bsel.density) + L"\n";
	txt += loc(L"body_charge_density") + tw2(bsel.charge_density) + L"\n";
	txt += loc(L"body_e") + tw2(bsel.e) + L"\n";
	txt += loc(L"body_mu_s") + tw2(bsel.mu_s) + L"\n";
	txt += loc(L"body_mu_d") + tw2(bsel.mu_d) + L"\n";
	txt += loc(L"body_damp_v") + tw2(bsel.damp_v) + L"\n";
	txt += loc(L"body_damp_v_ang") + tw2(bsel.damp_v_ang) + L"\n";
}

FPnBody :: FPnBody(App& app) : FPnLan(app, 400, 600, L"fpn_body") {
	vector<Control*> tmp;
	mkp(cly_body)(); mkp(rlb_body)();
	tmp = { &*cly_body, &*rlb_body };
	mkcl(cly); c = &*cly; Init(app);
}
