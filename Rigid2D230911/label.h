#pragma once
#include "font.h"
#include "control.h"

struct Label : Control {
	dvec gap;
	double dep = 0;
	int w = 0, h = 0;
	bool fixed_w = false;

	dcol c_txt;
	wstring txt;

	Label(bool big = false);
	int GetW() const override;
	int GetH() const override;

	void render(App& app);
	virtual void Sync(App& app) {}
	void Update(App& app) override;
};
struct LbLan : Label {
	wstring id_txt;
	LbLan() = default;
	LbLan(wstring const& id, bool big = false);
	void Sync(App& app) override;
};
