#include "Profiler.h"
#include <string>
#include <unordered_map>
#include <set>
#include <iostream>

std::unordered_map<std::string, uint64_t> logs;

void profilerLog(const char* name, uint64_t ticksToAdd) {
	if (auto log = logs.find(name) == logs.end()) {
		log = ticksToAdd;
		return;
	}
	logs[name] += ticksToAdd;
}
void profilerDisplayLogs() {
	std::set<std::string> found;
	size_t size = logs.size();
	for (size_t i = 0; i < size; i++) {
		uint64_t highestValue = 0;
		std::string name;
		for (auto& log : logs) {
			if (log.second > highestValue && found.find(log.first) == found.end()) {
				name = log.first;
			}
		}
		found.insert(name);
	}
	for (auto& f : found) {
		uint64_t ticks = logs[f];
		std::cout << f << " " << ticks << std::endl;
	}
}

std::string profileLinesDBGStr;
uint64_t profileLinesCPUCounter;
void _profileLinesStart(const char* file, const char* function, int line) {
	std::string str = file;
	str += " ";
	str += function;
	str += " ";
	str += std::to_string(line);
	profileLinesDBGStr = "";
	profileLinesDBGStr += str;
	profileLinesDBGStr += " <::> ";
	profileLinesCPUCounter = getCPUTicks();
}
void _profileLinesEnd(const char* file, const char* function, int line) {
	std::string str = file;
	str += " ";
	str += function;
	str += " ";
	str += std::to_string(line);
	profileLinesDBGStr += str;
	profileLinesCPUCounter = getCPUTicks() - profileLinesCPUCounter;
	profilerLog(profileLinesDBGStr.c_str(), profileLinesCPUCounter);
}