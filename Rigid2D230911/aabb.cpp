#include "aabb.h"

aabb::aabb(aabb const& b0, aabb const& b1) {
	x0 = min(b0.x0, b1.x0);
	x1 = max(b0.x1, b1.x1);
	y0 = min(b0.y0, b1.y0);
	y1 = max(b0.y1, b1.y1);
}
void aabb::expand(vec2 v) {
	x0 = min(v.x, x0);
	x1 = max(v.x, x1);
	y0 = min(v.y, y0);
	y1 = max(v.y, y1);
}

bool overlap(aabb const& b0, aabb const& b1) {
	bool bx = max(b0.x0, b1.x0) <= min(b0.x1, b1.x1);
	bool by = max(b0.y0, b1.y0) <= min(b0.y1, b1.y1);
	return bx && by;
}
