#include "clip.h"

clip::clip(wstring const& nm, bool *ok) {
	FILE* f = wfopen(nm, L"rb");
	if (!f) { 
		if (ok) { *ok = false; }
		return; 
	}
	int sz = 0;
	frd(sp); frd(replay); frdv(vs);
	fclose(f); 
	if (ok) { *ok = true; }
}
clip::clip(int sp) : sp(sp) {}
void clip::play(vector<double>& in, double vol) {
	for (auto& v : in) {
		if (csp >= n()) { return; }
		v += vs[csp] * vol;
		if (++csp == n() && replay) { csp = 0; }
	}
}

void clip::save(wstring const& nm) const {
	FILE* f = wfopen(nm, L"wb");
	if (!f) { return; } 
	int sz = 0;
	fwt(sp); fwt(replay); fwtv(vs);
	fclose(f);
}
