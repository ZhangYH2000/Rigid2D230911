#pragma once
#include "vptr.h"
#include "bra.h"

// 这里空间可能占的太大了，运行时多态可以解决。但是懒得麻烦了。
struct expr {
	wstring typ = L"null";
	// vec 和 access 其实都可以放到 call 的框架里。
	// 但是实现的时候我还是希望分开来，虽然也没啥理由就是了。
	// num, var, str, call, null, vec, access

	bool global = false;
	double num = 0;
	wstring str;
	wstring var;
	wstring fun;
	vector<expr> pars;
	vector<expr> vec;
	vptr<expr> loc, ind;

	expr() = default;
	expr(wstring const& typ);
	expr(double num);
	expr(wstring const& fun, vector<expr> const& pars);
	static expr mkstr(wstring const& str);
	static expr mkvar(wstring const& var, bool gl);
};
wstring tw(expr const& ex);

struct pre_expr {
	bool istk = false;
	expr ex;
	token tk;

	pre_expr() = default;
	pre_expr(expr const& ex);
	pre_expr(token const& tk);
};

bool is_uni_operator(token const& tk);
bool is_bin_operator(token const& tk);

bool parsing_neg(vector<pre_expr> const& in, expr& out);
bool parsing_bin(vector<pre_expr> const& in, expr& out, vector<wstring> const& mark);
expr parsing_pre_expr(vector<pre_expr> const& in);

expr parsing_call(vector<bra> const& in);
expr parsing_bins(vector<bra> const& in);
expr parsing_vec(vector<bra> const& in);
expr parsing_access(vector<bra> const& in);
expr parsing_expr(vector<bra> const& in);
