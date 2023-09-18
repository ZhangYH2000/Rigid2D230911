#pragma once
#include "utils.h"

struct token {
	wstring typ;
	double num = 0;
	wstring str;
	wstring var;
	bool global = false;

	token() = default;
	token(wstring const& typ);
	token(double num);
	static token mkstr(wstring const& str);
	static token mkvar(wstring const& var, bool gl);
};
wstring tw(token const& tk);

token get_tk_num(wstring const& in, int& id);
token get_tk_str(wstring const& in, int& id);
token get_tk_mark(wstring const& in, int& id);
token get_tk_gl_var(wstring const& in, int& id);
token get_tk_var_res(wstring const& in, int& id);
token get_token(wstring const& in, int& id);

vector<token> deal_sgn(vector<token> const& in);
vector<token> tokenize(wstring const& in);
