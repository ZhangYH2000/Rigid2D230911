#pragma once
#include "font.h"
#include "control.h"

struct Button : Control {
	dvec gap;
	double dep = 0;
	wstring txt;
	int w = 0, h = 0;
	dcol c_txt, c_frame, c_normal, c_chosen, c_hovered, c_invalid;

	bool chosen = false;
	bool enabled = true;
	bool hovered = false;

	Button(int w = 0);
	int GetW() const override;
	int GetH() const override;

	void render(App& app);
	virtual void OnClick(App& app) {}

	void Update(App& app) override;
	void Discard(App& app) override;
	void PreUpdate(App& app) override;
};
struct BtLan : Button {
	wstring id_txt;
	BtLan() = default;
	BtLan(int w, wstring const& id);
	void Update(App& app) override;
};

struct FloatPanel;
struct BtFPn : Button {
	FloatPanel* fpn = NULL;
	BtFPn() = default;
	BtFPn(FloatPanel* fpn);
	void OnClick(App& app) override;
	void Update(App& app) override;
};
