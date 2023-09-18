#pragma once
#include "vec2.h"
#include "tile.h"

void draw_rect_raw(tile& dest, dvec tl, int w, int h, drect vp, dcol col);
void draw_rect(tile& dest, dbuf& ds, double dep,
	dvec tl, int w, int h, drect vp, dcol col);
void draw_rect_tsf(tile& dest, dbuf& ds, double dep,
	vec2 ct, double w, double h, mat2 tsf, drect vp, dcol col);
void draw_eclipse(tile& dest, dbuf& ds, double dep,
	vec2 ct, double ax, double ay, drect vp, dcol col);
