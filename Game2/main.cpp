#include<graphics.h>
#include"util.h"
#include"resources_manager.h"
#include<chrono>
#include<thread>
#pragma comment(lib,"MSIMG32.LIB")
static  void draw_background() {
	static IMAGE* img_bg = ResourcesManager::instance()->find_image("background");
	static Rect rect_dst = {
		(getwidth() - img_bg->getwidth()) / 2,
		(getheight() - img_bg->getheight()) / 2,
		img_bg->getwidth(),
		img_bg->getheight()
	};
	putimage_ex(img_bg, &rect_dst);
}
int main(int argc, char** argv) {
	using namespace::std::chrono;
	HWND hwnd = initgraph(1280, 720, EW_SHOWCONSOLE);
	SetWindowText(hwnd, _T("Hollow Katana"));
	
	try {
		ResourcesManager::instance()->load();
	}
	catch (const LPCTSTR id) {
		TCHAR err_msg[512];
		_stprintf_s(err_msg, _T("�޷�����:%s"), id);
		MessageBox(hwnd, err_msg, _T("��Դ����ʧ��"), MB_OK | MB_ICONERROR);
		return -1;
	}
	const nanoseconds frame_duration(1000000000 / 144);
	steady_clock::time_point last_tick = steady_clock::now();

	ExMessage msg;
	bool is_quit = false;

	BeginBatchDraw();

	while (!is_quit) {
		//������Ϣ
		while (peekmessage(&msg)) {

		}
		steady_clock::time_point frame_start = steady_clock::now();
		duration<float> delta = duration<float>(frame_start - last_tick);
		// ��������
		setbkcolor(RGB(0, 0, 0));
		cleardevice();

		//������ͼ
		draw_background();
		FlushBatchDraw();
		last_tick = frame_start;
		nanoseconds sleep_duration = frame_duration - (steady_clock::now() - frame_start);
		if (sleep_duration > nanoseconds(0))
			std::this_thread::sleep_for(sleep_duration);
	}

	EndBatchDraw();
	return 0;
}