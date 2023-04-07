#pragma once
#include <iostream>
#include "FixedPoint.h"

template<typename T>
struct Vec2D {
	T x, y;
	__forceinline void operator+=(const Vec2D& other) {
		x += other.x;
		y += other.y;
	}
	__forceinline void operator-=(const Vec2D& other) {
		x -= other.x;
		y -= other.y;
	}
	__forceinline void operator/=(T num) {
		x /= num;
		y /= num;
	}
	__forceinline void operator*=(T num) {
		x = (x * num);
		y = (y * num);
	}
	__forceinline Vec2D<T> operator+(const Vec2D& other) const {
		return { x + other.x, y + other.y };
	}
	__forceinline Vec2D<T> operator-(const Vec2D& other) const {
		return { x - other.x, y - other.y };
	}
	__forceinline Vec2D<T> operator/(const T num) const {
		return { x / num, y / num };
	}
	__forceinline Vec2D<T> operator*(const T num) const {
		return { x * num, y * num };
	}
	__forceinline bool isZero() {
		if (x == 0 && y == 0)
			return true;
		return false;
	}
	__forceinline void normalize(uint32_t unit = 1) {
		T dist = getDistance();
		if (dist == 0) {
			x = y = 0;
			return;
		}
		x /= dist / unit; y /= dist / unit;
	}
	__forceinline void floor(T value) {
		if (x < value) x = value;
		if (y < value) y = value;
	}
	__forceinline void ceil(T value) {
		if (x > value) x = value;
		if (y > value) y = value;
	}
	__forceinline void bound(T floor, T ceil) {
		if (x < floor) x = floor;
		if (x > ceil) x = ceil;
		if (y < floor) y = floor;
		if (y > ceil) y = ceil;
	}
	__forceinline T getDistance() {
		return sqrt((x * x) + (y * y));
	}
	__forceinline uint64_t getDistanceSquared() {
		int64_t retValue = (x * x) + (y * y);
		if (retValue < 0) return -retValue;
		return retValue;
	}
	__forceinline T getDistanceFrom(Vec2D& other) {
		return (*this - other).getDistance();
	}
	__forceinline uint64_t getDistanceFromSquared(Vec2D& other) {
		return (*this - other).getDistanceSquared();
	}
	template<typename T>
	std::string getDebugStr() {
		std::string retValue = std::to_string(x);
		retValue += ", ";
		retValue += std::to_string(y);
		return retValue;
	}
	template<>
	std::string getDebugStr<FixedPoint<>>()	{
		std::string retValue = x.getAsString();
		retValue += ", ";
		retValue += y.getAsString();
		return retValue;
	}
	/*template<>
	std::string getDebugStr<float>() {
		std::string retValue = std::to_string(x);
		retValue += ", ";
		retValue += std::to_string(y);
		return retValue;
	}*/
};

