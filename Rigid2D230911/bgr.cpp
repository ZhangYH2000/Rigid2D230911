#include "bgr.h"
#include "cur.h"
#include "draw_str.h"
#include "draw_tile.h"

Bgr::Bgr(Cur& cur) {
	w = 1630; h = 860;
	tl = { 170, 0 }; dep = -100000;
	black = tile(w, h, dcol{}, 255);
}

#include "my_def.h"

void Bgr::render(Cur& cur) {
	draw_tile_raw(scr, tl, scr.rect(), black, black.rect());
	draw_str(scr, dscr, 999, dbstr,
		dcol(255), ft, tl + dvec(10, 10), w - 20, bgr.vp());
}

void Bgr::Update(Cur& cur) {
	hovered = (hvd == this);
	wheeled = (whd == this);

	if (hovered && msc(0)) { 
		cur.body_sel = NULL; 
		cur.con_sel = NULL; 
	}
	render(cur);
}
void Bgr::PreUpdate(Cur& cur) {
	bool ok = dhv <= dep && insd(msp, vp());
	if (ok) { dhv = dep; hvd = this; }

	ok = dwh <= dep && insd(msp, vp());
	if (ok) { dwh = dep; whd = this; }
}
