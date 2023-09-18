#include "draw_geo.h"
#include "draw_px_seg.h"

void draw_rect_raw(tile& dest, dvec tl, int w, int h, drect vp, dcol col) {
	int a = max(vp.left(), tl.x);
	int b = min(vp.right(), tl.x + w);
	int c = max(vp.top(), tl.y);
	int d = min(vp.bottom(), tl.y + h);
	if (a >= b || c >= d) { return; }

	vector<dcol> tmp(b - a, col);
	rep(j, c, d) {
		int dp = j * dest.w + a;
		memcpy(dest.cols.data() + dp, tmp.data(), (b - a) * sizeof(col));
	}
}
void draw_rect(tile& dest, dbuf& ds, double dep,
	dvec tl, int w, int h, drect vp, dcol col) {
	int a = max(vp.left(), tl.x);
	int b = min(vp.right(), tl.x + w);
	int c = max(vp.top(), tl.y);
	int d = min(vp.bottom(), tl.y + h);
	if (a >= b || c >= d) { return; }

	rep(i, a, b) rep(j, c, d) {
		int dp = j * dest.w + i;
		if (ds[dp] <= dep) { ds[dp] = dep; dest.cols[dp] = col; }
	}
}
void draw_rect_tsf(tile& dest, dbuf& ds, double dep,
	vec2 ct, double w, double h, mat2 tsf, drect vp, dcol col) {
	mat2 inv_tsf = tsf.inv();
	vec2 hwh_tsf = tsf.abs() * vec2(w, h) / 2;
	// 这个 1 是经验公式。我没做严格论证。
	int a = max(vp.left(), int(ct.x - hwh_tsf.x) - 1);
	int b = min(vp.right(), int(ct.x + hwh_tsf.x) + 1);
	int c = max(vp.top(), int(ct.y - hwh_tsf.y) - 1);
	int d = min(vp.bottom(), int(ct.y + hwh_tsf.y) + 1);
	if (a >= b || c >= d) { return; }

	rep(i, a, b) rep(j, c, d) {
		dvec d_pnt(i, j);
		// 下面式子的正确性从逆变换比较容易看，要注意四则运算的优先级。
		vec2 u_pnt = vec2(0.5) +
			inv_tsf * (vec2(d_pnt) - ct) / vec2(w, h);
		if (insd(u_pnt, { 1, 1 })) {
			int dp = d_pnt.y * dest.w + d_pnt.x;
			if (ds[dp] <= dep) { ds[dp] = dep; dest.cols[dp] = col; }
		}
	}
}
void draw_eclipse(tile& dest, dbuf& ds, double dep,
	vec2 ct, double ax, double ay, drect vp, dcol col) {
	int a = max(vp.left(), int(ct.x - ax) - 1);
	int b = min(vp.right(), int(ct.x + ax) + 1);
	int c = max(vp.top(), int(ct.y - ay) - 1);
	int d = min(vp.bottom(), int(ct.y + ay) + 1);
	double inv_aax = 1 / (ax * ax);
	double inv_aay = 1 / (ay * ay);

	rep(i, a, b) rep(j, c, d) {
		vec2 v = vec2(i, j) - ct;
		double tmp =
			v.x * v.x * inv_aax +
			v.y * v.y * inv_aay;
		if (tmp < 1) {
			int dp = j * dest.w + i;
			if (ds[dp] <= dep) {
				ds[dp] = dep; dest.cols[dp] = col;
			}
		}
	}
}
