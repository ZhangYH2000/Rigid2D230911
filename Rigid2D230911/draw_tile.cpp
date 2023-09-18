#include "draw_tile.h"

void draw_tile_raw
(tile& dest, dvec tl, drect vp_dest, tile const& src, drect vp_src) {
	int a = max(vp_dest.left(), tl.x);
	int b = min(vp_dest.right(), tl.x + vp_src.w);
	int c = max(vp_dest.top(), tl.y);
	int d = min(vp_dest.bottom(), tl.y + vp_src.h);
	if (a >= b || c >= d) { return; }

	rep(j, c, d) {
		int dp = j * dest.w + a;
		int sp = (j - tl.y + vp_src.top()) * src.w +
			(a - tl.x + vp_src.left());
		int sz_row = sizeof(dcol) * (b - a);
		memcpy(dest.cols.data() + dp, src.cols.data() + sp, sz_row);
	}
}
void draw_tile(tile& dest, dbuf& ds, double dep, dvec tl,
	drect vp_dest, tile const& src, drect vp_src) {
	int a = max(vp_dest.left(), tl.x);
	int b = min(vp_dest.right(), tl.x + vp_src.w);
	int c = max(vp_dest.top(), tl.y);
	int d = min(vp_dest.bottom(), tl.y + vp_src.h);
	if (a >= b || c >= d) { return; }

	rep(j, c, d) {
		int dp = j * dest.w + a;
		int sp = (j - tl.y + vp_src.top()) * src.w +
			(a - tl.x + vp_src.left());
		rep(i, a, b) {
			if (ds[dp] <= dep && src.as[sp] != 0) {
				ds[dp] = dep; dest.cols[dp] = src.cols[sp];
			} dp++; sp++;
		}
	}
}
bool hit_tile(dvec p, dvec tl, tile const& src, drect vp_src) {
	dvec s_pnt = vp_src.tl + p - tl;
	int sp = s_pnt.y * src.w + s_pnt.x;
	return insd(s_pnt, vp_src) && src.as[sp] != 0;
}
