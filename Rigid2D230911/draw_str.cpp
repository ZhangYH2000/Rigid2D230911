#include "draw_str.h"
#include "font.h"

void draw_char(tile& dest, dbuf& ds, double dep,
	dvec tl, drect vp_dest, tile const& src, dcol col) {
	int a = max(vp_dest.left(), tl.x);
	int b = min(vp_dest.right(), tl.x + src.w);
	int c = max(vp_dest.top(), tl.y);
	int d = min(vp_dest.bottom(), tl.y + src.h);
	if (a >= b || c >= d) { return; }

	rep(j, c, d) {
		int dp = j * dest.w + a;
		int sp = (j - tl.y) * src.w + a - tl.x;
		rep(i, a, b) {
			if (ds[dp] <= dep && src.as[sp] != 0) {
				ds[dp] = dep; dest.cols[dp] = col;
			} dp++; sp++;
		}
	}
}
void draw_str(tile& dest, dbuf& ds, double d, wstring const& s,
	dcol col, font const& ft, dvec& tl_cur, int& x_cur, int w, drect vp) {
	for (auto c : s) {
		if (c == '\n') { tl_cur.y += ft.h; x_cur = 0; }
		else {
			tile const& t = ft[c];
			if (w != 0 && x_cur > 0 && x_cur + t.w > w) {
				tl_cur.y += ft.h; x_cur = 0;
			} 
			dvec tmp = tl_cur + dvec(x_cur, 0);
			draw_char(dest, ds, d, tmp, vp, t, col); x_cur += t.w;
		}
	}
}
void draw_str(tile& dest, dbuf& ds, double d, wstring const& s,
	dcol c, font const& ft, dvec tl, int w, drect vp) {
	int x_cur = 0;
	draw_str(dest, ds, d, s, c, ft, tl, x_cur, w, vp);
}

dvec str_wh(wstring const& s, font const& ft, int w, int* x_last) {
	int x_cur = 0, w_out = 0, h_out = ft.h;
	for (auto c : s) {
		if (c == L'\n') {
			w_out = max(w_out, x_cur);
			h_out += ft.h; x_cur = 0;
		} else {
			tile const& t = ft[c];
			if (w != 0 && x_cur > 0 && x_cur + t.w > w) {
				w_out = max(w_out, x_cur);
				h_out += ft.h; x_cur = 0;
			}
			// 下面两个顺序可别乱动啊。
			x_cur += t.w; w_out = max(w_out, x_cur);
		}
	} 
	if (x_last) { *x_last = x_cur; } 
	return { w_out, h_out };
}
int str_index_at_pos(wstring const& s, font const& ft, dvec p, int w) {
	int x_cur = 0, y_cur = 0;
	if (0 > p.y) { return 0; }

	rep(ind, 0, s.size()) {
		wchar_t c = s[ind];
		if (c == L'\n') {
			y_cur += ft.h; x_cur = 0;
			if (y_cur > p.y) { return ind; }
		} else {
			tile const& t = ft[c];
			if (w != 0 && x_cur > 0 && x_cur + t.w > w) {
				y_cur += ft.h; x_cur = 0;
				if (y_cur > p.y) { return ind; }
			} x_cur += t.w;
			if (x_cur > p.x && y_cur + ft.h > p.y) {
				return x_cur - p.x > t.w / 2 ? ind : ind + 1;
			}
		}
	} return s.size();
}
