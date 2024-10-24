#include"enemy.h"
#include"character_manager.h"
#include"enemy_state_nodes.h"
#include<iostream>
EnemyAimState::EnemyAimState() {
	timer.set_one_shot(true);
	timer.set_wait_time(0.5f);
	timer.set_on_timeout([&]() {
		Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
		enemy->set_gravity_enabled(true);
		enemy->switch_state("dash_in_air");
		});
}

void EnemyAimState::on_enter() {
	//std::cout << "State: Aim\n";
	CharacterManager::instance()->get_enemy()->set_animation("aim");

	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
	enemy->set_gravity_enabled(false);
	enemy->set_velocity({ 0,0 });
	timer.restart();
}

void EnemyAimState::on_update(float delta) {
	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

	timer.on_update(delta);

	if (enemy->get_hp() <= 0)
		enemy->switch_state("dead");
}

void EnemyDashInAirState::on_enter() {
	//std::cout << "State: dash_in_air\n";
	CharacterManager::instance()->get_enemy()->set_animation("dash_in_air");

	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
	const Character* player = CharacterManager::instance()->get_player();
	Vector2 pos_target = { player->get_position().x,player->get_floor_y() };
	enemy->set_velocity((pos_target - enemy->get_position()).normalize() * SPEED_DASH);
	enemy->set_dashing_in_air(true);
	enemy->set_gravity_enabled(false);
	enemy->on_dash();

	play_audio(_T("enemy_dash"), false);
}

void EnemyDashInAirState::on_update(float delta) {
	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

	if (enemy->get_hp() <= 0)
		enemy->switch_state("dead");
	else if (enemy->is_on_floor())
		enemy->switch_state("idle");
}

void EnemyDashInAirState::on_exit() {
	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

	enemy->set_gravity_enabled(true);
	enemy->set_dashing_in_air(false);
}

EnemyDashOnFloorState::EnemyDashOnFloorState() {
	timer.set_one_shot(true);
	timer.set_wait_time(0.5f);
	timer.set_on_timeout([&]() {
		//std::cout << "timer tick\n";
		Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
		enemy->set_dashing_on_floor(false);
		});
}

void EnemyDashOnFloorState::on_enter() {
	//std::cout << "State: dash_on_floor\n";
	CharacterManager::instance()->get_enemy()->set_animation("dash_on_floor");

	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
	enemy->set_velocity({ enemy->get_facing_left() ? -SPEED_DASH : SPEED_DASH,0 });
	enemy->set_dashing_on_floor(true);
	enemy->on_dash();
	timer.restart();

	play_audio(_T("enemy_dash"), false);
}

void EnemyDashOnFloorState::on_update(float delta) {
	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
	timer.on_update(delta);
	if (enemy->get_hp() <= 0)
		enemy->switch_state("dead");
	else if (!enemy->get_dashing_on_floor())
		enemy->switch_state("idle");
}

void EnemyDeadState::on_enter() {
	MessageBox(GetHWnd(), _T("Good"), _T("��ս�ɹ�!"), MB_OK);
	exit(0);
}

void EnemyFallState::on_enter() {
	//std::cout << "State: fall\n";
	CharacterManager::instance()->get_enemy()->set_animation("fall");
}

void EnemyFallState::on_update(float delta) {
	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

	if (enemy->get_hp() <= 0)
		enemy->switch_state("dead");
	else if (enemy->is_on_floor())
		enemy->switch_state("idle");
}

