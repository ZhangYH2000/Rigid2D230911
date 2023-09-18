#pragma once
#include "tile.h"

struct App;
struct Control;
struct ScrollbarY;
struct Panel {
	dvec tl;
	double dep = 0;
	Control* c = NULL;
	int w = 0, h = 0;
	double wheel_mtp = 0;
	dvec tl_show_rel;
	drect vp_show_rel;
	dcol c_frame, c_panel;

	bool use_sb = false;
	// 我也不清楚为啥要用指针。有点惯性的感觉了。
	ptr<ScrollbarY> sb;

	int h_show = 0;
	bool full = false;
	bool hovered = false;
	bool wheeled = false;

	Panel();
	static Panel menu(App& app);
	static Panel bottom(App& app);

	int min_y() const;
	drect vp_show(App& app) const;
	// 不用 delegate 而用专门的 set_c 主要是希望能重置滚动。
	void set_c(App& app, Control* _c);
	void init_c(App& app);
	void init_sb();
	void render(App& app);

	void Init(App& app);
	virtual void Update(App& app);
	// 如果有可删除的 Panel 就用这个。
	virtual void Discard(App& app);
	virtual void PreUpdate(App& app);
};
