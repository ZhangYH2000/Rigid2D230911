#include "separator.h"
#include "app.h"
#include "draw_geo.h"

#include "ctrl_def.h"

SeparatorY::SeparatorY(int w) : w(w) {
	gap = { 10, 10 };
	c_mark = { 150, 150, 150 };
}
int SeparatorY::GetW() const { return w + gap.x; }
int SeparatorY::GetH() const { return 1 + gap.y; }

void SeparatorY::render(App& app) {
	// 虽然 dep 没什么用，但出于习惯还是保留了。
	draw_rect_raw(scr, tl, w, 1, vp, c_mark);
}
void SeparatorY::Update(App& app) { render(app); }
