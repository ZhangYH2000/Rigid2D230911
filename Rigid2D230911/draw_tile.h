#pragma once
#include "vec2.h"
#include "tile.h"

// 这里 vp_dest 允许奇异，但是 vp_src 不允许奇异。
void draw_tile_raw
(tile& dest, dvec tl, drect vp_dest, tile const& src, drect vp_src);
void draw_tile(tile& dest, dbuf& ds, double dep, dvec tl,
	drect vp_dest, tile const& src, drect vp_src);
bool hit_tile(dvec p, dvec tl, tile const& src, drect vp_src);
