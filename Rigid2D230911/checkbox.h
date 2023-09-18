#pragma once
#include "tile.h"
#include "control.h"

struct Checkbox : Control {
	int h = 0;
	dvec gap;
	double dep = 0;
	wstring txt;
	int s_box = 0, s_box_in = 0;
	dcol c_txt, c_frame, c_inner, c_normal, c_invalid, c_hovered;

	int w_txt = 0;
	bool checked = false;
	bool enabled = true;
	bool hovered = false;

	Checkbox(bool big = false);
	int GetW() const override;
	int GetH() const override;

	dvec tl_box() const;
	void render(App& app);
	virtual void Sync(App& app) {}
	virtual void Upload(App& app) const {}

	void Update(App& app) override;
	void Discard(App& app) override;
	void PreUpdate(App& app) override;
};
struct CbLan : Checkbox {
	wstring id_txt;
	CbLan() = default;
	CbLan(wstring const& id, bool big = false);
	void Update(App& app) override;
};
