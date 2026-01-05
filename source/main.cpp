#include <iostream>
#include <fstream>
#include <string>
#include "maths.hpp"

#define DISPLAY_HEIGHT 280
#define DISPLAY_WIDTH 240

bool pointOnRightSideOfLine(const Vec2& a, const Vec2& b, const Vec2& point)
{
	Vec2 ap = point - a;
	Vec2 abPerp = (b - a).perpendicular();
	return ap.dot(abPerp) >= 0;
}

bool pointInTriangle(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& point)
{
	bool sideAB = pointOnRightSideOfLine(a, b, point);
	bool sideBC = pointOnRightSideOfLine(b, c, point);
	bool sideCA = pointOnRightSideOfLine(c, a, point);
	return sideAB == sideBC && sideBC == sideCA;
}

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

int main()
{
	Vec3 image[DISPLAY_HEIGHT][DISPLAY_WIDTH];

	// Initialise a triangle
	Vec2 a(0.2f * DISPLAY_WIDTH, 0.2f * DISPLAY_HEIGHT);
	Vec2 b(0.7f * DISPLAY_WIDTH, 0.4f * DISPLAY_HEIGHT);
	Vec2 c(0.4f * DISPLAY_WIDTH, 0.8f * DISPLAY_HEIGHT);

	for (uint16_t y = 0; y < DISPLAY_HEIGHT; y++) {
		for (uint16_t x = 0; x < DISPLAY_WIDTH; x++) {
			Vec2 point(static_cast<float>(x), static_cast<float>(y));
			if (pointInTriangle(a, b, c, point)) {
				image[y][x].x = 0;
				image[y][x].y = 0;
				image[y][x].z = 1;
			}
		}
	}
	writeBitmap(image, "D:/New Projects/Programming/SoftwareRasteriser/output.bmp");
}
