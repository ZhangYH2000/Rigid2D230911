#pragma once
#include "token.h"

struct bra {
	int typ = 0;
	// 1 代表 ()，2 代表 {}, 3 代表 []。
	token tk;
	vector<bra> bras;
};
wstring tw(bra const& b);
vector<bra> bra_analysis(vector<token> const& in);
