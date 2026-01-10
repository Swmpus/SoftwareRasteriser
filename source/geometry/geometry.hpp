#pragma once
#include <array>

struct Model {
	const std::vector<Vec3> points;
	const std::vector<Vec3> triCols;

	Model(const std::vector<Vec3>& points, const std::vector<Vec3>& triCols) : points(points), triCols(triCols) {}
};
