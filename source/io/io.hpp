#include <fstream>
#include <iostream>
#include <sstream>
#include <maths.hpp>

#ifndef DISPLAY_HEIGHT
#define DISPLAY_HEIGHT 280
#endif
#ifndef DISPLAY_WIDTH
#define DISPLAY_WIDTH 240
#endif

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

std::vector<std::string> splitString(const std::string v, const char s)
{
	std::stringstream ss(v);
	std::string split;
	std::vector<std::string> splits;
    while (getline(ss, split, s)) {
    	splits.push_back(split);
    }
    return splits;
}

std::vector<Vec3> readObj(const std::string filePath)
{
	std::ifstream file(filePath);
	std::vector<Vec3> allPoints;
	std::vector<Vec3> trianglePoints;

	std::string line;
	while (std::getline(file, line)) {
    	std::cout << line << std::endl;
	    if (line.substr(0, 2) == "v ") {
	    	const auto splits = splitString(line.substr(2, line.size() - 2), ' ');
		    allPoints.push_back(
		    	Vec3(
		    		std::stof(splits[0]),
		    		std::stof(splits[1]),
		    		std::stof(splits[2])
		    		)
		    	);
	    } else if (line.substr(0, 2) == "f ") {
	    	const auto indexGroups = splitString(line.substr(2, line.size() - 2), ' ');
	    	for (int i = 0; i < indexGroups.size(); i++) {
	    		const int newIndex = std::stoi(splitString(indexGroups[i], '/')[0]) - 1;  // Face indices start aat 1
	    		if (i >= 3) {
	    			trianglePoints.push_back(trianglePoints[trianglePoints.size() - 3]);
	    			trianglePoints.push_back(trianglePoints[trianglePoints.size() - 2]);
	    		}
	    		trianglePoints.push_back(allPoints[newIndex]);
	    	}

	    	for (const Vec3 p : trianglePoints) {
	    		std::cout << "(" << p.x << ", " << p.y << ", " << p.z << ")";
	    	}
	    	std::cout << std::endl;
	    }
	}
	return trianglePoints;
}
