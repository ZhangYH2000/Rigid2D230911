#pragma once
#include "utils.h"

// 移动相关的还没添加，对这方面不是很有把握。
template<class T> struct vptr {
	ptr<T> p;
	vptr() = default;
	vptr(T const& t) { mkp(p)(t); }
	vptr(vptr const& v) {
		if (v) { mkp(p)(*v); } else { p.reset(); }
	}
	operator bool() const { return (bool)p; }
	void operator=(vptr const& v) {
		if (v) { mkp(p)(*v); } else { p.reset(); }
	}

	ptr<T>& operator->() { return p; }
	T& operator*() { return *p; }
	T const& operator*() const { return *p; }
};

