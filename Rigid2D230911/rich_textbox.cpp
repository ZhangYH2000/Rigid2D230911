#include "rich_textbox.h"
#include "app.h"
#include "draw_geo.h"
#include "draw_str.h"
#include "draw_comp.h"

#include "ctrl_def.h"

RichTextbox::RichTextbox() : RichTextbox(0, 0) {}
RichTextbox::RichTextbox(int w, int h, bool newline) : w(w), h(h) {
	gap = { 10, 10 }; dep = 1000;

	c_str = { 255, 255, 255 };
	c_edit = { 80, 40, 40 };
	c_frame = { 150, 150, 150 };
	c_normal = { 40, 40, 40 };
	c_cursor = { 255, 255, 255 };
	c_hovered = { 40, 40, 80 };
	c_invalid = { 0, 0, 0 };
	c_selected = { 150, 150, 255 };

	edit_mg = { 5, 5 };
	show_txt_mg = { 2, 2 };
	slide_mtp = 8;
	wheel_mtp = 40;
	twinkle_mtp = 8;
	w_cursor = 2; h_cursor = 20;

	if (newline) { w_str = w - 2 * edit_mg.x; }
}
int RichTextbox::GetW() const { return w + gap.x; }
int RichTextbox::GetH() const { return h + gap.y; }

drect RichTextbox::vp_show_txt() const {
	dvec tl_show_txt = tl + show_txt_mg;
	int w_show_txt = w - 2 * show_txt_mg.x;
	int h_show_txt = h - 2 * show_txt_mg.y;
	return overlap(vp, { tl_show_txt, w_show_txt, h_show_txt });
}
wstring RichTextbox::left() const {
	auto a = str.begin();
	auto b = str.begin() + edit_begin();
	return wstring(a, b);
}
wstring RichTextbox::mid() const {
	auto a = str.begin() + edit_begin();
	auto b = str.begin() + edit_end();
	return wstring(a, b);
}
wstring RichTextbox::right() const {
	auto a = str.begin() + edit_end();
	auto b = str.end();
	return wstring(a, b);
}
int RichTextbox::edit_begin() const { return min(edit_a, edit_b); }
int RichTextbox::edit_end() const { return max(edit_a, edit_b); }
bool RichTextbox::selected() const { return edit_a != edit_b; }
int RichTextbox::max_x_cs_rel() const { return w - edit_mg.x; }
int RichTextbox::max_y_cs_rel() const { return h - edit_mg.y; }
dvec RichTextbox::p_cs_rel(App& app) const {
	wstring tmp(str.begin(), str.begin() + edit_b);
	int x = 0; dvec p = str_wh(tmp, ft, w_str, &x);
	p.x = x; p.y -= ft.h;
	return (dvec)tl_str_rel + p;
}

void RichTextbox::select_a(App& app) {
	selecting = true;
	dvec p = msp - (tl + (dvec)tl_str_rel);
	edit_a = edit_b = str_index_at_pos(str, ft, p, w_str);
	update_x_cs_rel_hist(app);
}
void RichTextbox::select_b(App& app) {
	dvec p = msp - (tl + (dvec)tl_str_rel);
	edit_b = str_index_at_pos(str, ft, p, w_str);
	update_x_cs_rel_hist(app);
}
void RichTextbox::update_x_cs_rel_hist(App& app) {
	auto s = wstring(str.begin(), str.begin() + edit_b);
	int tmp = 0; str_wh(s, ft, w_str, &tmp);
	x_cs_rel_hist = tmp;
}

