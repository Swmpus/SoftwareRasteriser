#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "maths.hpp"

#define DISPLAY_HEIGHT 280
#define DISPLAY_WIDTH 240

void fileWriteBytes(std::ofstream& file, char* value, int length)
{
	file.write(value, length);
}

void fileWriteBytes(std::ofstream& file, uint32_t value)
{
	file.write(reinterpret_cast<const char*>(&value), 4);
}

void fileWriteBytes(std::ofstream& file, uint16_t value)
{
	file.write(reinterpret_cast<const char*>(&value), 2);
}

void fileWriteBytes(std::ofstream& file, uint8_t value)
{
	file.write(reinterpret_cast<const char*>(&value), 1);
}

void writeBitmap(Vec3 image[DISPLAY_HEIGHT][DISPLAY_WIDTH], std::string filePath)
{
	std::ofstream outputFile(filePath, std::ios::binary);
	// BMP header
	fileWriteBytes(outputFile, "BM", 2); // "BM" file header
	fileWriteBytes(outputFile, static_cast<uint32_t>(14 + 40 +  4 * DISPLAY_HEIGHT * DISPLAY_WIDTH)); // File size in bytes (4 bytes allocated)
	fileWriteBytes(outputFile, static_cast<uint32_t>(0));
	fileWriteBytes(outputFile, static_cast<uint32_t>(14 + 40)); // Data start offset
	// // DIB header
	fileWriteBytes(outputFile, static_cast<uint32_t>(40)); // Size of the DIB header
	fileWriteBytes(outputFile, static_cast<uint32_t>(DISPLAY_WIDTH));
	fileWriteBytes(outputFile, static_cast<uint32_t>(DISPLAY_HEIGHT));
	fileWriteBytes(outputFile, static_cast<uint16_t>(1)); // Number of colour planes? Not clear from wikipedia
	fileWriteBytes(outputFile, static_cast<uint16_t>(8 * 4)); // Bits per pixel
	fileWriteBytes(outputFile, static_cast<uint32_t>(0)); // Compression method (None)
	fileWriteBytes(outputFile, static_cast<uint32_t>(4 * DISPLAY_HEIGHT * DISPLAY_WIDTH)); // Image size in bytes (can be 0)
	fileWriteBytes(outputFile, static_cast<uint32_t>(0)); // Unused print/palette setting
	fileWriteBytes(outputFile, static_cast<uint32_t>(0)); // Unused print/palette setting
	fileWriteBytes(outputFile, static_cast<uint32_t>(0)); // Unused print/palette setting
	fileWriteBytes(outputFile, static_cast<uint32_t>(0)); // Unused print/palette setting
	// Data
	for (uint16_t y = 0; y < DISPLAY_HEIGHT; y++) {
		for (uint16_t x = 0; x < DISPLAY_WIDTH; x++) {
			fileWriteBytes(outputFile, static_cast<uint8_t>(image[y][x].z * 255));
			fileWriteBytes(outputFile, static_cast<uint8_t>(image[y][x].y * 255));
			fileWriteBytes(outputFile, static_cast<uint8_t>(image[y][x].x * 255));
			fileWriteBytes(outputFile, static_cast<uint8_t>(0)); // Padding
		}
	}
	outputFile.close();
}

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
	// Initialise a triangle
	Triangle tri(
		Vec2(0.3f * DISPLAY_WIDTH, 0.2f * DISPLAY_HEIGHT),
		Vec2(0.2f * DISPLAY_WIDTH, 0.4f * DISPLAY_HEIGHT),
		Vec2(0.4f * DISPLAY_WIDTH, 0.8f * DISPLAY_HEIGHT),
		Vec3(0, 1, 0)
	);

	std::vector<Triangle> tris(10);
	tris.push_back(tri);
	render(tris);
}
