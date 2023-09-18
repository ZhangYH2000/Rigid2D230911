#include "droplist.h"
#include "app.h"
#include "draw_geo.h"
#include "draw_str.h"
#include "draw_comp.h"

#include "ctrl_def.h"

wstring nm_none(int id, App &app) { return L"空"; }
Droplist::Droplist(int w, DpNm const& nm) : w(w), nm(nm) {
	gap = { 10, 10 }; dep = 1000;
	h_item = 25; h = 30;

	sign_r = 5;
	sign_margin = 15;
	x_txt_margin = 5;

	// 懒得区分标签和内容的颜色了。
	c_txt = { 255, 255, 255 };
	c_mark = { 255, 255, 255 };
	c_edit = { 150, 60, 60 };
	c_frame = { 150, 150, 150 };
	c_normal = { 40, 40, 40 };
	c_invalid = { 0, 0, 0 };
	c_hovered = { 60, 60, 150 };
}
int Droplist::GetW() const { return w + gap.x + w_txt; }
int Droplist::GetH() const {
	return h + gap.y + (edit ? n_item * h_item : 0);
}

dvec Droplist::tl_box() const { return tl + dvec(w_txt, 0); }
dvec Droplist::tl_item() const { return tl_box() + dvec(0, h); }
void Droplist::render_main(App& app) {
	dvec tl_txt = tl + dvec(0, (h - ft.h) / 2);
	draw_str(scr, dscr, dep, txt, c_txt, ft, tl_txt, 0, vp);

	dcol const& c =
		!enabled ? c_invalid :
		edit ? c_edit :
		rhvd ? c_hovered : c_normal;
	draw_px_rect_framed(scr, dscr, dep, tl_box(), w, h, vp, c, c_frame);

	vec2 ct_sign = (vec2)tl_box() + vec2(w - sign_margin, h / 2);
	draw_eclipse(scr, dscr, dep, ct_sign, sign_r, sign_r, vp, c_mark);

	dvec tl_item = tl + dvec(x_txt_margin, (h - ft.h) / 2);
	draw_str(scr, dscr, dep, nm(picked, app), c_txt, ft, tl_txt, 0, vp);
}
void Droplist::render_items(App& app) {
	if (edit) {
		draw_px_rect_framed(scr, dscr, dep, 
			tl_item(), w, h_item * n_item, vp, c_normal, c_frame);
		if (item_hv != -1) {
			dvec tl_hovered = tl_box() + dvec(0, h + h_item * item_hv);
			draw_px_rect_framed(scr, dscr, dep,
				tl_hovered, w, h_item, vp, c_hovered, c_frame);
		}
		rep(i, 0, n_item) {
			dvec tl_txt_item = tl_box() +
				dvec(0, h + h_item * i) +
				dvec(x_txt_margin, (h_item - ft.h) / 2);
			draw_str(scr, dscr, dep, nm(i, app), c_txt, ft, tl_txt_item, 0, vp);
		}
	}
}
void Droplist::render(App& app) {
	render_main(app); render_items(app);
}

// 在内容可变的情形下怎么处理我还没想清楚的。
void Droplist::Update(App& app) {
	hovered = (hvd == this);
	w_txt = str_wh(txt, ft, 0).x;
	if (edit) {
		if (hovered) {
			int tmp = msp.y - tl_box().y - h;
			item_hv = tmp >= 0 ? tmp / h_item : -1;
		}
		if (enabled && hovered && msc(0)) {
			if (item_hv != -1) { picked = item_hv; }
			Upload(app);
		}
		if (!enabled || msc(0)) { item_hv = -1; edit = false; }
	} else {
		Sync(app);
		edit = enabled && hovered && msc(0);
	} render(app);
}
void Droplist::Discard(App& app) {
	rmv; w_txt = 0;
	edit = hovered = false; item_hv = -1;
}
void Droplist::PreUpdate(App& app) {
	int real_h = h + (edit ? n_item * h_item : 0);
	bool ok = dhv <= dep &&
		insd(msp, { tl_box(), w, real_h}) && insd(msp, vp);
	if (ok) { dhv = dep; hvd = this; }
}

DpLan::DpLan(wstring const& id, int w, DpNm const& nm) : Droplist(w, nm), id_txt(id) {}
void DpLan::Update(App& app) { txt = loc(id_txt); Droplist::Update(app); }
