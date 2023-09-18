#include "bra.h"

wstring tw(bra const& b) {
	wstring tmp;
	if (b.typ == 0) { tmp += tw(b.tk); }
	else {
		tmp += tw(b.typ) + L" {";
		for (auto const& bb : b.bras) { tmp += tw(bb) + L", "; }
		tmp += L"}";
	} return tmp;
}

vector<bra> bra_analysis(vector<token> const& in) {
	vector<bra> out;
	int bra1 = 0, bra2 = 0, bra3 = 0;
	bra b; vector<token> tmp; bool zero = true;
	for (auto const& tk : in) {
		if (tk.typ == L"(") {
			++bra1; if (!zero) { tmp.push_back(tk); }
			zero = false; continue;
		} else if (tk.typ == L"{") {
			++bra2; if (!zero) { tmp.push_back(tk); }
			zero = false; continue;
		} else if (tk.typ == L"[") {
			++bra3; if (!zero) { tmp.push_back(tk); }
			zero = false; continue;
		}
		else if (tk.typ == L")") { --bra1; b.typ = 1; }
		else if (tk.typ == L"}") { --bra2; b.typ = 2; }
		else if (tk.typ == L"]") { --bra3; b.typ = 3; }
		else { b.typ = 0; }

		zero = bra1 == 0 && bra2 == 0 && bra3 == 0;
		if (zero) {
			if (b.typ) { b.bras = bra_analysis(tmp); tmp.clear(); } 
			else { b.tk = tk; } out.push_back(b); b = {};
			// 其它不相关的地方需要是零值，后面为了省事有时会利用这一点。
		} else { tmp.push_back(tk); }
	} return zero ? out : vector<bra>();
}
