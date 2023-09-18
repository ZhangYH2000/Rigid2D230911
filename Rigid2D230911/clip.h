#pragma once
#include "utils.h"

struct clip {
	int sp = 0;
	int csp = 0;
	bool replay = false;
	vector<double> vs;

	clip(wstring const& nm, bool* ok = NULL);
	clip(int sp = 0);
	int n() const { return vs.size(); }
	void play(vector<double>& in, double vol = 1);
	void save(wstring const& nm) const;
};
