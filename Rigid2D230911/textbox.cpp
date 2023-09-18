#include "textbox.h"
#include "app.h"
#include "draw_geo.h"
#include "draw_str.h"
#include "draw_comp.h"
#include "rich_textbox.h"

#include "ctrl_def.h"

Textbox::Textbox(int w) : w(w) {
	gap = { 10, 10 }; 
	dep = 1000; h = 30;

	c_txt = { 255, 255, 255 };
	c_str = { 255, 255, 255 };
	c_edit = { 150, 60, 60 };
	c_frame = { 150, 150, 150 };
	c_normal = { 40, 40, 40 };
	c_cursor = { 255, 255, 255 };
	c_hovered = { 60, 60, 150 };
	c_invalid = { 0, 0, 0 };
	c_selected = { 150, 150, 255 };

	w_edit_mg = 5;
	w_show_txt_mg = 2;
	slide_mtp = 8;
	twinkle_mtp = 8;
	w_cursor = 2; h_cursor = 20;
}
int Textbox::GetW() const { return w + gap.x + w_txt; }
int Textbox::GetH() const { return h + gap.y; }

dvec Textbox::tl_box() const { return tl + dvec(w_txt, 0); }
drect Textbox::vp_show_txt() const {
	dvec tl_show_txt = tl_box() + dvec(w_show_txt_mg, 0);
	int w_show_txt = w - 2 * w_show_txt_mg;
	return overlap(vp, { tl_show_txt, w_show_txt, h });
}
wstring Textbox::left() const {
	auto a = str.begin();
	auto b = str.begin() + edit_begin();
	return wstring(a, b);
}
wstring Textbox::mid() const {
	auto a = str.begin() + edit_begin();
	auto b = str.begin() + edit_end();
	return wstring(a, b);
}
wstring Textbox::right() const {
	auto a = str.begin() + edit_end();
	auto b = str.end();
	return wstring(a, b);
}
int Textbox::edit_begin() const { return min(edit_a, edit_b); }
int Textbox::edit_end() const { return max(edit_a, edit_b); }
bool Textbox::selected() const { return edit_a != edit_b; }
// 下面是对的，不需要加一减一什么的。
int Textbox::max_x_cs_rel() const { return w - w_edit_mg; }
int Textbox::x_cs_rel(App& app) const {
	wstring tmp(str.begin(), str.begin() + edit_b);
	return x_str_rel + str_wh(tmp, ft, 0).x;
}

void Textbox::select_a(App& app) {
	selecting = true;
	int x = msp.x - (tl_box().x + x_str_rel);
	edit_a = edit_b = str_index_at_pos(str, ft, { x, 0 }, 0);
}
void Textbox::select_b(App& app) {
	int x = msp.x - (tl_box().x + x_str_rel);
	edit_b = str_index_at_pos(str, ft, { x, 0 }, 0);
}

void Textbox::add_chs(App& app) {
	wstring sifted = sift_chs(chs, true);
	if (!sifted.empty()) {
		selecting = false;
		str = left() + sifted + right();
		edit_a = edit_b = edit_begin() + sifted.size();
	}
}
void Textbox::hdl_left(App& app) {
	selecting = false; 
	cursor_change = true;

	if (selected()) { edit_a = edit_b = edit_begin(); }
	else {
		if (--edit_a < 0) { edit_a = 0; }
		edit_b = edit_a;
	}
}
void Textbox::hdl_right(App& app) {
	selecting = false; 
	cursor_change = true;

	if (selected()) { edit_a = edit_b = edit_end(); }
	else {
		// 下面不要减一。
		if (++edit_a > str.size()) { edit_a = str.size(); }
		edit_b = edit_a;
	}
}
void Textbox::hdl_back(App& app) {
	selecting = false; 
	cursor_change = true;

	if (selected()) {
		str = left() + right();
		edit_a = edit_b = edit_begin();
	} else if (edit_begin() > 0) {
		x_str_rel += ft[str[edit_a]].w;
		x_str_rel = min<double>(x_str_rel, w_edit_mg);
		auto a = str.begin();
		auto b = str.begin() + edit_a - 1;
		str = wstring(a, b) + right();
		edit_b = --edit_a;
	}
}
void Textbox::hdl_delete(App& app) {
	selecting = false; 
	cursor_change = true;

	if (selected()) {
		str = left() + right();
		edit_a = edit_b = edit_begin();
	} else if (edit_end() < str.size()) {
		auto a = str.begin() + edit_a + 1;
		auto b = str.end();
		str = left() + wstring(a, b);
	}
}
void Textbox::hdl_clip(App& app) {
	selecting = false;
	set_clipboard(mid());
	str = left() + right();
	edit_a = edit_b = edit_begin();
}
void Textbox::hdl_copy(App& app) { set_clipboard(mid()); }
void Textbox::hdl_paste(App& app) {
	selecting = false;
	wstring sifted = sift_chs(get_clipboard(), true);
	str = left() + sifted + right();
	edit_a = edit_b = edit_begin() + sifted.size();
}
void Textbox::hdl_all(App& app) {
	selecting = false;
	edit_a = 0; edit_b = str.size();
}
void Textbox::hdl_key(App& app) {
	cursor_change = false;

	if (kbmsg[VK_LEFT]) { hdl_left(app); }
	else if (kbmsg[VK_RIGHT]) { hdl_right(app); }
	else if (kbmsg[VK_DELETE]) { hdl_delete(app); }
	else if (kbmsg[VK_BACK]) { hdl_back(app); }

	if (kbd[VK_CONTROL]) {
		if (kbc('C')) { hdl_copy(app); }
		else if (kbc('X')) { hdl_clip(app); }
		else if (kbc('V')) { hdl_paste(app); }
		else if (kbc('A')) { hdl_all(app); }
	}
}

