#pragma once
#include "tile.h"

struct App;
struct Control;
struct ScrollbarY;
struct FloatPanel {
	dvec tl;
	double dep = 0;
	Control* c = NULL;
	wstring txt;
	int w = 0, h = 0;
	double wheel_mtp = 0;
	dvec tl_show_rel;
	drect vp_show_rel;

	bool use_sb = false;
	ptr<ScrollbarY> sb;

	int h_bar = 0;
	int s_close = 0;
	int half_s_mark = 0;
	int margin_close = 0;
	int margin_title = 0;
	dcol c_txt, c_mark, c_frame, c_panel;
	dcol c_bar_normal, c_bar_dragged, c_bar_hovered;
	dcol c_close_normal, c_close_chosen, c_close_hovered, c_close_invalid;

	int h_show = 0;
	double t_click = 0;
	bool full = false;
	bool shown = false;
	bool hovered = false;
	bool wheeled = false;
	bool hovered_bar = false;
	bool dragged_bar = false;
	bool chosen_close = false;
	bool hovered_close = false;
	bool enabled_close = true;

	FloatPanel();
	FloatPanel(App& app, int _w, int _h);

	int min_y() const;
	drect vp_show(App& app) const;
	dvec tl_close() const;
	void set_c(App& app, Control* _c);
	void set_tl(App& app, dvec const& p);
	void init_c(App& app);
	void init_sb();

	void hide(App& app);
	void show(App& app);
	void deal_caption(App& app);
	void wheel_and_clamp_cy(App& app);

	void render_main(App& app);
	void render_close(App& app);
	void render(App& app);

	void Init(App& app);
	virtual void Update(App& app);
	virtual void Discard(App& app);
	virtual void PreUpdate(App& app);
};
struct FPnLan : FloatPanel {
	wstring id_txt;
	FPnLan() = default;
	FPnLan(App& app, int w, int h, wstring const& id);
	void Update(App& app) override;
};

bool less_fpn(FloatPanel* const fp0, FloatPanel* const fp1);
