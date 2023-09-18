#pragma once
#include "utils.h"

struct param {
	wstring nm;
	double val = 0;
	bool del = false;

	param(wstring const& nm = {});
	void save(FILE* f);
	param(FILE* f);
};
