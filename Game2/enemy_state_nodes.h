#pragma once
#include"timer.h"
#include"state_note.h"

class EnemyAimState :public StateNode {
public:
	EnemyAimState();
	~EnemyAimState() = default;

	void on_enter()override;
	void on_update(float delta)override;

private:
	Timer timer;
};

class EnemyDashInAirState :public StateNode {
public:
	EnemyDashInAirState() = default;
	~EnemyDashInAirState() = default;

	void on_enter()override;
	void on_update(float delta)override;
	void on_exit()override;
private:
	const float SPEED_DASH = 1500.f;
};

class EnemyDashOnFloorState :public StateNode {
public:
	EnemyDashOnFloorState();
	~EnemyDashOnFloorState() = default;

	void on_enter()override;
	void on_update(float delta)override;
private:
	const float SPEED_DASH = 1000.f;
	Timer timer;
};

class EnemyDeadState :public StateNode {
public:
	EnemyDeadState() = default;
	~EnemyDeadState() = default;

	void on_enter()override;
};


class EnemyFallState :public StateNode {
public:
	EnemyFallState() = default;
	~EnemyFallState() = default;

	void on_enter()override;
	void on_update(float delta)override;
};

class EnemyIdleState :public StateNode {
public:
	EnemyIdleState();
	~EnemyIdleState() = default;

	void on_enter()override;
	void on_update(float delta)override;
	void on_exit()override;
private:
	Timer timer;
};

class EnemyJumpState :public StateNode {
public:
	EnemyJumpState() = default;
	~EnemyJumpState() = default;

	void on_enter()override;
	void on_update(float delta)override;
private:
	const float SPEED_JUMP = 1000.f;
};

class EnemyRunState :public StateNode {
public:
	EnemyRunState() = default;
	~EnemyRunState() = default;

	void on_enter()override;
	void on_update(float delta)override;
	void on_exit()override;
private:
	const float MIN_DIS = 350.f;
	const float SPEED_RUN = 500.f;
};

class EnemySquatState :public StateNode {
public:
	EnemySquatState();
	~EnemySquatState() = default;

	void on_enter()override;
	void on_update(float delta)override;
private:
	Timer timer;
};

class EnemyThrowBarbState :public StateNode {
public:
	EnemyThrowBarbState();
	~EnemyThrowBarbState() = default;

	void on_enter()override;
	void on_update(float delta)override;
private:
	Timer timer;
};

class EnemyThrowSilkState :public StateNode {
public:
	EnemyThrowSilkState();
	~EnemyThrowSilkState() = default;

	void on_enter()override;
	void on_update(float delta)override;
private:
	Timer timer;
};

class EnemyThrowSwordState :public StateNode {
public:
	EnemyThrowSwordState();
	~EnemyThrowSwordState() = default;

	void on_enter()override;
	void on_update(float delta)override;
private:
	Timer timer_throw;
	Timer timer_switch;
};





