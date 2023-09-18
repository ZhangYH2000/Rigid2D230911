#include "control.h"

void Delegate::set_c(App& app, Control* _c) {
	if (c && c != _c) { c->Discard(app); }
	c = _c;
}
int Delegate::GetW() const { return c ? c->GetW() : 0; }
int Delegate::GetH() const { return c ? c->GetH() : 0; }

void Delegate::Update(App& app) {
	if (c) { c->tl = tl; c->vp = vp; c->Update(app); }
}
void Delegate::Discard(App& app) { if (c) { c->Discard(app); } }
void Delegate::PreUpdate(App& app) { if (c) { c->PreUpdate(app); } }
