#include "tile.h"

dcol::dcol(wstring const& s) {
	int i = 0;
	r = fetch_num(i, s);
	g = fetch_num(i, s);
	b = fetch_num(i, s);
}
wstring tw(dcol c) {
	return L"(" + tw(c.r) + L"," + tw(c.g) + L"," + tw(c.b) + L")";
}
void cover(dcol& bc, BYTE& ba, dcol fc, BYTE fa) {
	// 目前还不会推导公式，应该有简化的空间。
	if (fa == 0) { return; }
	if (fa == 255) { bc = fc; ba = 255; return; }

	int a = 255 * (ba + fa) - ba * fa;
	int r = (bc.r * ba * (255 - fa) + fc.r * fa * 255) / a;
	int g = (bc.g * ba * (255 - fa) + fc.g * fa * 255) / a;
	int b = (bc.b * ba * (255 - fa) + fc.b * fa * 255) / a; a /= 255;
	bc = dcol(r, g, b); ba = a;
}

tile::tile(int w, int h, dcol c, BYTE a) :
	w(w), h(h), cols(n(), c), as(n(), a) {}
tile::tile(int w, int h, tile const& src, drect vp_src) : tile(w, h) {
	rep(i, 0, w) rep(j, 0, h) {
		dvec d_pnt(i, j);
		dvec s_pnt = d_pnt * dvec(vp_src.w, vp_src.h) / dvec(w, h);

		int dp = d_pnt.y * w + d_pnt.x;
		int sp = (s_pnt.y + vp_src.top()) * src.w + s_pnt.x + vp_src.left();
		cols[dp] = src.cols[sp]; as[dp] = src.as[sp];
	}
}
void tile::save(wstring const& nm) const {
	FILE* f = wfopen(nm, L"wb");
	if (!f) { return; }
	fwt(w); fwt(h);
	fwts(&cols[0], n()); fwts(&as[0], n()); fclose(f);
}
tile::tile(wstring const& nm, bool* ok) {
	FILE* f = wfopen(nm, L"rb");
	if (ok) { *ok = f; }
	if (!f) { return; }
	frd(w); frd(h);
	cols.resize(w * h); as.resize(w * h);
	frds(&cols[0], n()); frds(&as[0], n()); fclose(f);
}

int tile::n() const { return w * h; }
drect tile::rect() const { return { w, h }; }
void tile::fill(dcol c, BYTE a) {
	rep(i, 0, n()) { cols[i] = c; } 
	memset(as.data(), a, n());
}
