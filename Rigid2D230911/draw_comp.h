#pragma once
#include "vec2.h"
#include "tile.h"

void draw_px_rect_framed_raw
(tile& dest, dvec tl, int w, int h, drect vp, dcol c, dcol fc);
void draw_px_rect_frame(tile& dest, dbuf& ds, double d,
	dvec tl, int w, int h, drect vp, dcol c);
void draw_px_rect_framed(tile& dest, dbuf& ds, double d,
	dvec tl, int w, int h, drect vp, dcol c, dcol fc);
