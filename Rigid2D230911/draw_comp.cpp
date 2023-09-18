#include "draw_comp.h"
#include "draw_geo.h"

void draw_px_rect_framed_raw
(tile& dest, dvec tl, int w, int h, drect vp, dcol c, dcol fc) {
	draw_rect_raw(dest, tl, w, h, vp, c);
	dvec p0 = tl;
	dvec p1 = tl + dvec(w, 0);
	dvec p2 = tl + dvec(0, h);

	draw_rect_raw(dest, p0, w, 1, vp, fc);
	draw_rect_raw(dest, p2, w, 1, vp, fc);
	draw_rect_raw(dest, p0, 1, h, vp, fc);
	draw_rect_raw(dest, p1, 1, h, vp, fc);
}
void draw_px_rect_frame(tile& dest, dbuf& ds, double d,
	dvec tl, int w, int h, drect vp, dcol c) {
	dvec p0 = tl;
	dvec p1 = tl + dvec(w, 0);
	dvec p2 = tl + dvec(0, h);

	draw_rect(dest, ds, d, p0, w, 1, vp, c);
	draw_rect(dest, ds, d, p2, w, 1, vp, c);
	draw_rect(dest, ds, d, p0, 1, h, vp, c);
	draw_rect(dest, ds, d, p1, 1, h, vp, c);
}
void draw_px_rect_framed(tile& dest, dbuf& ds, double d,
	dvec tl, int w, int h, drect vp, dcol c, dcol fc) {
	draw_rect(dest, ds, d, tl, w, h, vp, c);
	draw_px_rect_frame(dest, ds, d, tl, w, h, vp, fc);
}
