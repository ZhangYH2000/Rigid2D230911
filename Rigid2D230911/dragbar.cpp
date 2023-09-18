#include "dragbar.h"
#include "app.h"
#include "draw_geo.h"
#include "draw_str.h"

#include "ctrl_def.h"

DragbarX::DragbarX(int w) : bw(w) {
	gap = { 10, 10 }; dep = 1000;
	bh = 2; nw = 12; nh = 30;

	c_txt = { 255, 255, 255 };
	c_bar = { 80, 80, 80 };
	c_normal = { 80, 80, 80 };
	c_hovered = { 20, 20, 255 };
	c_invalid = { 0, 0, 0 };
	c_dragged = { 250, 20, 20 };
}
int DragbarX::GetW() const { return bw + gap.x + w_txt; }
int DragbarX::GetH() const { return nh + gap.y; }

// 下面的表达式是正确的，不需要加一减一什么的。
double DragbarX::uval() const { return (double)nx_rel / (bw - nw); }
void DragbarX::set_nx_rel(double uv) {
	int tmp = uv * (bw - nw);
	nx_rel = clmp(tmp, 0, bw - nw);
}
dvec DragbarX::tl_node() const { return tl + dvec(w_txt + nx_rel, 0); }
void DragbarX::render(App& app) {
	dvec tl_txt = tl + dvec(0, (nh - ft.h) / 2);
	draw_str(scr, dscr, dep, txt, c_txt, ft, tl_txt, 0, vp);

	dcol const& c =
		!enabled ? c_invalid :
		dragged ? c_dragged :
		rhvd ? c_hovered : c_normal;
	dvec tl_bar = tl + dvec(w_txt, (nh - bh) / 2);
	draw_rect_raw(scr, tl_bar, bw, bh, vp, c_bar);
	draw_rect_raw(scr, tl_node(), nw, nh, vp, c);
}

void DragbarX::Update(App& app) {
	hovered = (hvd == this);
	w_txt = str_wh(txt, ft, 0).x;
	if (dragged) {
		nx_rel = clmp(msp.x - (tl.x + w_txt + nw / 2), 0, bw - nw);
		// 在刚刚抓住的那一帧没有更新，在放下的那一帧还在更新。省事。
		Upload(app);
		if (enabled && !msd[0]) { OnDone(app); }
		dragged = enabled && msd[0];
	} else {
		Sync(app);
		dragged = enabled && hovered && msc(0);
	} render(app);
}
void DragbarX::Discard(App& app) { 
	rmv; w_txt = 0;
	dragged = hovered = false; 
}
void DragbarX::PreUpdate(App& app) {
	bool ok = dhv <= dep &&
		insd(msp, { tl_node(), nw, nh }) && insd(msp, vp);
	if (ok) { dhv = dep; hvd = this; }
}

DbXLan::DbXLan(wstring const& id, int w) : DragbarX(w), id_txt(id) {}
void DbXLan::Update(App& app) { txt = loc(id_txt); DragbarX::Update(app); }
