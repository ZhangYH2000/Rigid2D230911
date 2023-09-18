#include "group.h"
#include "body.h"
#include "connection.h"
#include "cur.h"

void Group::Warp(drect rc) {
	for (auto b : bs) if (b->inv_m == 0 || b->dragged) { return; }
	bool ok = false;

	ok = true;
	for (auto b : bs) if (b->o.x > rc.left()) { ok = false; break; }
	if (ok) for (auto b : bs) { b->o.x += rc.w; b->track.clear(); }

	ok = true;
	for (auto b : bs) if (b->o.x < rc.right()) { ok = false; break; }
	if (ok) for (auto b : bs) { b->o.x -= rc.w; b->track.clear(); }

	ok = true;
	for (auto b : bs) if (b->o.y > rc.top()) { ok = false; break; }
	if (ok) for (auto b : bs) { b->o.y += rc.h;  b->track.clear(); }

	ok = true;
	for (auto b : bs) if (b->o.y < rc.bottom()) { ok = false; break; }
	if (ok) for (auto b : bs) { b->o.y -= rc.h;  b->track.clear(); }
}

void find_component(vector<Body*>& out, Body* b) {
	b->visited = true;
	out.push_back(b);
	for (auto c : b->cons) {
		auto tmp = the_other(b, *c);
		if (tmp && !tmp->visited) {
			find_component(out, tmp);
		}
	}
}
vector<ptr<Group>> FormGroups(vector<ptr<Body>> const& bs) {
	vector<ptr<Group>> out;
	vector<Body*> tmp;
	for (auto b : bs) { b->visited = false; }
	for (auto b : bs) {
		if (!b->visited) {
			tmp.clear();
			find_component(tmp, &*b);
			out.push_back(msh<Group>(tmp));
		}
	} return out;
}
