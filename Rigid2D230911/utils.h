#pragma once
#include "include.h"

double constexpr PI = 3.1415926535;
double constexpr EU = 2.7182818284;

#define tw to_wstring
#define ptr shared_ptr
#define msh make_shared
#define mkp(nm) nm = msh<std::remove_reference_t<decltype(*nm)>>

#define rep(x, a, b) for (int x = (a), _n = int(b); x < _n; ++x)

#define frd(x) fread(&(x), sizeof(x), 1, f);
#define fwt(x) fwrite(&(x), sizeof(x), 1, f);
#define frds(x, n) fread(x, sizeof(*(x)), n, f);
#define fwts(x, n) fwrite(x, sizeof(*(x)), n, f);
#define frdv(x) frd(sz); (x).resize(sz); frds(&*(x).begin(), sz);
#define fwtv(x) sz = (x).size(); fwt(sz); fwts(&*(x).begin(), sz);

// 注意 insd 是开区间，clmp 闭区间。
template<class T>
bool insd(T v, T a, T b) { return v >= a && v < b; }
template<class T>
T const& clmp(T v, T a, T b) { return min(b, max(a, v)); }

inline int mod(int a, int b) { return (a % b + b) % b; }
inline double modf(double a, double b) { return fmod(fmod(a, b) + b, b); }
inline double modp(double an) { return modf(an + PI, 2 * PI) - PI; }

// 注意 [a, b] 是闭区间，但 sz 是开的。
int drnd(int a, int b);
int drnd(int sz);
double frnd(double a, double b);
double frnd(double b);
double nrnd(double mean, double stddev);

wstring tw2(double d);
wstring tw(string const& s);
string to_string(wstring const& s);
double atof(string const& s);
double wtof(wstring const& s);
FILE* wfopen(wstring const& nm, wstring const& arg);

void hide_console();
void show_console();
void hide_console_cursor();
void print_console(wstring const& s, bool newline = false, int width = 80);

void set_locale();
wstring get_clipboard();
void set_clipboard(wstring const& s);

double get_val(double x, vector<double> const& ys);
double fetch_num(int& i, wstring const& s);
wstring sift_chs(wstring const& chs, bool ignore_newline = false);
