#include "app.h"
#include "tile.h"

App::App() {
	w = 800; h = 600;
	title = L"程序"; class_name = L"class";
}
void App::Init() {
	addlog(L"开始加载程序.\n");
	set_locale(); scr = tile(w, h);
	dscr = ini_dscr = dbuf(w * h, -DBL_MAX);
	bool ret = ft.init();
	addlog(ret ? L"成功加载字体模块.\n" : L"加载字体模块失败.\n");

	HRESULT hr = SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
	addlog(hr == S_OK ? L"成功设置 DPI 感知.\n" : L"设置 DPI 感知失败.\n");
	hinst = GetModuleHandle(NULL);
	addlog(hinst ? L"成功取得模块句柄.\n" : L"取得模块句柄失败.\n");

	register_window_class(); create_window(); ret = wv.init();
	addlog(ret ? L"成功加载声音模块.\n" : L"加载声音模块失败.\n");
	in.init(hwnd); init_bmi(); load_localization();

	hdc = GetDC(hwnd);
	addlog(hdc ? L"成功取得设备环境句柄.\n" : L"取得设备环境句柄失败.\n");
	sys_hcs = LoadCursorW(NULL, IDC_ARROW);
	addlog(sys_hcs ? L"成功加载光标.\n" : L"加载光标失败.\n");
}
void App::Run(bool console) {
	if (!console) { hide_console(); }
	MSG msg{}; fps.run(); ShowWindow(hwnd, SW_SHOW);

	while (true) {
		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) { break; }
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else {
			bool ret = SetWindowText(hwnd, title.c_str());
			if (!ret) { addlog(L"设置标题失败.\n"); }
			fps.update(); reset_dscr();
			ret = in.sync_mouse(hwnd);
			if (!ret) { addlog(L"同步鼠标位置失败.\n"); }
			Update(); ret = wv.play();
			if (!ret) { addlog(L"播放声音失败.\n"); }
			present_scr(); in.reset(); own.reset();
		}
	} ReleaseDC(hwnd, hdc);
	wv.exit(); addlog(L"退出程序.\n\n");
}

void App::reset_dscr() {
	memcpy(dscr.data(), ini_dscr.data(), dscr.size() * sizeof(double));
}
void App::present_scr() {
	int ret = SetDIBitsToDevice(hdc, 0, 0, w, h,
		0, 0, 0, h, scr.cols.data(), &bmi, DIB_RGB_COLORS);
	if (!ret) { addlog(L"展示屏幕失败.\n"); }
}
void App::addlog(wstring const& s) {
	FILE* f_log = wfopen(L"运行记录.log", L"a");
	if (!f_log) { print_console(L"运行记录打开失败。", true); }
	else { fwprintf(f_log, s.c_str()); fclose(f_log); }
}
wstring App::loc(wstring const& id) const {
	if (id.empty()) { return {}; }
	auto it = dict.find(id);
	if (it == dict.end()) { return {}; }
	auto const& strs = it->second;
	return lan >= strs.size() ? L"" : strs[lan];
}

void App::init_bmi() {
	BITMAPINFOHEADER hd{};
	hd.biSize = sizeof(hd);
	hd.biWidth = w;
	hd.biHeight = -h;
	hd.biPlanes = 1;
	hd.biBitCount = 24;
	hd.biCompression = BI_RGB;
	bmi.bmiHeader = hd;
}
void App::load_localization() {
	FILE* f = wfopen(L"localization.txt", L"r");
	if (!f) { return; }

	bool dollar = false;
	wstringstream s; wstring id;
	vector<wstring> strs; wchar_t c = getwc(f);
	while (c != WEOF) {
		if (c == L'$') {
			if (dollar) {
				strs.push_back(s.str()); 
				s = wstringstream();
			} dollar = !dollar;
		}
		else if (dollar) { s << c; }
		else if (!iswspace(c)) {
			if (!strs.empty()) {
				dict[id] = strs; 
				strs.clear(); id.clear();
			} id += c;
		} c = getwc(f);
	} 
	if (!strs.empty()) { dict[id] = strs; } 
	fclose(f);
}
void App::register_window_class() {
	WNDCLASSEXW wc{};
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = wnd_proc;
	wc.hInstance = hinst;
	wc.lpszClassName = class_name.c_str();

	ATOM ret = RegisterClassExW(&wc);
	addlog(ret ? L"成功注册窗口类.\n" : L"注册窗口类失败.\n");
}
void App::create_window() {
	RECT cr{}; cr.right = w; cr.bottom = h;
	DWORD sty = WS_SYSMENU | WS_CAPTION;
	BOOL ret = AdjustWindowRect(&cr, sty, FALSE);
	addlog(ret ? L"成功调整窗口矩形.\n" : L"调整窗口矩形失败.\n");
	int rw = cr.right - cr.left;
	int rh = cr.bottom - cr.top;

	hwnd = CreateWindowExW(0, class_name.c_str(),
		title.c_str(), sty, 0, 0, rw, rh, NULL, NULL, hinst, this);
	addlog(hwnd ? L"成功创建窗口.\n" : L"创建窗口失败.\n");
}
