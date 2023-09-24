#pragma once
#include <iostream>
#include "FixedPoint.h"

template<typename T>
struct Vec2D {
	T x, y;

	template<class U>
    __forceinline Vec2D& assign(glm::vec<2, U> vec)
    {
    	// I did not want to disable implicit aggregate initialization
		// so I used assign instead of ctor
        x = static_cast<T>(vec.x);
        y = static_cast<T>(vec.y);
        return *this;
    }

	__forceinline void operator+=(const Vec2D& other) {
		x += other.x;
		y += other.y;
	}
	__forceinline void operator+=(T num) {
		x += num;
		y += num;
	}
	__forceinline void operator-=(const Vec2D& other) {
		x -= other.x;
		y -= other.y;
	}
	__forceinline void operator-=(T num) {
		x -= num;
		y -= num;
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
	__forceinline Vec2D<T> operator+(T num) const {
		return { x + num, y + num };
	}
	__forceinline Vec2D<T> operator-(const Vec2D& other) const {
		return { x - other.x, y - other.y };
	}
	__forceinline Vec2D<T> operator-(T num) const {
		return { x - num, y - num };
	}
	__forceinline Vec2D<T> operator/(const T num) const {
		return { x / num, y / num };
	}
	__forceinline Vec2D<T> operator*(const T num) const {
		return { x * num, y * num };
	}
	__forceinline bool operator>(const Vec2D& other) const {
		T a = x + y;
		T b = other.x + other.y;
		return a > b;
	}
	__forceinline bool operator>=(const Vec2D& other) const {
		T a = x + y;
		T b = other.x + other.y;
		return a >= b;
	}
	__forceinline bool operator<(const Vec2D& other) const {
		T a = x + y;
		T b = other.x + other.y;
		return a < b;
	}
	__forceinline bool operator<=(const Vec2D& other) const {
		T a = x + y;
		T b = other.x + other.y;
		return a <= b;
	}
	__forceinline bool isEqual(const Vec2D& other) const {
        if (x == other.x && y == other.y)
            return true;
        return false;
	}
	__forceinline bool isZero() const {
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
	__forceinline bool isBetween(Vec2D& start, Vec2D& end) {
		if (x < start.x || x > end.x || y < start.y || y > end.y)
			return false;
		return true;
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
	__forceinline void operator+=(T num) {
		x += num;
		y += num;
		z += num;
	}
	__forceinline void operator-=(const Vec3D& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}
	__forceinline void operator-=(T num) {
		x -= num;
		y -= num;
		z -= num;
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
	__forceinline Vec3D<T> operator+(T num) const {
		return { x + num, y + num, z + num };
	}
	__forceinline Vec3D<T> operator-(const Vec3D& other) const {
		return { x - other.x, y - other.y, z - other.z };
	}
	__forceinline Vec3D<T> operator-(T num) const {
		return { x + num, y + num, z + num };
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
	__forceinline bool operator>(const Vec3D& other) const {
		T a = x + y + z;
		T b = other.x + other.y + other.z;
		return a > b;
	}
	__forceinline bool operator>=(const Vec3D& other) const {
		T a = x + y + z;
		T b = other.x + other.y + other.z;
		return a >= b;
	}
	__forceinline bool operator<(const Vec3D& other) const {
		T a = x + y + z;
		T b = other.x + other.y + other.z;
		return a < b;
	}
	__forceinline bool operator<=(const Vec3D& other) const {
		T a = x + y + z;
		T b = other.x + other.y + other.z;
		return a <= b;
	}
	__forceinline bool isEqual(const Vec3D& other) const {
        if (x == other.x && y == other.y && z == other.z)
            return true;
        return false;
	}
	__forceinline bool isZero() const {
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
	__forceinline bool isBetween(const Vec3D<T>& min, const Vec3D<T>& max) {
		if (x < min.x || x > max.x ||
			y < min.y || y > max.y ||
			z < min.z || z > max.z)
			return false;
		return true;
	}
	__forceinline T getDistance() {
		return sqrt((x * x) + (y * y) + (z * z));
	}
	__forceinline uint64_t getDistanceSquared() {
		int64_t retValue = (x * x) + (y * y) + (z * z);
		if (retValue < 0) return -retValue;
		return retValue;
	}
	__forceinline uint64_t getDistanceSquaredFP() {
		int64_t retValue = 
			(int64_t)(x.getRaw() * (int64_t)x.getRaw()) +
			(int64_t)(y.getRaw() * (int64_t)y.getRaw()) +
            ((int64_t)z.getRaw() * (int64_t)z.getRaw());
		if (retValue < 0) return -retValue;
		return retValue;
	}
	__forceinline T getDistanceFrom(Vec3D& other) {
		return (*this - other).getDistance();
	}
	__forceinline uint64_t getDistanceFromSquared(Vec3D& other) {
		return (*this - other).getDistanceSquared();
	}
	void cross(Vec3D<T> a, Vec3D<T> b) {
		x = a.y * b.z - a.z * b.y;
		y = -(a.x * b.z - a.z * b.x);
		z = a.x * b.y - a.y * b.x;
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