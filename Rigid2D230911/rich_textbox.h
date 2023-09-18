#pragma once
#include "vec2.h"
#include "font.h"
#include "control.h"

struct RichTextbox : Control {
	dvec gap;
	int w_str = 0;
	double dep = 0;
	int w = 0, h = 0;
	bool clr_upon_edit = false;
	dcol c_str, c_edit, c_frame, c_normal,
		c_cursor, c_hovered, c_invalid, c_selected;

	dvec edit_mg;
	dvec show_txt_mg;
	double slide_mtp = 0;
	double wheel_mtp = 0;
	double twinkle_mtp = 0;
	int w_cursor = 0, h_cursor = 0;

	wstring str;
	vec2 tl_str_rel;
	int edit_a = 0, edit_b = 0;
	// 下面的 rel 是相对于 tl_str_rel 的。
	int x_cs_rel_hist = 0;

	bool edit = false;
	bool enabled = true;
	bool hovered = false;
	bool selecting = false;
	bool cursor_change = false;

	RichTextbox();
	RichTextbox(int w, int h, bool newline = true);
	int GetW() const override;
	int GetH() const override;

	drect vp_show_txt() const;
	wstring left() const;
	wstring mid() const;
	wstring right() const;
	int edit_begin() const;
	int edit_end() const;
	bool selected() const;
	int max_x_cs_rel() const;
	int max_y_cs_rel() const;
	dvec p_cs_rel(App& app) const;

	void select_a(App& app);
	void select_b(App& app);
	void update_x_cs_rel_hist(App& app);

	void add_chs(App& app);
	void hdl_left(App& app);
	void hdl_right(App& app);
	void hdl_up(App& app);
	void hdl_down(App& app);
	void hdl_back(App& app);
	void hdl_delete(App& app);
	void hdl_clip(App& app);
	void hdl_copy(App& app);
	void hdl_paste(App& app);
	void hdl_all(App& app);
	void hdl_key(App& app);

	void clamp_cs_selecting(App& app);
	void clamp_cs_not_selecting(App& app);
	void clamp_cursor(App& app);
	void clamp_str(App& app);

	void render_main(App& app);
	void render_cursor(App& app);
	void render(App& app);

	virtual void Sync(App& app) {}
	virtual void OnDone(App& app) const {}
	virtual void Upload(App& app) const {}

	void Update(App& app) override;
	void Discard(App& app) override;
	void PreUpdate(App& app) override;
};
