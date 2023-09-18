#pragma once
#include "tile.h"

wchar_t extern CHINESE_CHARS[];
wchar_t extern NUMBERS[];
wchar_t extern NORMAL_SYMBOLS[];
wchar_t extern UPPERS[];
wchar_t extern LOWERS[];
wchar_t extern ADDITIONAL_CHINESE_CHARS[];

// 中文字体规格：画布：600x600，粗细：40，保存后：60x60。
// 由于历史原因，颜色一般是: (0,255,0)。
struct font {
	int h = 0;
	wstring chi;
	tile missing;
	map<wchar_t, tile> fts;

	font();
	font(int h);

	tile const& operator[](wchar_t c) const;
	// 严格来说用单独的方法更好，但是这样很方便。
	wstring rnd(int n) const;

	bool load_font(wstring const& name, wchar_t c);
	bool init();
};