void RichTextbox::add_chs(App& app) {
	wstring sifted = sift_chs(chs, false);
	if (!sifted.empty()) {
		selecting = false;
		str = left() + sifted + right();
		edit_a = edit_b = edit_begin() + sifted.size();
		update_x_cs_rel_hist(app);
	}
}
void RichTextbox::hdl_left(App& app) {
	selecting = false;
	cursor_change = true;

	if (selected()) { edit_a = edit_b = edit_begin(); }
	else {
		if (--edit_a < 0) { edit_a = 0; }
		edit_b = edit_a;
	} update_x_cs_rel_hist(app);
}
void RichTextbox::hdl_right(App& app) {
	selecting = false;
	cursor_change = true;

	if (selected()) { edit_a = edit_b = edit_end(); }
	else {
		// 下面不要减一。
		if (++edit_a > str.size()) { edit_a = str.size(); }
		edit_b = edit_a;
	} update_x_cs_rel_hist(app);
}
void RichTextbox::hdl_up(App& app) {
	selecting = false;
	cursor_change = true;

	if (selected()) { edit_a = edit_b = edit_begin(); }
	else {
		wstring tmp(str.begin(), str.begin() + edit_b);
		int y = str_wh(tmp, ft, w_str).y - 2 * ft.h;
		dvec p(x_cs_rel_hist, y);
		edit_a = edit_b = str_index_at_pos(str, ft, p, w_str);
	}
}
void RichTextbox::hdl_down(App& app) {
	selecting = false;
	cursor_change = true;

	if (selected()) { edit_a = edit_b = edit_end(); }
	else {
		wstring tmp(str.begin(), str.begin() + edit_b);
		int y = str_wh(tmp, ft, w_str).y;
		dvec p(x_cs_rel_hist, y);
		edit_a = edit_b = str_index_at_pos(str, ft, p, w_str);
	}
}
void RichTextbox::hdl_back(App& app) {
	selecting = false;
	cursor_change = true;

	if (selected()) {
		str = left() + right();
		edit_a = edit_b = edit_begin();
	} else if (edit_begin() > 0) {
		auto a = str.begin();
		auto b = str.begin() + edit_a - 1;
		str = wstring(a, b) + right();
		edit_b = --edit_a;
	} update_x_cs_rel_hist(app);
}
void RichTextbox::hdl_delete(App& app) {
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
	// 按道理 delete 不用更新的。
	// update_x_cs_rel_hist(app);
}
void RichTextbox::hdl_clip(App& app) {
	selecting = false;
	set_clipboard(mid());
	str = left() + right();
	edit_a = edit_b = edit_begin();
	update_x_cs_rel_hist(app);
}
void RichTextbox::hdl_copy(App& app) { set_clipboard(mid()); }
void RichTextbox::hdl_paste(App& app) {
	selecting = false;
	wstring sifted = sift_chs(get_clipboard(), false);
	str = left() + sifted + right();
	edit_a = edit_b = edit_begin() + sifted.size();
	update_x_cs_rel_hist(app);
}
void RichTextbox::hdl_all(App& app) {
	selecting = false;
	edit_a = 0; edit_b = str.size();
	update_x_cs_rel_hist(app);
}
void RichTextbox::hdl_key(App& app) {
	cursor_change = false;

	if (kbmsg[VK_LEFT]) { hdl_left(app); }
	else if (kbmsg[VK_RIGHT]) { hdl_right(app); }
	else if (kbmsg[VK_UP]) { hdl_up(app); }
	else if (kbmsg[VK_DOWN]) { hdl_down(app); }
	else if (kbmsg[VK_DELETE]) { hdl_delete(app); }
	else if (kbmsg[VK_BACK]) { hdl_back(app); }

	if (kbd[VK_CONTROL]) {
		if (kbc('C')) { hdl_copy(app); }
		else if (kbc('X')) { hdl_clip(app); }
		else if (kbc('V')) { hdl_paste(app); }
		else if (kbc('A')) { hdl_all(app); }
	}
}

