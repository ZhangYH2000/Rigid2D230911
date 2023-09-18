#include "app_wv.h"

wvblk::wvblk(int n) : dat(n) {
	hdr.lpData = (LPSTR)dat.data();
	hdr.dwBufferLength = 2 * n;
}
int wvblk::n() const { return dat.size(); }
bool wvblk::done() const { return hdr.dwFlags & WHDR_DONE; }
bool wvblk::play(HWAVEOUT hwo) {
	auto ret0 = waveOutPrepareHeader(hwo, &hdr, sizeof(hdr));
	auto ret1 = waveOutWrite(hwo, &hdr, sizeof(hdr));
	return
		ret0 == MMSYSERR_NOERROR &&
		ret1 == MMSYSERR_NOERROR;
}
bool wvblk::finish(HWAVEOUT hwo) {
	return waveOutUnprepareHeader(hwo, &hdr, sizeof(hdr)) == MMSYSERR_NOERROR;
}

app_wv::app_wv() {
	// 这里实时播放的处理我还不知道怎么做的更好。
	sp = 44100; len_block = 5e-4;
	n_safe = 120; n_blocks = 512;
}
int app_wv::len_block_n() const { return len_block * sp; }
bool app_wv::init() {
	wofm.wFormatTag = WAVE_FORMAT_PCM;
	wofm.nChannels = 1;
	wofm.nSamplesPerSec = sp;
	// 好像这里不能是其它数了，没怎么搞清楚。
	wofm.wBitsPerSample = 16;
	wofm.nBlockAlign = 2;
	wofm.nAvgBytesPerSec = 2 * sp;

	auto ret = waveOutOpen(&hwo, WAVE_MAPPER, &wofm, NULL, 0, CALLBACK_NULL);
	if (ret != MMSYSERR_NOERROR) { return false; }
	rep(i, 0, n_blocks) {
		wvblks.push_back(msh<wvblk>(len_block_n()));
	} return true;
}
bool app_wv::play() {
	while (n_play > 0 && wvblks.front()->done()) {
		bool ret = wvblks.front()->finish(hwo);
		if (!ret) { return false; } n_play--;
		wvblks.push_back(wvblks.front());
		wvblks.pop_front();
	} 
	
	int cur = 0;
	int n_in = max(0, n_safe - n_play);
	for (auto v : wvin) {
		wvblks[n_play]->dat[cur++] =
			clmp(int(v * 256 * 128), -128 * 256, 128 * 256 - 1);
		if (cur == len_block_n()) {
			bool ret = wvblks[n_play++]->play(hwo);
			if (!ret) { return false; } cur = 0;
		}
	} wvin = vector<double>(n_in * len_block_n());
	return true;
}
void app_wv::exit() {
	while (n_play > 0) {
		while (!wvblks.front()->done()) { Sleep(0); }
		wvblks.front()->finish(hwo);
		wvblks.pop_front(); n_play--;
	} waveOutClose(hwo);
}
