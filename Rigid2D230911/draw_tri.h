#pragma once
#include "tile.h"

void sub_draw_tri(tile& dest, dbuf& ds,
	dvec pa, dvec pb, dvec pc, double d, dcol col, bool same_y);
void draw_tri(tile& dest, dbuf& ds, drect const& vp,
	dvec pa, dvec pb, dvec pc, double d, dcol col);
void draw_tri(tile& dest, dbuf& ds, drect const& vp,
	vec2 pa, vec2 pb, vec2 pc, double d, dcol col);
