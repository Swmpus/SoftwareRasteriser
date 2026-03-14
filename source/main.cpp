#include <string>
#include <vector>
#include <ctime> 
#include <cmath>
#include "maths.hpp"
#include "io.hpp"
#include "geometry.hpp"

#ifndef FOV
#define FOV 60
#endif
#ifndef DISPLAY_HEIGHT
#define DISPLAY_HEIGHT 280
#endif
#ifndef DISPLAY_WIDTH
#define DISPLAY_WIDTH 240
#endif

Vec3 WorldToScreeen(const Vec3 &objectPoint, const Transform transform, const Vec2 screenPixelShape) {
	Vec3 worldPoint = transform.transformVector(objectPoint);

	float screenHeightToWorld = tan(degToRad(FOV) / 2) * 2; // Screen height in world space
	 // Calculate a conversion factor between metres and pixels
	float pixelsPerWorldUnit = screenPixelShape.y / screenHeightToWorld / worldPoint.z;

	Vec2 pixelOffset(worldPoint.x, worldPoint.y);
	Vec2 pixelCoords = pixelOffset * pixelsPerWorldUnit + screenPixelShape / 2;
	return Vec3(pixelCoords.x, pixelCoords.y, worldPoint.z);
}

void renderToTarget(const Model model, RenderTarget& target)
{
	const Vec2 screenPixelShape(
		static_cast<float>(target.width),
		static_cast<float>(target.height));

	// This function draws to target as a side-effect, deliberately
	for (int i = 0; i * 3 < model.points.size(); i++) {
		const Vec3 a = WorldToScreeen(model.points[i * 3], model.transform, screenPixelShape);
		const Vec3 b = WorldToScreeen(model.points[i * 3 + 1], model.transform, screenPixelShape);
		const Vec3 c = WorldToScreeen(model.points[i * 3 + 2], model.transform, screenPixelShape);

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
				auto weights = Vec3();
				if (pointInTriangle(a.asVec2(), b.asVec2(), c.asVec2(), point, weights)) {
					Vec3 depths(a.z, b.z, c.z);
					uint32_t pixelIndex = y * target.width + x;
					float currentDepth = target.depthBuffer[pixelIndex];
					float newDepth = depths.dot(weights);
					// TODO Come up with a better placeholder than 0
					if (currentDepth == 0 || newDepth <= currentDepth)
						// TODO Need to be sure that this is by value, and not by reference
						target.image[pixelIndex] = model.triCols[i];
						target.depthBuffer[pixelIndex] = newDepth;
				}
			}
		}
	}
}

int main()
{
	// Seeed the random number generator
	std::srand((unsigned) std::time(0));

	// Read in an object to display
	const auto trianglePoints = readObj("D:/New Projects/Programming/SoftwareRasteriser/Resources/biplane.obj");
	// Generate random colours for the triangles of the object
	std::vector<Vec3> triangleCols;
	for (int i = 0; i < trianglePoints.size() / 3; i++) {
		triangleCols.push_back(
			Vec3(
				(float) rand() / RAND_MAX,
				(float) rand() / RAND_MAX,
				(float) rand() / RAND_MAX));
	}
	Vec3 cubePos(0.0f, 0.0f, 7.0f);
	const Transform objectTransform (degToRad(45), degToRad(45), degToRad(45), cubePos);
	Model object(trianglePoints, triangleCols, objectTransform);

	RenderTarget renderTarget(DISPLAY_HEIGHT, DISPLAY_WIDTH);

	// Actually render an image of the model to the target
	renderToTarget(object, renderTarget);

	// Then write the image to disk
	writeBitmap(renderTarget, "D:/New Projects/Programming/SoftwareRasteriser/output.bmp");
}
