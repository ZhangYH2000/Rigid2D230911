#pragma once
#include "font.h"
#include "control.h"

struct Textbox : Control {
	dvec gap;
	double dep = 0;
	wstring txt;
	int w = 0, h = 0;
	bool clr_upon_edit = false;
	dcol c_txt, c_str, c_edit, c_frame, c_normal,
		c_cursor, c_hovered, c_invalid, c_selected;

	// cs = cursor，mg = margin。
	// 也是 x_str_rel 的默认值，否则一开始编辑文字的位置就会变化。
	int w_edit_mg = 0;
	int w_show_txt_mg = 0;
	double slide_mtp = 0;
	double twinkle_mtp = 0;
	int w_cursor = 0, h_cursor = 0;

	wstring str;
	double x_str_rel = 0;
	int edit_a = 0, edit_b = 0;

	int w_txt = 0;
	bool edit = false;
	bool enabled = true;
	bool hovered = false;
	bool selecting = false;
	bool cursor_change = false;

	Textbox(int w = 0);
	int GetW() const override;
	int GetH() const override;

	dvec tl_box() const;
	drect vp_show_txt() const;
	wstring left() const;
	wstring mid() const;
	wstring right() const;
	int edit_begin() const;
	int edit_end() const;
	bool selected() const;
	int max_x_cs_rel() const;
	int x_cs_rel(App& app) const;

	void select_a(App& app);
	void select_b(App& app);

	void add_chs(App& app);
	void hdl_left(App& app);
	void hdl_right(App& app);
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
struct TbLan : Textbox {
	wstring id_txt;
	TbLan() = default;
	TbLan(wstring const& id, int w);
	void Update(App& app) override;
};
