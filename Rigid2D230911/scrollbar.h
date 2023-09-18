#pragma once
#include "tile.h"

struct App;
struct Control;
// 如果对象的高度超过了范围，Scrollbar 不会自动更正。
// 这一权限留给 Panel 等对象的直接上级。
// Scrollbar 不是控件，不继承 Control 类。
struct ScrollbarY {
	dvec tl;
	double dep = 0;
	Control* c = NULL;
	int w = 0, h = 0;
	int h_show = 0, top_show = 0;
	dcol c_bar, c_frame, c_normal, c_dragged, c_hovered, c_invalid;

	int ny_rel = 0, nh = 0;
	bool full = false;
	bool dragged = false;
	bool enabled = true;
	bool hovered = false;
	bool hovered_node = false;

	ScrollbarY();
	// 没有 set_c 不会 Discard 之前的对象。
	void render(App& app);

	virtual void Update(App& app);
	virtual void Discard(App& app);
	virtual void PreUpdate(App& app);
};
