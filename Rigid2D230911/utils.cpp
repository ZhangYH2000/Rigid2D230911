#include "utils.h"

// 有个大问题是代码的格式与排版到底是怎样的。
// 又想要不死板教条，又想要稳定性，又想要读起来好看，很纠结。
// 后面有些地方的逻辑实在是太乱了，点名 parse 那一块的。

int drnd(int a, int b) {
	static std::random_device rd;
	static std::mt19937 generator(rd());
	std::uniform_int_distribution<int> d(a, b);
	return d(generator);
}
int drnd(int sz) { return drnd(0, sz - 1); }
double frnd(double a, double b) {
	static std::random_device rd;
	static std::mt19937 generator(rd());
	std::uniform_real_distribution<double> d(a, b);
	return d(generator);
}
double frnd(double b) { return frnd(0, b); }
double nrnd(double mean, double stddev) {
	static std::random_device rd;
	static std::mt19937 generator(rd());
	std::normal_distribution<double> d(mean, stddev);
	return d(generator);
}

// 用 sprintf_s 可以保证安全，但不知道 1024 的空间是否足够。
wstring tw2(double d) {
	static wchar_t tmp[1024];
	swprintf_s(tmp, L"%.2lf", d); return tmp;
}
// 下面的规范应该参考 MSDN。
wstring tw(string const& s) {
	vector<wchar_t> tmp(s.size() + 1);
	swprintf_s(tmp.data(), tmp.size(), L"%S", s.c_str());
	return tmp.data();
}
string to_string(wstring const& s) {
	// 我忘了有没有核实过一定是这个大小了。
	vector<char> tmp(2 * s.size() + 2);
	sprintf_s(tmp.data(), tmp.size(), "%S", s.c_str());
	return tmp.data();
}
double atof(string const& s) { return atof(s.c_str()); }
double wtof(wstring const& s) { return atof(to_string(s)); }
// 必须用 _wfopen，否则有编码问题。
FILE* wfopen(wstring const& nm, wstring const& arg) {
	FILE* f = NULL;
	_wfopen_s(&f, nm.c_str(), arg.c_str()); return f;
}

void hide_console() { ShowWindow(GetConsoleWindow(), SW_HIDE); }
void show_console() { ShowWindow(GetConsoleWindow(), SW_SHOW); }
void hide_console_cursor() {
	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;

	GetConsoleCursorInfo(hdl, &info);
	info.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}
void print_console(wstring const& s, bool newline, int width) {
	hide_console_cursor();

	auto hdl = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hdl, &info);
	SetConsoleCursorPosition(hdl, { 0, info.dwCursorPosition.Y });
	wprintf(s.c_str());

	GetConsoleScreenBufferInfo(hdl, &info);
	rep(i, info.dwCursorPosition.X, width) { printf(" "); }
	if (newline) { printf("\n"); }
}

void set_locale() { setlocale(LC_ALL, "zh-CN"); }
// 我不懂下面的 Lock 和 Unlock 是什么意思。
wstring get_clipboard() {
	wstring clip;
	if (OpenClipboard(NULL)) {
		HANDLE hdl = GetClipboardData(CF_UNICODETEXT);
		wchar_t* p = (wchar_t*)GlobalLock(hdl);

		if (p) { clip = p; }
		GlobalUnlock(hdl); 
		CloseClipboard();
	} return clip;
}
// 从 MSDN 的例子改的，不太懂，可能有错。
void set_clipboard(wstring const& s) {
	if (OpenClipboard(NULL)) {
		int sz = (s.size() + 1) * sizeof(wchar_t);
		HGLOBAL hdl = GlobalAlloc(GMEM_MOVEABLE, sz);
		if (!hdl) { return; }
		wchar_t* p = (wchar_t*)GlobalLock(hdl);
		if (!p) { return; }

		memcpy(p, s.c_str(), sz);
		GlobalUnlock(hdl);
		SetClipboardData(CF_UNICODETEXT, hdl);
		CloseClipboard();
	}
}

double get_val(double x, vector<double> const& ys) {
	x = clmp(x * (ys.size() - 1), 0.0, ys.size() - 1.0);
	int x0 = floor(x), x1 = ceil(x);
	if (x0 == x1) { return ys[x0]; }
	return ys[x0] * (x1 - x) + ys[x1] * (x - x0);
}
double fetch_num(int& i, wstring const& s) {
	wstring tmp;
	while (i != s.size()) {
		auto c = s[i++];
		if (c != L'(' && c != L')') {
			if (c == L',') { return wtof(tmp); }
			tmp += c;
		}
	} return wtof(tmp);
}
wstring sift_chs(wstring const& chs, bool ignore_newline) {
	wstringstream s; bool cr = false;
	for (auto c : chs) if (c == L'\r') { cr = true; }
	for (auto c : chs) {
		if (iswprint(c)) { s << c; }
		else {
			bool newline =
				c == L'\n' && !cr ||
				c == L'\r' && cr;
			if (newline && !ignore_newline) { s << L'\n'; }
			if (c == L'\t') { s << L"    "; }
		}
	} return s.str();
}
