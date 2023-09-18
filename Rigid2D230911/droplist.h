#pragma once
#include "font.h"
#include "control.h"

typedef function<wstring(int, App&)> DpNm;
wstring dpnm_none(int id, App &app);

struct Droplist : Control {
	dvec gap;
	double dep = 0;
	wstring txt;
	int w = 0, h = 0;

	// 即使某些设计中 picked 可以取 -1，默认值还是设成 0 吧。
	// 这里 picked 能取什么值还没想清楚的，能不能取 -1 之类，要实践中得到。
	int picked = 0;
	int h_item = 0;
	int n_item = 0;
	DpNm nm = dpnm_none;

	double sign_r = 0;
	int sign_margin = 0;
	int x_txt_margin = 0;
	dcol c_txt, c_mark, c_edit, c_frame, c_normal, c_invalid, c_hovered;

	// -1 代表没有悬浮在任何选项上面。
	int item_hv = -1;
	int w_txt = 0;
	bool edit = false;
	bool enabled = true;
	bool hovered = false;

	Droplist(int w = 0, DpNm const &nm = dpnm_none);
	int GetW() const override;
	int GetH() const override;

	dvec tl_box() const;
	dvec tl_item() const;
	void render_main(App& app);
	void render_items(App& app);
	void render(App& app);
	virtual void Sync(App& app) {}
	// 现在默认的是 Upload 的校正不改变控件本身值，只改变目标值。
	virtual void Upload(App& app) const {}

	void Update(App& app) override;
	void Discard(App& app) override;
	void PreUpdate(App& app) override;
};
struct DpLan : Droplist {
	wstring id_txt;
	DpLan() = default;
	DpLan(wstring const& id, int w, DpNm const& nm = dpnm_none);
	void Update(App& app) override;
};
