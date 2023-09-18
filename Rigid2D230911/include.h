#pragma once
#include <cmath>
#include <cstdio>
#include <cfloat>
#include <cwctype>
#include <cstdlib>
#include <clocale>

#include <map>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <array>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <iosfwd>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <type_traits>
#include <unordered_set>

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>
#include <windowsx.h>
#include <ShellScalingApi.h>

#undef UNICODE
#undef _UNICODE
#undef NOMINMAX

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Shcore.lib")
#pragma comment(lib, "Ws2_32.lib")

using std::map;
using std::set;
using std::list;
using std::deque;
using std::queue;
using std::array;
using std::vector;
using std::string;
using std::wstring;
using std::function;
using std::shared_ptr;
using std::wstringstream;
using std::unordered_set;

using std::max;
using std::min;
using std::swap;
using std::next;
using std::move;
using std::sort;
using std::find;
using std::find_if;
using std::remove;
using std::remove_if;
using std::to_string;
using std::to_wstring;
using std::make_shared;
using std::stable_sort;
