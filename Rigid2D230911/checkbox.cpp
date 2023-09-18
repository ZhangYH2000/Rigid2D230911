#include "checkbox.h"
#include "app.h"
#include "draw_geo.h"
#include "draw_str.h"

#include "ctrl_def.h"

Checkbox::Checkbox(bool big) {
	h = big ? 30 : 20;
	gap = { 10, 10 }; dep = 1000;
	s_box = 20; s_box_in = 10;

	c_txt = { 255, 255, 255 };
	c_frame = { 150, 150, 150 };
	c_inner = { 0, 255, 0 };
	c_normal = { 80, 80, 80 };
	c_invalid = { 0, 0, 0 };
	c_hovered = { 20, 20, 255 };
}
// 这里会利用到空格的宽度刚好等于 gap.x。
// 否则的话应该对 txt 是否为空做讨论，这里怕麻烦不管了。
int Checkbox::GetW() const { return s_box + gap.x + w_txt; }
int Checkbox::GetH() const { return h + gap.y; }

dvec Checkbox::tl_box() const { return tl + dvec(w_txt, (h - s_box) / 2); }
void Checkbox::render(App& app) {
	dvec tl_txt = tl + dvec(0, (h - ft.h) / 2);
	draw_str(scr, dscr, dep, txt, c_txt, ft, tl_txt, 0, vp);

	dcol const& c =
		!enabled ? c_invalid :
		rhvd ? c_hovered : c_normal;
	draw_rect_raw(scr, tl_box(), s_box, s_box, vp, c);
	if (checked) {
		int hs = (s_box - s_box_in) / 2;
		dvec tl_box_in = tl_box() + dvec(hs, hs);
		draw_rect_raw(scr, tl_box_in, s_box_in, s_box_in, vp, c_inner);
	}
}

void Checkbox::Update(App& app) {
	hovered = (hvd == this);
	w_txt = str_wh(txt, ft, 0).x;
	bool click = enabled && hovered && msc(0);
	if (click) { checked = !checked; Upload(app); }
	else { Sync(app); } render(app);
}
void Checkbox::Discard(App& app) { 
	rmv; w_txt = 0;
	hovered = false; 
}
void Checkbox::PreUpdate(App& app) {
	bool ok = dhv <= dep &&
		insd(msp, { tl_box(), s_box, s_box }) && insd(msp, vp);
	if (ok) { dhv = dep; hvd = this; }
}

CbLan::CbLan(wstring const& id, bool big) : Checkbox(big), id_txt(id) {}
void CbLan::Update(App& app) { txt = loc(id_txt); Checkbox::Update(app); }