EnemyIdleState::EnemyIdleState() {
	timer.set_one_shot(true);
	timer.set_on_timeout([&]() {
		Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

		int rand_num = range_random(0, 100);
		if (enemy->get_hp() > 5) {
			if (rand_num <= 25) {
				if (!enemy->is_on_floor())
					enemy->switch_state("fall");
				else
					enemy->switch_state("jump");//25%
			}
			else if (rand_num <= 50) {
				if (!enemy->is_on_floor())
					enemy->switch_state("fall");
				else
					enemy->switch_state("run");//25%
			}
			else if (rand_num <= 80)
				enemy->switch_state("squat");
			else if (rand_num <= 90)
				enemy->switch_state("throw_silk");
			else
				enemy->switch_state("throw_sword");
		}
		else {
			if (rand_num <= 25) {
				if (!enemy->is_on_floor())
					enemy->switch_state("fall");
				else
					enemy->switch_state("jump");//25%
			}
			else if (rand_num <= 60)
				enemy->switch_state("throw_sword");//35%
			else if (rand_num <= 70)
				enemy->switch_state("throw_silk");//10%
			else if (rand_num <= 90)
				enemy->switch_state("throw_barb");//20%
			else
				enemy->switch_state("squat");	//10%
		}
		});
}

void EnemyIdleState::on_enter() {
	//std::cout << "State: idle\n";
	CharacterManager::instance()->get_enemy()->set_animation("idle");

	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
	enemy->set_velocity({ 0,0 });

	float wait_time = 0;
	if (enemy->get_hp() > 5)
		wait_time = range_random(0, 2) * 0.25f; // 0.0s ~ 0.5s
	else
		wait_time = range_random(0, 1) * 0.25f; // 0.0s ~ 0.25s

	timer.set_wait_time(wait_time);
	timer.restart();
}

void EnemyIdleState::on_update(float delta) {
	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

	timer.on_update(delta);

	if (enemy->get_hp() <= 0)
		enemy->switch_state("dead");
	else if (enemy->get_velocity().y > 0)
		enemy->switch_state("fall");
}

void EnemyIdleState::on_exit() {
	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
	enemy->set_facing_left(enemy->get_position().x > CharacterManager::instance()->get_player()->get_position().x);
}

void EnemyJumpState::on_enter() {
	//std::cout << "State: jump\n";
	CharacterManager::instance()->get_enemy()->set_animation("jump");

	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
	enemy->set_velocity({ 0,-SPEED_JUMP });
}

void EnemyJumpState::on_update(float delta) {
	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

	if (enemy->get_hp() <= 0)
		enemy->switch_state("dead");
	else if (enemy->get_velocity().y > 0) {
		int rand_num = range_random(0, 100);
		if (enemy->get_hp() > 5) {
			if (rand_num <= 50)
				enemy->switch_state("aim");
			else if (rand_num <= 80)
				enemy->switch_state("fall");
			else
				enemy->switch_state("throw_silk");
		}
		else {
			if (rand_num <= 50)
				enemy->switch_state("throw_silk");
			else if (rand_num <= 80)
				enemy->switch_state("fall");
			else
				enemy->switch_state("aim");
		}
	}
}

void EnemyRunState::on_enter() {
	//std::cout << "State: run\n";
	CharacterManager::instance()->get_enemy()->set_animation("run");
	play_audio(_T("enemy_run"), true);
}

void EnemyRunState::on_update(float delta) {
	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

	const Vector2& pos_enemy = enemy->get_position();
	const Vector2& pos_player = CharacterManager::instance()->get_player()->get_position();
	enemy->set_velocity({pos_enemy.x < pos_player.x?SPEED_RUN:-SPEED_RUN,0});

	if (enemy->get_hp() <= 0)
		enemy->switch_state("dead");
	else if (abs(pos_enemy.x - pos_player.x) <= MIN_DIS) {
		int rand_num = range_random(0, 100);
		if (enemy->get_hp() > 5) {
			if (rand_num <= 75)
				enemy->switch_state("squat");
			else
				enemy->switch_state("throw_silk");
		}
		else {
			if (rand_num <= 75)
				enemy->switch_state("throw_silk");
			else
				enemy->switch_state("squat");
		}
		stop_audio(_T("enemy_run"));
	}
}

void EnemyRunState::on_exit() {
	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
	enemy->set_velocity({ 0,0 });
}

EnemySquatState::EnemySquatState() {
	timer.set_one_shot(true);
	timer.set_wait_time(0.5f);
	timer.set_on_timeout([&]() {
		Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
		enemy->switch_state("dash_on_floor");
		});
}

