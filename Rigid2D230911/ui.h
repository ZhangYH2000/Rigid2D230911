#pragma once
#include "utils.h"

struct Cur;
struct FloatPanel;
struct FPnGlobal;
struct FPnBody;
struct FPnCon;
struct FPnCmd;
struct FPnPar;
struct FPnLang;
struct FPnAbout;
struct PnMenu;
struct PnBottom;
struct UI {
	vector<FloatPanel*> fpns;
	ptr<FPnGlobal> fpn_global;
	ptr<FPnBody> fpn_body;
	ptr<FPnCon> fpn_con;
	ptr<FPnCmd> fpn_cmd;
	ptr<FPnPar> fpn_par;
	ptr<FPnLang> fpn_lang;
	ptr<FPnAbout> fpn_about;
	ptr<PnMenu> pn_menu;
	ptr<PnBottom> pn_bottom;

	UI(Cur& cur) {};
	void sort_fpns();

	// 这里用 init 的原因是控件会用 cur->ui 来访问 ui, 但这个时候 ui 尚未初始化。
	// 这也是为什么主程序不需要 Init。这应该是一个普遍的问题。
	// 好像上面这些话已经过时了，不过还是保留这个设计以防万一。
	void Init(Cur& cur);
	void Update(Cur& cur);
	void PreUpdate(Cur& cur);
};
