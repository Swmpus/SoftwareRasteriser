#include <string>
#include <vector>
#include "maths.hpp"
#include "io.hpp"
#include "geometry.hpp"

#ifndef DISPLAY_HEIGHT
#define DISPLAY_HEIGHT 280
#endif
#ifndef DISPLAY_WIDTH
#define DISPLAY_WIDTH 240
#endif

void render(const std::vector<Triangle>& tris)
{
	Vec3 image[DISPLAY_HEIGHT][DISPLAY_WIDTH];

	for (const auto& tri : tris) {
		float minX = std::min(std::min(tri.a.x, tri.b.x), tri.c.x);
		float minY = std::min(std::min(tri.a.y, tri.b.y), tri.c.y);
		float maxX = std::max(std::max(tri.a.x, tri.b.x), tri.c.x);
		float maxY = std::max(std::max(tri.a.y, tri.b.y), tri.c.y);

		int xBoundBot = std::max(static_cast<int>(round(minX)), 0);
		int yBoundBot = std::max(static_cast<int>(round(minY)), 0);
		int xBoundTop = std::min(static_cast<int>(round(maxX)), DISPLAY_WIDTH - 1);
		int yBoundTop = std::min(static_cast<int>(round(maxY)), DISPLAY_HEIGHT - 1);

		for (int y = yBoundBot; y <= yBoundTop; y++) {
			for (int x = xBoundBot; x <= xBoundTop; x++) {
				Vec2 point(static_cast<float>(x), static_cast<float>(y));
				if (tri.pointInTriangle(point)) {
					image[y][x].x = tri.colour.x;
					image[y][x].y = tri.colour.y;
					image[y][x].z = tri.colour.z;
				}
			}
		}
	}
	writeBitmap(image, "D:/New Projects/Programming/SoftwareRasteriser/output.bmp");
}

int main()
{
	const auto trianglePoints = readObj("D:/New Projects/Programming/SoftwareRasteriser/Resources/Cube.obj");

	std::vector<Vec3> triangleCols;
	for (int i = 0; i < trianglePoints.size() / 3; i++) {
		triangleCols.push_back(Vec3(0.0f, 1.0f, 0.0f));
	}

	Model cube(trianglePoints, triangleCols);
}
