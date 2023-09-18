#pragma once
#include "utils.h"

// 这里反正能用，不知道有没有更好的实现方式。
struct wvblk {
	WAVEHDR hdr{};
	vector<int16_t> dat;

	wvblk(int n = 0);
	int n() const;
	bool done() const;
	bool play(HWAVEOUT hwo);
	bool finish(HWAVEOUT hwo);
};

struct app_wv {
	HWAVEOUT hwo{};
	int sp = 0;
	int n_safe = 0;
	int n_blocks = 0;
	double len_block = 0;
	WAVEFORMATEX wofm{};

	int n_play = 0;
	vector<double> wvin;
	// 实践发现下面用指针是必要的, 否则涉及到复制会有错误，可能跟 winapi 有关。
	deque<ptr<wvblk>> wvblks;

	app_wv();
	int len_block_n() const;
	bool init();
	bool play();
	void exit();
};
