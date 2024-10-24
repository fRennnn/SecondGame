#include"util.h"
#include"collision_manager.h"
#include"resources_manager.h"
#include"character_manager.h"
#include"bullet_time_manager.h"

#include<chrono>
#include<thread>
#include<graphics.h>
#include<iostream>
#pragma comment(lib,"MSIMG32.LIB")
bool is_debug = false;
struct SmartTimer {
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;

	SmartTimer() {
		start = std::chrono::high_resolution_clock::now();
	}
	~SmartTimer() {
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;

		float s = duration.count();
		std::cout << "Timer took " << s << " s" << std::endl;
	}
};
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
static void draw_remain_hp() {
	static IMAGE* img_ui_heart = ResourcesManager::instance()->find_image("ui_heart");
	Rect rect_dst = { 0,10,img_ui_heart->getwidth(),img_ui_heart->getheight() };
	for (int i = 0; i < CharacterManager::instance()->get_player()->get_hp(); ++i) {
		rect_dst.x = 10 + i * 40;
		putimage_ex(img_ui_heart, &rect_dst);
	}
}
int main(int argc, char** argv) {
	int FPS = 0;
	const double Sec = 1.0f;
	double timer = 0;
	using namespace::std::chrono;
	HWND hwnd = initgraph(1280, 720);
	SetWindowText(hwnd, _T("粪作^^"));
	try {
		ResourcesManager::instance()->load();
	}
	catch (const LPCTSTR id) {
		TCHAR err_msg[512];
		_stprintf_s(err_msg, _T("无法加载:%s"), id);
		MessageBox(hwnd, err_msg, _T("资源加载失败"), MB_OK | MB_ICONERROR);
		return -1;
	}

	play_audio(_T("bgm"), true);

	const nanoseconds frame_duration(1000000000 / 144);
	steady_clock::time_point last_tick = steady_clock::now(); 

	ExMessage msg; 
	bool is_quit = false;

	BeginBatchDraw();

	while (!is_quit) {
		std::chrono::time_point<std::chrono::steady_clock> start, end;
		std::chrono::duration<float> duration;
		start = std::chrono::high_resolution_clock::now();
		//处理消息
		while (peekmessage(&msg)) {
			CharacterManager::instance()->on_input(msg);
		}

		steady_clock::time_point frame_start = steady_clock::now();
		std::chrono::duration<float> delta = std::chrono::duration<float>(frame_start - last_tick);

		// 处理更新
		float scaled_delta = BulletTimeManager::instance()->on_update(delta.count());

		//std::cout << delta.count() << "\n";
		//0.018  
		CharacterManager::instance()->on_update(scaled_delta);
		CollisionManager::instance()->process_collide();  

		setbkcolor(RGB(0, 0, 0));
		cleardevice();

		//处理绘图
		draw_background();
		CharacterManager::instance()->on_render();
		if (is_debug) {
			CollisionManager::instance()->on_debug_render();
		}
		
		draw_remain_hp();

		FlushBatchDraw();

		last_tick = frame_start;
		nanoseconds sleep_duration = frame_duration - (steady_clock::now() - frame_start);
		if (sleep_duration > nanoseconds(0))
			std::this_thread::sleep_for(sleep_duration);

		end = std::chrono::high_resolution_clock::now();
		duration = end - start;

		/*timer += duration.count();
		FPS++;
		if (timer > Sec) {
			timer = 0;
			std::cout << "FPS : " << FPS << "\n";
			FPS = 0;
		}*/

		//1000 / FPS - frame_delta_time
	}

	EndBatchDraw();
	return 0;
}