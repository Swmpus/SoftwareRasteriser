#pragma once
#include <array>

struct Model {
	const std::vector<Vec3> points;
	const std::vector<Vec3> triCols;
	const Transform transform;

	Model(const std::vector<Vec3>& points, const std::vector<Vec3>& triCols, const Transform transform) : points(points), triCols(triCols), transform(transform) {}
};

struct RenderTarget {
	const uint16_t height;
	const uint16_t width;
	std::vector<Vec3> image;

	RenderTarget(const uint16_t height, const uint16_t width) : height(height), width(width), image(height * width) {}
};
