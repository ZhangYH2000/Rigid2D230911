#include "var.h"
#include "bra.h"
#include "vec3.h"
#include "proc.h"

// 每次都能找到新 bug，很可能还有一些 bug。
// 同时速度上也有很多改进空间。

Var::Var(double num) : num(num) { typ = L"num"; }
Var::Var(wstring const& str) : str(str) { typ = L"str"; }
Var::Var(decltype(fun) const& user) : fun(user) { typ = L"user"; }
Var::Var(vector<ptr<Var>> const& vec) : vec(vec) { typ = L"vec"; }
Var::Var(Program const& ps, vector<wstring>const& pars) :
	procs(ps), pars(pars) { typ = L"fun"; }

vec2 tv2(Var const& v) {
	vec2 out;
	if (v.vec.size() < 2) { return {}; }
	out.x = v.vec[0]->num;
	out.y = v.vec[1]->num; return out;
}
vec3 tv3(Var const& v) {
	vec3 out;
	if (v.vec.size() < 3) { return {}; }
	out.x = v.vec[0]->num;
	out.y = v.vec[1]->num;
	out.z = v.vec[2]->num; return out;
}

ptr<Var> findvar(Scope& gl, Scope& sc, wstring nm, bool isgl) {
	if (isgl) {
		if (gl.find(nm) == gl.end()) { gl[nm] = msh<Var>(); }
		return gl[nm];
	} else {
		if (sc.find(nm) == sc.end()) { sc[nm] = msh<Var>(); }
		return sc[nm];
	}
}
bool default_fun(Scope& gl, Scope& sc, expr const& ex, Var& out) {
	auto const& f = ex.fun;
	// 下面用宏而不是在运算之前 Eval 是为了速度，测试发现需要这样。
	// 说实话其实现在有点怀疑有没有真的测试过。
	// 不知道有没有别的替代方法。
#define d0 eval(gl, sc, ex.pars[0])->num
#define d1 eval(gl, sc, ex.pars[1])->num
#define s0 eval(gl, sc, ex.pars[0])->str
#define s1 eval(gl, sc, ex.pars[1])->str

	if (f == L"!") { out = Var(!d0); return true; }
	if (f == L"++") { out = Var(d0); return true; }
	if (f == L"--") { out = Var(-d0); return true; }
	if (f == L"sin") { out = Var(sin(d0));  return true; }
	if (f == L"cos") { out = Var(cos(d0));  return true; }
	if (f == L"abs") { out = Var(abs(d0));  return true; }
	if (f == L"+") { out = Var(d0 + d1); return true; }
	if (f == L"-") { out = Var(d0 - d1); return true; }
	if (f == L"*") { out = Var(d0 * d1); return true; }
	if (f == L"/") { out = Var(d0 / d1); return true; }
	if (f == L"<") { out = Var(d0 < d1); return true; }
	if (f == L">") { out = Var(d0 > d1); return true; }
	if (f == L"<=") { out = Var(d0 <= d1); return true; }
	if (f == L">=") { out = Var(d0 >= d1); return true; }
	if (f == L"==") { out = Var(d0 == d1); return true; }
	if (f == L"!=") { out = Var(d0 != d1); return true; }
	if (f == L"&&") { out = Var(d0 && d1); return true; }
	if (f == L"||") { out = Var(d0 || d1); return true; }
	if (f == L"pow") { out = Var(pow(d0, d1)); return true; }
	if (f == L"rand") { out = Var(frnd(d0, d1)); return true; }
	if (f == L"rand_int") { out = Var(drnd(d0, d1)); return true; }
	if (f == L"str") { out = Var(tw2(d0)); return true; }
	if (f == L".") { out = Var(s0 + s1); return true; }
	return false;
#undef d0
#undef d1
#undef s0
#undef s1
}

ptr<Var> eval_call(Scope& gl, Scope& sc, expr const& ex) {
	Var tmp; Scope loc;
	if (default_fun(gl, sc, ex, tmp)) { return msh<Var>(tmp); }

	// 下面必须是值复制，这是为了避免函数自己被抹去。
	auto fun = findvar(gl, sc, ex.fun, ex.global);
	if (fun->typ == L"user" && fun->fun) {
		vector<ptr<Var>> tmp;
		for (auto const& p : ex.pars) { 
			tmp.push_back(eval(gl, sc, p)); 
		} return fun->fun(tmp);
	}

	rep(i, 0, min(fun->pars.size(), ex.pars.size())) {
		loc[fun->pars[i]] = eval(gl, sc, ex.pars[i]);
	} bool ret = false; 
	return Execute(ret, gl, loc, fun->procs);
}
ptr<Var> eval_access(Scope& gl, Scope& sc, expr const& ex) {
	if (!ex.loc || !ex.ind) { return msh<Var>(); }
	auto loc = eval(gl, sc, *ex.loc);
	auto ind = eval(gl, sc, *ex.ind);

	if (ind->typ == L"num") {
		if (loc->typ != L"vec") { *loc = {}; loc->typ = L"vec"; }
		rep(i, loc->vec.size() - 1, ind->num) {
			loc->vec.push_back(msh<Var>());
		} return loc->vec[ind->num];
	}

	if (ind->typ == L"str") {
		if (loc->typ != L"dic") { *loc = {}; loc->typ = L"dic"; }
		if (loc->dic.find(ind->str) == loc->dic.end()) {
			loc->dic[ind->str] = msh<Var>();
		} return loc->dic[ind->str];
	} 
	
	return msh<Var>();
}
ptr<Var> eval(Scope& gl, Scope& sc, expr const& ex) {
	if (ex.typ == L"call") { return eval_call(gl, sc, ex); }
	else if (ex.typ == L"var") {
		if (ex.var == L"PI") { return msh<Var>(PI); }
		return findvar(gl, sc, ex.var, ex.global);
	}
	else if (ex.typ == L"num") { return msh<Var>(ex.num); }
	else if (ex.typ == L"str") { return msh<Var>(ex.str); }
	else if (ex.typ == L"vec") {
		vector<ptr<Var>> tmp;
		for (auto const& e : ex.vec) { 
			tmp.push_back(msh<Var>(*eval(gl, sc, e))); 
		} return msh<Var>(tmp);
	}
	else if (ex.typ == L"access") { return eval_access(gl, sc, ex); }
	return msh<Var>();
}

ptr<Var> Execute(bool& ret, Scope& gl, Scope& sc, Program const& ps) {
	for (auto const& p : ps) {
		if (p.typ == L"single") { eval(gl, sc, p.single); }
		else if (p.typ == L"return") { ret = true; return eval(gl, sc, p.ret); }
		else if (p.typ == L"define") {
			*findvar(gl, sc, p.def_fun, false) = Var(p.procs, p.def_pars);
		}
		else if (p.typ == L"assign") { *eval(gl, sc, p.left) = *eval(gl, sc, p.right); }
		else if (p.typ == L"if") {
			auto& ps = eval(gl, sc, p.if_con)->num ? p.if_procs : p.else_procs;
			auto v = Execute(ret, gl, sc, ps); if (ret) { return v; }
		}
		else if (p.typ == L"while") {
			while (eval(gl, sc, p.lp_con)->num) {
				auto v = Execute(ret, sc, gl, p.lp); if (ret) { return v; }
			}
		}
	} return msh<Var>();
}
ptr<Var> Execute(Scope& gl, Program const& ps) {
	bool ret = false; return Execute(ret, gl, gl, ps);
}
Program Compile(wstring const& code) {
	return parsing_procs(bra_analysis(tokenize(code)));
}
