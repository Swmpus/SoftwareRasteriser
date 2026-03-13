#pragma once

#include <array>
#include <cmath>

const double pi = 3.14159265358979323846;

float degToRad(const float degrees)
{
	return degrees * pi/180;
}

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
		result.x = vector.x * m[0] + vector.y * m[1] + vector.z * m[2];
		result.y = vector.x * m[3] + vector.y * m[4] + vector.z * m[5];
		result.z = vector.x * m[6] + vector.y * m[7] + vector.z * m[8];
    	return result;
	}

	Mat33 operator*(const Mat33 &matrix) const
	{
		std::array<float, 9> result;
		result[0] = m[0] * matrix.m[0] + m[1] * matrix.m[3] + m[2] * matrix.m[6];
		result[1] = m[0] * matrix.m[1] + m[1] * matrix.m[4] + m[2] * matrix.m[7];
		result[2] = m[0] * matrix.m[2] + m[1] * matrix.m[5] + m[2] * matrix.m[8];

		result[3] = m[3] * matrix.m[0] + m[4] * matrix.m[3] + m[5] * matrix.m[6];
		result[4] = m[3] * matrix.m[1] + m[4] * matrix.m[4] + m[5] * matrix.m[7];
		result[5] = m[3] * matrix.m[2] + m[4] * matrix.m[5] + m[5] * matrix.m[8];

		result[6] = m[6] * matrix.m[0] + m[7] * matrix.m[3] + m[8] * matrix.m[6];
		result[7] = m[6] * matrix.m[1] + m[7] * matrix.m[4] + m[8] * matrix.m[7];
		result[8] = m[6] * matrix.m[2] + m[7] * matrix.m[5] + m[8] * matrix.m[8];
    	return Mat33(result);
	}
};

struct Transform {
	float yaw;  // Radians
	float pitch;  // Radians
	float roll;  // Radians
	Vec3 position;

	Transform(float yaw, float pitch, float roll, Vec3 position) : yaw(yaw), pitch(pitch), roll(roll), position(position) {}

	Mat33 getBasisVectors() const
	{
		Mat33 yawBasis(std::array<float, 9> {
			cos(yaw), 0.0f, sin(yaw),
			0.0f, 1.0f, 0.0f,
			-sin(yaw), 0.0f, cos(yaw)
		});
		Mat33 PitchBasis(std::array<float, 9> {
			1.0f, 0.0f, 0.0f,
			0.0f, cos(pitch), sin(pitch),
			0.0f, -sin(pitch), cos(pitch)
		});
		Mat33 RollBasis(std::array<float, 9> {
			cos(roll), sin(roll), 0.0f,
			-sin(roll), cos(roll), 0.0f,
			0.0f, 0.0f, 1.0f
		});
		return (yawBasis * PitchBasis) * RollBasis;
	}

	Vec3 transformVector(const Vec3 &vector) const
	{
		return getBasisVectors() * vector + position;
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
