#include "font.h"

font::font() : font(20) {}
font::font(int h) : h(h) {}
tile const& font::operator[](wchar_t c) const {
	auto it = fts.find(c);
	return it == fts.end() ? missing : it->second;
}
wstring font::rnd(int n) const {
	wstring s;
	rep(i, 0, n) { s += chi[drnd(chi.size())]; } return s;
}

bool font::load_font(wstring const& name, wchar_t c) {
	wstring path = L"./fonts/" + name + L".tile";
	bool ret = false; tile t0(path, &ret);
	if (ret) {
		tile t1 = tile(t0.w * h / t0.h, h, t0, t0.rect());
		fts[c] = t1; return true;
	}
	else { return false; }
}
bool font::init() {
	wstring sym = (wstring)NUMBERS + NORMAL_SYMBOLS + LOWERS;
	wstring upp = (wstring)UPPERS;
	wstring cha = (wstring)CHINESE_CHARS + ADDITIONAL_CHINESE_CHARS;

	load_font(L"less", L'<');
	load_font(L"quote", L'\"');
	load_font(L"blank", L' ');
	load_font(L"slash", L'/');
	load_font(L"colon", L':');
	load_font(L"greater", L'>');
	load_font(L"vertical", L'|');
	load_font(L"asterisk", L'*');
	load_font(L"question", L'?');
	load_font(L"backslash", L'\\');
	for (auto c : sym) { load_font({ c }, c); }
	for (auto c : upp) { load_font({ c, c }, c); }
	for (auto c : cha) {
		if (load_font({ c }, c)) { chi.push_back(c); }
	}

	wstring path = L"./fonts/missing.tile";
	bool ret = false; tile t0(path, &ret);
	if (ret) {
		tile t1 = tile(t0.w * h / t0.h, h, t0, t0.rect());
		missing = t1; return true;
	}
	else { return false; }
}
