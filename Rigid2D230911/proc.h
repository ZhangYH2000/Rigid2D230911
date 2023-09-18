#pragma once
#include "expr.h"

struct proc {
	wstring typ = L"empty";
	// assign, single, define, return, if, while
	expr ret;
	expr single;
	expr left, right;

	wstring def_fun;
	vector<proc> procs;
	vector<wstring> def_pars;

	expr lp_con;
	vector<proc> lp;
	expr if_con;
	vector<proc> if_procs, else_procs;

	proc() = default;
	proc(wstring const& typ);
};
wstring tw(proc const& p);

proc parsing_def(vector<bra> const& in);
proc parsing_ret(vector<bra> const& in);
proc parsing_assign(vector<bra> const& in);
proc parsing_if(vector<bra> const& in);
proc parsing_while(vector<bra> const& in);

proc parsing_proc(vector<bra> const& in);
vector<proc> parsing_procs(vector<bra> const& in);
