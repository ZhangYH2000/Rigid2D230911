#include "vec2.h"

vec2::vec2(wstring const& s) {
	int i = 0;
	x = fetch_num(i, s);
	y = fetch_num(i, s);
}
wstring tw2(vec2 v) {
	return L"(" + tw2(v.x) + L"," + tw2(v.y) + L")";
}

bool insd(vec2 v, rect r) {
	return
		insd(v.x, r.left(), r.right()) &&
		insd(v.y, r.top(), r.bottom());
}

// 不知道能不能优化，同时二次和三次应该有单独的实现。
vec2 bezier(vector<vec2> ps, double t) {
	if (ps.size() == 2) { return ps[0] * (1 - t) + ps[1] * t; }
	vector<vec2> ps0, ps1;
	rep(i, 0, ps.size()) {
		if (i != 0) { ps1.push_back(ps[i]); }
		if (i != ps.size() - 1) { ps0.push_back(ps[i]); }
	}
	return bezier(ps0, t) * (1 - t) + bezier(ps1, t) * t;
}
// 这里的公式我不能熟练的推出来。
double dist_sqr(vec2 a, vec2 b, vec2 p) {
	if (a == b) { return (a - p).lensqr(); }
	double ap = (p - a).lensqr();
	double bp = (p - b).lensqr();
	double ab = (a - b).lensqr();
	double dif = ap - bp;
	return
		ab + bp < ap ? bp :
		ab + ap < bp ? ap :
		(ap + bp) / 2 - ab / 4 - dif * dif / (4 * ab);
}
