#include "FixedPoint.h"
#include <fstream>
#include <array>

std::array<FixedPoint<256 * 256>, 256 * 256 * 2 * _FP_MAX_SIN_INPUT> _sinLookupTable16;
std::array<FixedPoint<256 * 256>, 256 * 256 * 2 * _FP_MAX_SIN_INPUT> _cosinLookupTable16;
std::array<FixedPoint<256>, 256 * 2 * _FP_MAX_SIN_INPUT> _sinLookupTable8;
std::array<FixedPoint<256>, 256 * 2 * _FP_MAX_SIN_INPUT> _cosinLookupTable8;

void initFixedPointUtilities() {
	std::string path = "./data/lookupTables/sin16_7.txt";
	std::ifstream file;
	file.open(path);
	if (file.is_open() == false) {
		std::cout << "Error: initFixedPointUtilities() couldn't access file " << path;
		throw;
	}
	std::string line;
	uint32_t i = 0;
	while (getline(file, line)) {
		FixedPoint<256 * 256> fp;
		if (fp.fromString(line)) {
			std::cout << "Error: initFixedPointUtilities() input file is invalid reading " << line;
			throw;
		}
		_sinLookupTable16[i] = fp;
		i++;
	}
	file.close();
	path = "./data/lookupTables/cosin16_t.txt";
	file.open(path);
	if (file.is_open() == false) {
		std::cout << "Error: initFixedPointUtilities() couldn't access file " << path;
		throw;
	}
	line;
	i = 0;
	while (getline(file, line)) {
		FixedPoint<256 * 256> fp;
		if (fp.fromString(line)) {
			std::cout << "Error: initFixedPointUtilities() input file is invalid reading " << line;
			throw;
		}
		_cosinLookupTable16[i] = fp;
		i++;
	}
	file.close();
}

void setupFixedPointTableFiles() {
    
	FixedPoint<256 * 256>
        min = -static_cast<int32_t>(_FP_MAX_SIN_INPUT),
        max = static_cast<int32_t>(_FP_MAX_SIN_INPUT);
	uint32_t lastValue = max.getRaw();
	for (uint32_t i = 0; i <= lastValue; i++) {
		FixedPoint<256 * 256> fp;
		fp.setRaw(i);
		float asFloat = fp.getAsFloat();
		float result = sin(asFloat);
		_sinLookupTable16[i] = result;
		result = cos(asFloat);
		_cosinLookupTable16[i] = result;
	}
	uint32_t startingIndex = lastValue + 1;
	uint32_t endingIndex = lastValue + lastValue;
	for (uint32_t i = startingIndex; i <= endingIndex; i++) {
		FixedPoint<256 * 256> fp;
		fp.setRaw(i-startingIndex);
		float asFloat = fp.getAsFloat();
		asFloat = -asFloat;
		float result = sin(asFloat);
		_sinLookupTable16[i - 1] = result;
		result = cos(asFloat);
		_cosinLookupTable16[i - 1] = result;
	}
}