#include "expr.h"
#include "bra.h"

expr::expr(wstring const& typ) : typ(typ) {}
expr::expr(double num) : num(num) { typ = L"num"; }
expr::expr(wstring const& fun, vector<expr> const& pars) :
	fun(fun), pars(pars) {
	typ = L"call";
}
expr expr::mkstr(wstring const& str) {
	auto ex = expr(L"str");
	ex.str = str; return ex;
}
expr expr::mkvar(wstring const& var, bool gl) {
	auto ex = expr(L"var");
	ex.var = var; ex.global = gl; return ex;
}

wstring tw(expr const& ex) {
	wstring tmp = L"[" + ex.typ;
	if (ex.typ == L"var") { tmp += (ex.global ? L":&" : L":") + ex.var; }
	else if (ex.typ == L"num") { tmp += L":" + tw2(ex.num); }
	else if (ex.typ == L"str") { tmp += L":" + ex.str; }
	else if (ex.typ == L"access") {
		if (!ex.loc || !ex.ind) { return {}; }
		tmp += L":" + tw(*ex.loc) + L"," + tw(*ex.ind);
	}
	else if (ex.typ == L"vec") {
		tmp += L":";
		for (auto const& v : ex.vec) { tmp += tw(v) + L","; }
	}
	else if (ex.typ == L"call") {
		tmp += L":" + ex.fun + L",";
		for (auto const& par : ex.pars) { tmp += tw(par) + L","; }
	} 
	tmp += L"]"; return tmp;
}

pre_expr::pre_expr(expr const& ex) : ex(ex) {}
pre_expr::pre_expr(token const& tk) : tk(tk) { istk = true; }

bool is_uni_operator(token const& tk) {
	// 由于 +，- 的特殊性，它们都被当成二元运算符处理。
	if (tk.typ.empty()) { return false; }
	auto const& s = tk.typ;
	return s == L"!";
}
bool is_bin_operator(token const& tk) {
	if (tk.typ.empty()) { return false; }
	return !iswalnum(tk.typ[0]) && !is_uni_operator(tk);
}

bool parsing_neg(vector<pre_expr> const& in, expr& out) {
	rep(i, 0, in.size()) {
		auto const &pre = in[i]; 
		if (!pre.istk) { continue; }
		auto const &s = pre.tk.typ;

		if (s == L"+" || s == L"-")
		if (i == 0 || in[i - 1].istk) {
			bool bad =
				i == in.size() - 1 || in[i + 1].istk;
			if (bad) { out = expr(L"error"); return false; }

			auto tmp = vector<pre_expr>(in.begin(), in.begin() + i);
			tmp.push_back(expr(s + s, { in[i + 1].ex }));
			rep(j, i + 2, in.size()) { tmp.push_back(in[j]); }
			out = parsing_pre_expr(tmp); return true;
		}
	} return false;
}
bool parsing_bin(vector<pre_expr> const& in, expr& out, vector<wstring> const& mark) {
	rep(i, 0, in.size()) {
		auto const& pre = in[i];
		if (!pre.istk) { continue; }
		auto const& s = pre.tk.typ;

		if (find(mark.begin(), mark.end(), s) != mark.end()) {
			bool bad =
				i == 0 || i == in.size() - 1 ||
				in[i - 1].istk || in[i + 1].istk;
			if (bad) { out = expr(L"error"); return false; }

			auto tmp = vector<pre_expr>(in.begin(), in.begin() + i - 1);
			tmp.push_back(expr(s, { in[i - 1].ex, in[i + 1].ex }));
			rep(j, i + 2, in.size()) { tmp.push_back(in[j]); }
			out = parsing_pre_expr(tmp); return true;
		}
	} return false;
}
expr parsing_pre_expr(vector<pre_expr> const& in) {
	if (in.empty()) { return expr(L"error"); }
	if (in.size() == 1) {
		if (in[0].istk) { return expr(L"error"); } 
		return in[0].ex;
	}

	expr out;
	if (parsing_neg(in, out)) { return out; }
	if (parsing_bin(in, out, { L"*", L"/" })) { return out; }
	if (parsing_bin(in, out, { L"+", L"-" })) { return out; }
	if (parsing_bin(in, out,
		{ L"==", L"!=", L"<=", L">=", L"<", L">" })) { return out; }
	if (parsing_bin(in, out, { L"&&", L"||" })) { return out; }
	if (parsing_bin(in, out, { L"." })) { return out; }
	return expr(L"error");
}

expr parsing_call(vector<bra> const& in) {
	expr ex(L"call"); 
	ex.fun = in[0].tk.var; 
	ex.global = in[0].tk.global;

	vector<bra> tmp;
	for (auto const& b : in[1].bras) {
		if (b.tk.typ == L",") {
			ex.pars.push_back(parsing_expr(tmp)); 
			tmp.clear();
		} else { tmp.push_back(b); }
	} 
	if (!tmp.empty()) { ex.pars.push_back(parsing_expr(tmp)); }
	return ex;
}
expr parsing_bins(vector<bra> const& in) {
	vector<bra> tmp; vector<pre_expr> pres;
	for (auto const& b : in) {
		if (is_bin_operator(b.tk)) {
			if (!tmp.empty()) {
				pres.push_back(parsing_expr(tmp)); 
				tmp.clear();
			} pres.push_back(b.tk);
		} else { tmp.push_back(b); }
	}
	if (!tmp.empty()) { pres.push_back(parsing_expr(tmp)); }
	return parsing_pre_expr(pres);
}
expr parsing_vec(vector<bra> const& in) {
	expr ex(L"vec"); vector<bra> tmp;
	for (auto const& b : in[0].bras) {
		if (b.tk.typ == L",") {
			ex.vec.push_back(parsing_expr(tmp)); 
			tmp.clear();
		} else { tmp.push_back(b); }
	}
	if (!tmp.empty()) { ex.vec.push_back(parsing_expr(tmp)); }
	return ex;
}
expr parsing_access(vector<bra> const& in) {
	expr ex(L"access");
	ex.loc = parsing_expr(vector<bra>(in.begin(), in.end() - 1));
	ex.ind = parsing_expr(in.back().bras); return ex;
}

expr parsing_expr(vector<bra> const& in) {
	if (in.empty()) { return expr(L"error"); }
	auto const& b = in[0];
	if (in.size() == 1) {
		if (b.typ == 0) {
			if (b.tk.typ == L"var") { return expr::mkvar(b.tk.var, b.tk.global); }
			else if (b.tk.typ == L"str") { return expr::mkstr(b.tk.str); }
			else if (b.tk.typ == L"num") { return expr(b.tk.num); }
			else if (b.tk.typ == L"null") { return expr(L"null"); }
			else { return expr(L"error"); }
		}
		else if (b.typ == 1) { return parsing_expr(b.bras); }
		else if (b.typ == 3) { return parsing_vec(in); }
		else { return expr(L"error"); }
	}

	bool call = in.size() == 2 &&
		in[0].tk.typ == L"var" && in[1].typ == 1;
	if (call) { return parsing_call(in); }

	bool uni = in.size() == 2 && is_uni_operator(b.tk);
	if (uni) { return expr(b.tk.typ, { parsing_expr({ in[1] }) }); }

	for (auto const& br : in)
	if (is_bin_operator(br.tk)) { return parsing_bins(in); }

	if (in.back().typ == 3) { return parsing_access(in); }
	return expr(L"error");
}
