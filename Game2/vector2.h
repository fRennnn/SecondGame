#pragma once
#include<cmath>
class Vector2 {
public:
	float x = 0;
	float y = 0;
public:
	Vector2() = default;
	~Vector2() = default;

	Vector2(float _x, float _y)
		: x(_x), y(_y) { }

	Vector2 operator+(const Vector2& vec) const {
		return Vector2(x + vec.x, y + vec.y);
	}

	void operator+=(const Vector2& vec) {
		x += vec.x, y += vec.y;
	}

	void operator-=(const Vector2& vec) {
		x -= vec.x, y -= vec.y;
	}

	Vector2 operator-(const Vector2& vec) const {
		return Vector2(x - vec.x, y - vec.y);
	}

	float operator*(const Vector2& vec) const {
		return x * vec.x + y * vec.y;
	}

	Vector2 operator*(float val) const {
		return Vector2(x * val, y * val);
	}

	void operator*=(const Vector2& vec) {
		x *= vec.x, y *= vec.y;
	}

	float length()
	{
		return sqrt(x * x + y * y);
	}

	Vector2 normalize()
	{
		float len = length();

		if (len == 0)
			return Vector2(0, 0);

		return Vector2(x / len, y / len);
	}
};
//you NEED this inline!!!
inline float abs_x(const Vector2& left, const Vector2& right) {
	return std::abs(left.x - right.x);
}

inline float abs_y(const Vector2& left, const Vector2& right) {
	return std::abs(left.y - right.y);
}