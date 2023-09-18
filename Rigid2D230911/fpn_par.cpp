#include "fpn_par.h"
#include "cur.h"
#include "par.h"

#define cur (*(Cur*)&app)
#include "my_def.h"
#define par (*cur.pars[id])

TbParNm::TbParNm(int id) : id(id), Textbox(270) {};
void TbParNm::Sync(App& app) { str = par.nm; }
void TbParNm::OnDone(App& app) const { par.nm = str; }
BtDelPar::BtDelPar(int id) : id(id), BtLan(80, L"bt_del_par") {}
void BtDelPar::OnClick(App& app) { par.del = true; }
DbXParVal::DbXParVal(int id) : id(id), DragbarX(270) {}
void DbXParVal::Sync(App& app) { set_nx_rel(par.val); }
void DbXParVal::Upload(App& app) const { par.val = uval(); }
TbParVal::TbParVal(int id) : id(id), Textbox(80) {};
void TbParVal::Sync(App& app) { str = tw2(par.val); }
void TbParVal::OnDone(App& app) const { par.val = wtof(str); }

ClyPar::ClyPar(int id) {
	vector<Control*> tmp;
	mkp(tb_nm)(id); mkp(bt_del)(id);
	tmp = { &*tb_nm, &*bt_del }; mkcl(clx_nm);
	mkp(dbx_val)(id); mkp(tb_val)(id);
	tmp = { &*dbx_val, &*tb_val }; mkcl(clx_val);
	mkp(spy)(400);
	cs = { &*clx_nm, &*clx_val, &*spy };
}
void ClyPars::Update(App& app) {
	cs.clear();
	rep(i, mbs.size(), cur.pars.size()) { mbs.push_back(msh<ClyPar>(i)); }
	rep(i, 0, cur.pars.size()) { cs.push_back(&*mbs[i]); }
	rep(i, cur.pars.size(), mbs.size()) { mbs[i]->Discard(app); }
	CtrlListY::Update(app);
}

BtAddPar::BtAddPar() : BtLan(160, L"bt_add_par") {}
void BtAddPar::OnClick(App& app) { cur.pars.push_back(msh<param>(loc(L"unnamed_par"))); }

FPnPar::FPnPar(App& app) : FPnLan(app, 400, 600, L"fpn_par") {
	vector<Control*> tmp;
	mkp(cly_pars)();
	mkp(bt_add_par)();
	tmp = { &*cly_pars, &*bt_add_par };
	mkcl(cly); c = &*cly; Init(app);
}
