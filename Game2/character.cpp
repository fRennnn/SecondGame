#include"character.h"
#include"collision_manager.h"

Character::Character() {
	hit_box = CollisionManager::instance()->create_collision_box();
	hurt_box = CollisionManager::instance()->create_collision_box();

	timer_invulnerable_status.set_wait_time(1.0f);
	timer_invulnerable_status.set_one_shot(true);
	timer_invulnerable_status.set_on_timeout([&]() {
		is_invulnerable = false;
		});

	timer_invulnerable_blink.set_wait_time(0.075f);
	timer_invulnerable_blink.set_one_shot(false);
	timer_invulnerable_blink.set_on_timeout([&]() {
		is_blink_invisible = !is_blink_invisible;
		});
}

Character::~Character() {
	CollisionManager::instance()->destroy_collision_box(hit_box);
	CollisionManager::instance()->destroy_collision_box(hurt_box);
}
