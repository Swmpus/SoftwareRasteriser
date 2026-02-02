#pragma once
#include <array>
#include <cmath>

struct Vec3 {
	float x;
	float y;
	float z;

	Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
	Vec3() : x(0.0f), y(0.0f), z(0.0f) {}

	float dot(const Vec3& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	Vec3 cross(const Vec3& other) const
	{
		return Vec3(
			y * other.z - z * other.y,
			z  * other.x - x * other.z,
			x * other.y - y * other.x
		);
	}

	Vec3 perpendicular() const
	{
		if (x == 0 && y == 0 && z != 0) {
			return cross(Vec3(1.0f, 0.0f, 0.0f));
		} else {
			return cross(Vec3(0.0f, 0.0f, 1.0f));
		}
	}

	float len() const
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	Vec3 operator+(const Vec3& other) const
	{
		return Vec3(x + other.x, y + other.y, z + other.z);
	}

	Vec3 operator-(const Vec3& other) const
	{
    	return Vec3(x - other.x, y - other.y, z - other.z);
	}

	Vec3 operator*(float scalar) const
	{
    	return Vec3(x * scalar, y * scalar, z * scalar);
	}

	Vec3 operator/(float scalar) const
	{
    	return Vec3(x / scalar, y / scalar, z / scalar);
	}
};

struct Vec2 {
	float x;
	float y;

	Vec2(float x, float y) : x(x), y(y) {}
	Vec2() : x(0.0f), y(0.0f) {}

	float dot(const Vec2& other) const
	{
		return x * other.x + y * other.y;
	}

	Vec2 perpendicular() const
	{
		return Vec2(y, -x);
	}

	float len() const
	{
		return std::sqrt(x * x + y * y);
	}

	Vec2 operator+(const Vec2& other) const
	{
		return Vec2(x + other.x, y + other.y);
	}

	Vec2 operator-(const Vec2& other) const
	{
    	return Vec2(x - other.x, y - other.y);
	}

	Vec2 operator*(float scalar) const
	{
    	return Vec2(x * scalar, y * scalar);
	}

	Vec2 operator/(float scalar) const
	{
    	return Vec2(x / scalar, y / scalar);
	}

	Vec2 operator+(float scalar) const
	{
    	return Vec2(x + scalar, y + scalar);
	}
};

struct Mat33 {
	// Row major
	std::array<float, 9> m;

	Mat33(std::array<float, 9> m) : m(m) {}
	Mat33() : m(std::array<float, 9>()) {}

	Vec3 operator*(const Vec3 &vector) const
	{
		Vec3 result = Vec3();
		result.x = vector.x * m[0] + vector.x * m[3] + vector.x * m[6];
		result.y = vector.y * m[1] + vector.y * m[4] + vector.y * m[7];
		result.z = vector.z * m[2] + vector.z * m[5] + vector.z * m[8];
    	return result;
	}
};

struct Transform {
	float yaw;

	Transform(float yaw) : yaw(yaw) {}

	Mat33 getBasisVectors()
	{
		Mat33 basisVectors(std::array<float, 9> {cos(yaw), 0.0f, sin(yaw), 0.0f, 1.0f, 0.0f, -sin(yaw), 0.0f, cos(yaw)});
		return basisVectors;
	}

	Vec3 transformVector(Vec3 vector)
	{
		return getBasisVectors() * vector;
	}
};

bool pointOnRightSideOfLine(const Vec2& a, const Vec2& b, const Vec2& point)
{
	Vec2 ap = point - a;
	Vec2 abPerp = (b - a).perpendicular();
	return ap.dot(abPerp) >= 0;
}

// a, b, and c are the vertices of the triangle wound in a clockwise order
bool pointInTriangle(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& point)
{
	bool sideAB = pointOnRightSideOfLine(a, b, point);
	bool sideBC = pointOnRightSideOfLine(b, c, point);
	bool sideCA = pointOnRightSideOfLine(c, a, point);
	return sideAB && sideBC && sideCA;
}
