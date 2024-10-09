#pragma once
#include"vector2.h"
#include"collision_layer.h"
#include<functional>

class CollisionManager;

class CollisionBox {
	friend CollisionManager;

private:
	Vector2 size;
	Vector2 position;
	bool enabled = true;
	std::function<void()> on_collide;
	//自身碰撞箱
	CollisionLayer layer_src = CollisionLayer::None;
	//目标碰撞箱
	CollisionLayer layer_dst = CollisionLayer::None;

private:
	CollisionBox() = default;
	~CollisionBox() = default;

public:
	void set_enabled(bool flag) {
		enabled = flag;
	}

	void set_layer_src(CollisionLayer layer) {
		layer_src = layer;
	}

	void set_layer_dst(CollisionLayer layer) {
		layer_dst = layer;
	}

	void set_on_collide(std::function<void()> on_collide) {
		this->on_collide = on_collide;
	}

	void set_size(const Vector2& size) {
		this->size = size;
	}

	const Vector2& get_size()const {
		return size;
	}

	void set_position(const Vector2& position) {
		this->position = position;
	}
};