#pragma once
#include "font.h"
#include "control.h"

struct RichLabel : Control {
	dvec gap;
	double dep = 0;
	int w = 0, h = 0;
	bool fixed_w = false;
	bool fixed_h = false;

	dcol c_txt;
	wstring txt;

	RichLabel(int w = 0);
	int GetW() const override;
	int GetH() const override;

	int w_line() const;
	void render(App& app);
	virtual void Sync(App& app) {}
	void Update(App& app);
};
struct RLbLan : RichLabel {
	wstring id_txt;
	RLbLan() = default;
	RLbLan(int w, wstring const& id);
	void Sync(App& app) override;
};
