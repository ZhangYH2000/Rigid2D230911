#include "draw_curve.h"
#include "draw_px_seg.h"

void draw_eclipse_frame(tile& dest, dbuf& ds, double dep,
	vec2 ct, double ax, double ay, drect vp, dcol col, int n) {

	auto p0 = ct + vec2(ax, 0);
	rep(i, 0, n - 1) {
		double phi = 2 * PI * (i + 1) / n;
		auto p1 = ct + vec2(ax, ay) * vec2(cos(phi), sin(phi));
		draw_px_seg(dest, ds, p0, p1, dep, vp, col);
		p0 = p1;
	}
	auto p1 = ct + vec2(ax, 0);
	draw_px_seg(dest, ds, p0, p1, dep, vp, col);
}
