#include "par.h"

param::param(wstring const& nm) : nm(nm) {}
void param::save(FILE* f) {
	int sz = 0;
	fwtv(nm); fwt(val);
}
param::param(FILE* f) {
	int sz = 0;
	frdv(nm); frd(val);
}