template<typename T>
struct Vec3D {
	T x, y, z;
	__forceinline void operator+=(const Vec3D& other) {
		x += other.x;
		y += other.y;
		z += other.z;
	}
	__forceinline void operator-=(const Vec3D& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}
	__forceinline void operator/=(T num) {
		x /= num;
		y /= num;
		z /= num;
	}
	__forceinline void operator*=(T num) {
		x = (x * num);
		y = (y * num);
		z = (z * num);
	}
	__forceinline void operator*=(const Vec3D& other) {
		x = (x * other.x);
		y = (y * other.y);
		z = (z * other.z);
	}
	__forceinline Vec3D<T> operator+(const Vec3D& other) const {
		return { x + other.x, y + other.y, z + other.z };
	}
	__forceinline Vec3D<T> operator-(const Vec3D& other) const {
		return { x - other.x, y - other.y, z - other.z };
	}
	__forceinline Vec3D<T> operator/(const T num) const {
		return { x / num, y / num, z / num };
	}
	__forceinline Vec3D<T> operator*(const T num) const {
		return { x * num, y * num, z * num };
	}
	__forceinline Vec3D<T> operator*(const Vec3D& other) const {
		return { x * other.x, y * other.y, z * other.z };
	}
	__forceinline bool isZero() {
		if (x == 0 && y == 0 && z == 0)
			return true;
		return false;
	}
	__forceinline void normalize(uint32_t unit = 1) {
		T dist = getDistance();
		if (dist == 0) {
			x = y = z = 0;
			return;
		}
		x /= dist / unit; y /= dist / unit; z /= dist;
	}
	__forceinline void floor(T value) {
		if (x < value) x = value;
		if (y < value) y = value;
		if (z < value) z = value;
	}
	__forceinline void ceil(T value) {
		if (x > value) x = value;
		if (y > value) y = value;
		if (z > value) z = value;
	}
	__forceinline void bound(T floor, T ceil) {
		if (x < floor) x = floor;
		if (x > ceil) x = ceil;
		if (y < floor) y = floor;
		if (y > ceil) y = ceil;
		if (z < floor) z = floor;
		if (z > ceil) z = ceil;
	}
	__forceinline T getDistance() {
		return sqrt((x * x) + (y * y) + (z * z));
	}
	__forceinline uint64_t getDistanceSquared() {
		int64_t retValue = (x * x) + (y * y) + (z * z);
		if (retValue < 0) return -retValue;
		return retValue;
	}
	__forceinline T getDistanceFrom(Vec3D& other) {
		return (*this - other).getDistance();
	}
	__forceinline uint64_t getDistanceFromSquared(Vec3D& other) {
		return (*this - other).getDistanceSquared();
	}
	template<typename T>
	std::string getDebugStr() {
		std::string retValue = std::to_string(x);
		retValue += ", ";
		retValue += std::to_string(y);
		retValue += ", ";
		retValue += std::to_string(z);
		return retValue;
	}
	template<>
	std::string getDebugStr<FixedPoint<>>() {
		std::string retValue = x.getAsString();
		retValue += ", ";
		retValue += y.getAsString();
		retValue += ", ";
		retValue += z.getAsString();
		return retValue;
	}
};



inline std::string T_Vec2DNormalizeFixedPoint() {
	Vec2D<FixedPoint<>> vec = { 5, 10 };
	vec.normalize();
	if (vec.getDebugStr<FixedPoint<>>() != "0.5, 1") {
		std::string retValue = "Expected value ";
		retValue += "0.5, 1";
		retValue += " instead got ";
		retValue += vec.getDebugStr<FixedPoint<>>();
		return retValue;
	}
	return "";
}
inline std::string T_Vec2DNormalize() {
	Vec2D<float> vec = { 5, 10 };
	vec.normalize();
	if (vec.getDebugStr<float>() != "0.5, 1") {
		std::string retValue = "Expected value ";
		retValue += "0.5, 1";
		retValue += " instead got ";
		retValue += vec.getDebugStr<float>();
		return retValue;
	}
	return "";
}
inline std::string T_Vec2DNormalizeDouble() {
	Vec2D<double> vec = { 5, 10 };
	vec.normalize();
	if (vec.getDebugStr<double>() != "0.5, 1") {
		std::string retValue = "Expected value ";
		retValue += "0.5, 1";
		retValue += " instead got ";
		retValue += vec.getDebugStr<double>();
		return retValue;
	}
	return "";
}
inline void testVec2D() {
	std::string dbgStr = "";
	dbgStr = T_Vec2DNormalizeFixedPoint();
	if (dbgStr != "") {
		std::cout << "T_Vec2DNormalizeFixedPoint(): ";
		std::cout << dbgStr << std::endl;
		dbgStr = "";
	}
	dbgStr = T_Vec2DNormalize();
	if (dbgStr != "") {
		std::cout << "T_Vec2DNormalize(): ";
		std::cout << dbgStr << std::endl;
		dbgStr = "";
	}
	dbgStr = T_Vec2DNormalizeDouble();
	if (dbgStr != "") {
		std::cout << "T_Vec2DNormalizeDouble(): ";
		std::cout << dbgStr << std::endl;
		dbgStr = "";
	}
}