void Textbox::clamp_cs_selecting(App& app) {
	double val = (msp.x - tl_box().x) - max_x_cs_rel();
	if (val > 0) { x_str_rel -= dt * slide_mtp * val; }
	val = w_edit_mg - (msp.x - tl_box().x);
	if (val > 0) { x_str_rel += dt * slide_mtp * val; }
}
void Textbox::clamp_cs_not_selecting(App& app) {
	if (x_cs_rel(app) > max_x_cs_rel()) {
		x_str_rel -= x_cs_rel(app) - max_x_cs_rel();
	} else if (x_cs_rel(app) < w_edit_mg) {
		x_str_rel += w_edit_mg - x_cs_rel(app);
	}
}
void Textbox::clamp_cursor(App& app) {
	if (selecting) { clamp_cs_selecting(app); }
	else { clamp_cs_not_selecting(app); }
}
void Textbox::clamp_str(App& app) {
	int x_end_rel = x_str_rel + str_wh(str, ft, 0).x;
	if (x_end_rel < max_x_cs_rel()) {
		x_str_rel += max_x_cs_rel() - x_end_rel;
	}
	if (x_str_rel > w_edit_mg) { x_str_rel = w_edit_mg; }
}

void Textbox::render_main(App& app) {
	dvec tl_txt = tl + dvec(0, (h - ft.h) / 2);
	draw_str(scr, dscr, dep, txt, c_txt, ft, tl_txt, 0, vp);

	dcol const& c =
		!enabled ? c_invalid :
		edit ? c_edit :
		rhvd ? c_hovered : c_normal;
	draw_px_rect_framed_raw(scr, tl_box(), w, h, vp, c, c_frame);

	int x_cur = 0;
	dvec tl_str = tl_box() + dvec(x_str_rel, (h - ft.h) / 2);
	draw_str(scr, dscr, dep, left(), c_str, ft, tl_str, x_cur, 0, vp_show_txt());
	draw_str(scr, dscr, dep, mid(), c_selected, ft, tl_str, x_cur, 0, vp_show_txt());
	draw_str(scr, dscr, dep, right(), c_str, ft, tl_str, x_cur, 0, vp_show_txt());
}
void Textbox::render_cursor(App& app) {
	bool show_cursor = edit &&
		(sin(dur * twinkle_mtp) > 0 || cursor_change);
	if (show_cursor) {
		int x_cs = tl_box().x + x_cs_rel(app) - w_cursor / 2;
		int y_cs = tl_box().y + (h - h_cursor) / 2;
		draw_rect_raw(scr, { x_cs, y_cs }, w_cursor, h_cursor, vp_show_txt(), c_cursor);
	}
}
void Textbox::render(App& app) {
	render_main(app); render_cursor(app);
}

void Textbox::Update(App& app) {
	hovered = (hvd == this);
	w_txt = str_wh(txt, ft, 0).x;
	if (edit) {
		if (selecting) { select_b(app); }
		if (enabled && hovered && msc(0)) { select_a(app); }
		if (!msd[0]) { selecting = false; }
		hdl_key(app); add_chs(app); clamp_cursor(app); Upload(app);

		if (enabled && !hovered && msc(0)) { OnDone(app); }
		if (!enabled || !hovered && msc(0)) {
			// 请注意 kb_owner 的登记在 PreUpdate 中进行。
			free_kb(this); edit = selecting = false;
			edit_a = edit_b = 0;
		}
	} else {
		Sync(app);
		if (enabled && hovered && msc(0)) {
			if (clr_upon_edit) { str.clear(); }
			select_a(app); edit = true;
		}
	} clamp_str(app); render(app);
}
void Textbox::Discard(App& app) {
	rmv; free_kb(this);
	w_txt = 0; edit = hovered = false;
	selecting = cursor_change = false;
	edit_a = edit_b = 0;
}
void Textbox::PreUpdate(App& app) {
	if (edit) { kb = this; }

	bool ok = dhv <= dep &&
		insd(msp, { tl_box(), w, h }) && insd(msp, vp);
	if (ok) { dhv = dep; hvd = this; }
}

TbLan::TbLan(wstring const& id, int w) : Textbox(w), id_txt(id) {}
void TbLan::Update(App& app) { txt = loc(id_txt); Textbox::Update(app); }
