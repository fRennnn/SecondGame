#pragma once
#include<functional>
class Timer {
public:
	Timer() = default;
	~Timer() = default;

	void restart() {
		pass_time = 0;
		shotted = false;
	}

	void set_wait_time(float val) {
		wait_time = val;
	}

	void set_one_shot(bool flag) {
		one_shoot = flag;
	}

	void set_on_timeout(std::function<void()> on_timeout) {
		this->on_timeout = on_timeout;
	}

	void pause() {
		paused = true;
	}

	void resume() {
		paused = false;
	}
	/*ʱ�䵽���ִ�лص�����*/
	void on_update(float delta) {
		if (paused)
			return;

		pass_time += delta;
		if (pass_time >= wait_time) {
			bool can_shot = (!one_shoot || (one_shoot && !shotted));
			shotted = true;
			if (can_shot && on_timeout)
				on_timeout();
			pass_time -= wait_time;
		}
	}
private:
	float pass_time = 0;					//�ѹ�ʱ��
	float wait_time = 0;					// �ȴ�ʱ��
	bool paused = false;					//�Ƿ���ͣ
	bool shotted = false;					//�Ƿ񴥷�
	bool one_shoot = false;					//���δ���
	std::function<void()> on_timeout;		//�����ص�
};