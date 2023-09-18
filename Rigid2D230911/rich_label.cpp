#include "rich_label.h"
#include "app.h"
#include "draw_str.h"

#include "ctrl_def.h"

RichLabel::RichLabel(int w) : w(w) {
	gap = { 10, 10 }; dep = 1000;
	fixed_w = true;
	c_txt = { 255, 255, 255 }; txt = L"´ó¶ÎÎÄ±¾";
}
int RichLabel::GetW() const { return w + gap.x; }
int RichLabel::GetH() const { return h + gap.y; }

int RichLabel::w_line() const { return fixed_w ? w : 0; }
void RichLabel::render(App& app) {
	draw_str(scr, dscr, dep, txt, c_txt, ft, tl, w_line(), vp);
}
void RichLabel::Update(App& app) {
	Sync(app);
	dvec wh = str_wh(txt, ft, w_line());
	if (!fixed_w) { w = wh.x; }
	if (!fixed_h) { h = wh.y; }
	render(app);
}

RLbLan::RLbLan(int w, wstring const& id) : id_txt(id), RichLabel(w) {}
void RLbLan::Sync(App& app) { txt = loc(id_txt); }
