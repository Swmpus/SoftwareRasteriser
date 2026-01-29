#include <string>
#include <vector>
#include <ctime> 
#include "maths.hpp"
#include "io.hpp"
#include "geometry.hpp"

#ifndef DISPLAY_HEIGHT
#define DISPLAY_HEIGHT 280
#endif
#ifndef DISPLAY_WIDTH
#define DISPLAY_WIDTH 240
#endif

Vec2 WorldToScreeen(const Vec3 inputPoint) {
	Vec2 point(inputPoint.x, inputPoint.y);
	return point;
}

void renderToTarget(const Model model, RenderTarget target)
{
	for (int i = 0; i * 3 <= model.points.size() - 2; i += 3) {
		// TODO These need to be projected
		const Vec2 a = WorldToScreeen(model.points[i]);
		const Vec2 b = WorldToScreeen(model.points[i + 1]);
		const Vec2 c = WorldToScreeen(model.points[i + 2]);

		float minX = std::min(std::min(a.x, b.x), c.x);
		float minY = std::min(std::min(a.y, b.y), c.y);
		float maxX = std::max(std::max(a.x, b.x), c.x);
		float maxY = std::max(std::max(a.y, b.y), c.y);

		int xBoundBot = std::max(static_cast<int>(round(minX)), 0);
		int yBoundBot = std::max(static_cast<int>(round(minY)), 0);
		int xBoundTop = std::min(static_cast<int>(round(maxX)), target.width - 1);
		int yBoundTop = std::min(static_cast<int>(round(maxY)), target.height - 1);

		for (int y = yBoundBot; y <= yBoundTop; y++) {
			for (int x = xBoundBot; x <= xBoundTop; x++) {
				Vec2 point(static_cast<float>(x), static_cast<float>(y));
				if (pointInTriangle(a, b, c, point)) {
					// Need to be sure that this is by value, and not by reference
					target.image[y * target.width + x] = model.triCols[i];
				}
			}
		}
	}
	// TODO Need to find  out if the reference to target is actually used,
	// or if I need to return the data here
}

int main()
{
	// Seeed the random number generator
	std::srand((unsigned) std::time(0));

	// Read in an object to display
	const auto trianglePoints = readObj("D:/New Projects/Programming/SoftwareRasteriser/Resources/Cube.obj");
	// Generate random colours for the triangles of the object
	std::vector<Vec3> triangleCols;
	for (int i = 0; i < trianglePoints.size() / 3; i++) {
		triangleCols.push_back(
			Vec3(
				(float) rand() / RAND_MAX,
				(float) rand() / RAND_MAX,
				(float) rand() / RAND_MAX));
	}
	Model cube(trianglePoints, triangleCols);

	RenderTarget renderTarget(DISPLAY_HEIGHT, DISPLAY_WIDTH);

	// Actually render an image of the model to the target
	renderToTarget(cube, renderTarget);

	// Then write the image to disk
	writeBitmap(renderTarget, "D:/New Projects/Programming/SoftwareRasteriser/output.bmp");
}
