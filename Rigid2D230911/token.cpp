#include "token.h"

token::token(wstring const& typ) : typ(typ) {}
token::token(double num) : num(num) { typ = L"num"; }
token token::mkstr(wstring const& str) {
	token tk(L"str");
	tk.str = str; return tk;
}
token token::mkvar(wstring const& var, bool gl) {
	token tk(L"var");
	tk.var = var; tk.global = gl; return tk;
}
wstring tw(token const& tk) {
	wstring tmp = L"[" + tk.typ;
	if (tk.typ == L"var") { tmp += (tk.global ? L":&" : L":") + tk.var; }
	if (tk.typ == L"num") { tmp += L":" + tw2(tk.num); }
	if (tk.typ == L"str") { tmp += L":" + tk.str; }
	tmp += L"]"; return tmp;
}

token get_tk_num(wstring const& in, int& id) {
	wstring tmp = { in[id] }; bool ok = true;
	bool dot = false;
	while (ok && ++id != in.size()) {
		auto c = in[id];
		if (c == L'.') {
			if (dot) { return token(L"error"); } 
			dot = true;
		}
		ok = iswdigit(c) || c == L'.';
		if (ok) { tmp += c; }
	}
	auto c = in[id];
	if (isalpha(c) || c == L'_') { return token(L"error"); }
	return token(wtof(tmp));
}
token get_tk_str(wstring const& in, int& id) {
	wstring tmp; bool ok = true;
	bool escape = false;
	while (ok && ++id != in.size()) {
		auto c = in[id];
		if (c == L'\n') { return token(L"error"); }
		if (escape) {
			if (c == L'n') { c = L'\n'; }
			escape = false; ok = true;
		} else { escape = (c == L'\\'); ok = (c != L'\"'); }
		if (ok && !escape) { tmp += c; }
	} ++id;
	return token::mkstr(tmp);
}
token get_tk_mark(wstring const& in, int& id) {
	vector<wstring> two = {
		L"||", L"&&", L"==", L"!=", L"<=", L">=", L"//"
	};
	if (id + 1 < in.size())
	for (auto const& s : two)
	if (in[id] == s[0] && in[id + 1] == s[1]) { id += 2; return token(s); }

	if (id < in.size())
	for (auto c : (wstring)L",.+-*/=<>!(){}[]&; \n\"")
	if (in[id] == c) { ++id; return token(wstring({ c })); }
	return token(L"error");
}
token get_tk_gl_var(wstring const& in, int& id) {
	wstring tmp; bool ok = true;
	// 下面这行判断的顺序很重要，ok 不能移到右边。
	while (ok && ++id != in.size()) {
		auto c = in[id];
		ok = iswalpha(c) || c == L'_' || iswdigit(c);
		if (ok) { tmp += c; }
	} return token::mkvar(tmp, true);
}
token get_tk_var_res(wstring const& in, int& id) {
	wstring tmp = { in[id] }; bool ok = true;
	// 下面这行判断的顺序很重要，ok 不能移到右边。
	while (ok && ++id != in.size()) {
		auto c = in[id];
		ok = iswalpha(c) || c == L'_' || iswdigit(c);
		if (ok) { tmp += c; }
	}
	bool reserved =
		tmp == L"def" || tmp == L"return" || tmp == L"if" ||
		tmp == L"else" || tmp == L"while" || tmp == L"null";
	if (reserved) { return token(tmp); }
	return token::mkvar(tmp, false);
}
token get_token(wstring const& in, int& id) {
	auto c = in[id];
	if (c == L'&') { return get_tk_gl_var(in, id); }
	if (iswalpha(c) || c == L'_') { return get_tk_var_res(in, id); }
	if (iswdigit(c)) { return get_tk_num(in, id); }
	if (c == L'\"') { return get_tk_str(in, id); }
	return get_tk_mark(in, id);
}

vector<token> deal_sgn(vector<token> const& in) {
	vector<token> out;
	bool add = false; int sgn = 1;

	for (auto const& tk : in) {
		bool old_add = add;
		add = tk.typ == L"+" || tk.typ == L"-";
		if (add) {
			if (old_add) { sgn *= tk.typ == L"+" ? 1 : -1; }
			else { sgn = tk.typ == L"+" ? 1 : -1; } continue;
		} else if (old_add) {
			out.push_back(token(sgn == 1 ? L"+" : L"-"));
		} out.push_back(tk);
	}
	if (add) { out.push_back(token(sgn == 1 ? L"+" : L"-")); }
	return out;
}
vector<token> tokenize(wstring const& in) {
	int id = 0; vector<token> tmp;
	while (id != in.size()) {
		// 你之前在 tk 前面加了 const &，千万不要在这样了。
		auto tk = get_token(in, id);
		bool blank =
			tk.typ == L" " || tk.typ == L"\n";
		if (tk.typ == L"//") {
			while (id != in.size() && in[id++] != L'\n');
		}
		else if (tk.typ == L"error") { return {}; }
		else if (!blank) { tmp.push_back(tk); }
	} return deal_sgn(tmp);
}
