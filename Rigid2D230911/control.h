#pragma once
#include "dvec.h"

struct App;
struct Control {
	dvec tl;
	drect vp;

	virtual ~Control() {}
	virtual void Update(App& app) {}
	// 在析构之前要执行 Discard。
	virtual void Discard(App& app) {}
	virtual void PreUpdate(App& app) {}

	virtual int GetW() const { return 0; }
	virtual int GetH() const { return 0; }
};
// Delegate 会自动 Discard 它的成员，这是它与 CtrlList 不同的一点。
// Delegate 要求 c 一直是有效的，即使是在 c 被覆盖的那一帧。
struct Delegate : Control {
	Control* c = NULL;
	void set_c(App& app, Control* _c);
	int GetW() const override;
	int GetH() const override;

	void Update(App& app) override;
	void Discard(App& app) override;
	void PreUpdate(App& app) override;
};