void EnemySquatState::on_enter() {
	//std::cout << "State: squat\n";
	CharacterManager::instance()->get_enemy()->set_animation("squat");

	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
	enemy->set_facing_left(enemy->get_position().x > CharacterManager::instance()->get_player()->get_position().x);
	timer.restart();
}

void EnemySquatState::on_update(float delta) {
	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

	timer.on_update(delta);

	if (enemy->get_hp() <= 0)
		enemy->switch_state("dead");
}

EnemyThrowBarbState::EnemyThrowBarbState() {
	timer.set_one_shot(true);
	timer.set_wait_time(0.8f);
	timer.set_on_timeout([&]() {
		Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
		enemy->throw_barbs();
		enemy->switch_state("idle");
		});
}

void EnemyThrowBarbState::on_enter() {
	//std::cout << "State: throw_barb\n";
	CharacterManager::instance()->get_enemy()->set_animation("throw_barb");
	timer.restart();
	play_audio(_T("enemy_throw_barbs"), false);
}

void EnemyThrowBarbState::on_update(float delta) {
	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

	timer.on_update(delta);
	if (enemy->get_hp() <= 0)
		enemy->switch_state("dead");
}

EnemyThrowSilkState::EnemyThrowSilkState() {
	timer.set_one_shot(true);
	timer.set_wait_time(0.9f);
	timer.set_on_timeout([&]() {
		Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
		enemy->set_gravity_enabled(true);
		enemy->set_throwing_silk(false);
		if (!enemy->is_on_floor() && enemy->get_hp() > 5 && range_random(0, 100) <= 25)
			enemy->switch_state("aim");
		else if (!enemy->is_on_floor())
			enemy->switch_state("fall");
		else
			enemy->switch_state("idle");
		});
 }

void EnemyThrowSilkState::on_enter() {
	//std::cout << "State: throw_silk\n";
	CharacterManager::instance()->get_enemy()->set_animation("throw_silk");

	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
	enemy->set_gravity_enabled(true);
	enemy->set_throwing_silk(true);
	enemy->set_velocity({ 0,0 });
	enemy->on_throw_silk();
	timer.restart();

	play_audio(_T("enemy_throw_silk"), false);
}

void EnemyThrowSilkState::on_update(float delta) {
	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

	timer.on_update(delta);

	if (enemy->get_hp() <= 0)
		enemy->switch_state("dead");
}

EnemyThrowSwordState::EnemyThrowSwordState() {
	timer_throw.set_wait_time(0.65f);
	timer_throw.set_one_shot(true);
	timer_throw.set_on_timeout([&]() {
		//std::cout << "timer_throw tick\n";
		Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
		enemy->throw_sword();

		play_audio(_T("enemy_throw_sword"), false);
		});

	timer_switch.set_wait_time(0.65f);
	timer_switch.set_one_shot(true);
	timer_switch.set_on_timeout([&]() {
		//std::cout << "timer_switch tick\n";
		Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

		int rand_num = range_random(0, 100);
		if (enemy->get_hp() > 5) {
			if (rand_num <= 50)
				enemy->switch_state("squat");
			else if (rand_num <= 80)
				enemy->switch_state("jump");
			else
				enemy->switch_state("idle");
		}
		else {
			if (rand_num <= 50)
				enemy->switch_state("jump");
			else if (rand_num <= 80)
				enemy->switch_state("throw_silk");
			else
				enemy->switch_state("idle");
		}
		});
}

void EnemyThrowSwordState::on_enter() {
	//std::cout << "State: throw_sword\n";
	CharacterManager::instance()->get_enemy()->set_animation("throw_sword");
	timer_throw.restart();
	timer_switch.restart();
}

void EnemyThrowSwordState::on_update(float delta) {
	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

	timer_throw.on_update(delta);
	timer_switch.on_update(delta);

	if (enemy->get_hp() <= 0)
		enemy->switch_state("dead");
}