void RichTextbox::clamp_cs_selecting(App& app) {
	double val = (msp.x - tl.x) - max_x_cs_rel();
	if (val > 0) { tl_str_rel.x -= dt * slide_mtp * val; }
	val = edit_mg.x - (msp.x - tl.x);
	if (val > 0) { tl_str_rel.x += dt * slide_mtp * val; }
	val = (msp.y - tl.y) - max_y_cs_rel();
	if (val > 0) { tl_str_rel.y -= dt * slide_mtp * val; }
	val = edit_mg.y - (msp.y - tl.y);
	if (val > 0) { tl_str_rel.y += dt * slide_mtp * val; }
}
void RichTextbox::clamp_cs_not_selecting(App& app) {
	if (p_cs_rel(app).x > max_x_cs_rel()) {
		tl_str_rel.x -= p_cs_rel(app).x - max_x_cs_rel();
	} else if (p_cs_rel(app).x < edit_mg.x) {
		tl_str_rel.x += edit_mg.x - p_cs_rel(app).x;
	}
	// 我们不对 y 这么做，主要是为了让鼠标滚轮正常，其实细想挺奇怪的。
}
void RichTextbox::clamp_cursor(App& app) {
	if (selecting) { clamp_cs_selecting(app); }
	else { clamp_cs_not_selecting(app); }
}
void RichTextbox::clamp_str(App& app) {
	// 除了文字过少以外，clamp_selecting 和 wheel 后也需要这一步。
	dvec p_end_rel = (dvec)tl_str_rel + str_wh(str, ft, w_str);
	if (p_end_rel.x < max_x_cs_rel()) {
		tl_str_rel.x += max_x_cs_rel() - p_end_rel.x;
	}
	if (p_end_rel.y < max_y_cs_rel()) {
		tl_str_rel.y += max_y_cs_rel() - p_end_rel.y;
	}
	// 下面两行是配合上面的。
	if (tl_str_rel.x > edit_mg.x) { tl_str_rel.x = edit_mg.x; }
	if (tl_str_rel.y > edit_mg.y) { tl_str_rel.y = edit_mg.y; }
}

void RichTextbox::render_main(App& app) {
	dcol c =
		!enabled ? c_invalid :
		edit ? c_edit :
		rhvd ? c_hovered : c_normal;
	draw_px_rect_framed_raw(scr, tl, w, h, vp, c, c_frame);

	int x_cur = 0;
	dvec tl_txt = tl + (dvec)tl_str_rel;
	draw_str(scr, dscr, dep, left(), c_str, ft, tl_txt, x_cur, w_str, vp_show_txt());
	draw_str(scr, dscr, dep, mid(), c_selected, ft, tl_txt, x_cur, w_str, vp_show_txt());
	draw_str(scr, dscr, dep, right(), c_str, ft, tl_txt, x_cur, w_str, vp_show_txt());
}
void RichTextbox::render_cursor(App& app) {
	bool show_cursor = edit &&
		(sin(dur * twinkle_mtp) > 0 || cursor_change);
	if (show_cursor) {
		dvec tl_cs = tl + p_cs_rel(app) + dvec(-w_cursor, ft.h - h_cursor) / 2;
		draw_rect_raw(scr, tl_cs, w_cursor, h_cursor, vp_show_txt(), c_cursor);
	}
}
void RichTextbox::render(App& app) {
	render_main(app); render_cursor(app);
}

void RichTextbox::Update(App& app) {
	hovered = (hvd == this);
	if (edit) {
		// 我感觉 selecting 有关的逻辑也是有点乱的。
		// 怎么看待 UI 交互真的是个大问题。怎么清晰的表达需求。
		// 感觉还是不要钻牛角尖，这肯定不是完美的做法，事实上需求本身就很模糊。
		if (selecting) { select_b(app); }
		if (enabled && hovered && msc(0)) { select_a(app); }
		if (!msd[0]) { selecting = false; }
		hdl_key(app); add_chs(app); clamp_cursor(app); Upload(app);

		if (enabled && !hovered && msc(0)) { OnDone(app); }
		if (!enabled || !hovered && msc(0)) {
			// 请注意 kb_owner 的登记在 PreUpdate 中进行。
			free_kb(this); edit = selecting = false;
			edit_a = edit_b = 0; x_cs_rel_hist = 0;
		}
	} else {
		Sync(app);
		if (enabled && hovered && msc(0)) {
			if (clr_upon_edit) { str.clear(); }
			select_a(app); edit = true;
		}
	} 

	if (whd == this) { tl_str_rel.y += msw * wheel_mtp; }
	clamp_str(app); render(app);
}
void RichTextbox::Discard(App& app) {
	rmv; free_kb(this);
	edit = hovered = false; selecting = false;
	edit_a = edit_b = 0; x_cs_rel_hist = 0;
}
void RichTextbox::PreUpdate(App& app) {
	if (edit) { kb = this; }

	bool ok = dhv <= dep &&
		insd(msp, { tl, w, h }) && insd(msp, vp);
	if (ok) { dhv = dep; hvd = this; }

	ok = dwh <= dep &&
		insd(msp, { tl, w, h }) && insd(msp, vp);
	if (ok) { dwh = dep; whd = this; }
}
