#pragma once
#include "dvec.h"

struct Cur;
struct Body;
struct Connection;
struct Group {
	vector<Body*> bs;
	Group(vector<Body*> const& bs) : bs(bs) {}
	void Warp(drect rc);
};

void find_component(vector<Body*>& out, Body* b);
vector<ptr<Group>> FormGroups(vector<ptr<Body>> const& bs);
