#pragma once
#include <stdint.h>
#include <string>
//#include "RockBox/fixedpoint.h"

#define FIXED_POINT_DEFAULT_SCALE 256

template <uint32_t decScale = FIXED_POINT_DEFAULT_SCALE>
class FixedPoint {
	int32_t data;
public:
	FixedPoint() {
		data = 0;
	}
	FixedPoint(const char* str) {
		fromString(str);
	}
	/*FixedPoint(const FixedPoint& other) {
		uint32_t otherScale = other.getDecScale();
		fromString(other.getAsString());
		std::cout << "In FixedPoint(other)" << std::endl;
		std::cout << "this:" << getRaw() << ", other:" << other.getRaw() << std::endl;
		std::cout << "this:" << getAsInt() << ", other:" << other.getAsInt() << std::endl;
		std::cout << "this:" << getAsString() << ", other:" << other.getAsString() << std::endl;
	}*/
	FixedPoint(const FixedPoint<decScale>& other) {
		data = other.data;
	}
	FixedPoint(uint32_t other) {
		data = other * decScale;
	}
	FixedPoint(int32_t other) {
		data = other * decScale;
	}
	FixedPoint(float other) {
		data = other * decScale;
	}
	inline void operator+=(const FixedPoint<decScale>& other) {
		data = data + other.data;
	}
	inline void operator-=(const FixedPoint<decScale>& other) {
		data = data - other.data;
	}
	inline void operator*=(const FixedPoint<decScale>& other) {
		//TODO: does data need to be (int64_t)?
		data = (int64_t)((int64_t)data * other.data) / decScale;  //TODO: less accurate than the below?
		//auto value = (int64_t)(data * other.data) / (decScale / 2);
		//data = (value / 2) + (value % 2);
	}
	inline void operator/=(const FixedPoint<decScale>& other) {
		data = (int64_t)((int64_t)data * decScale) / other.data;
	}
	/*FixedPoint<decScale> operator+(int32_t other) const {
		//return data + (other.data / decScale);
	}
	FixedPoint<decScale> operator-(int32_t other) const {
		//return data - (other.data / decScale);
	}
	FixedPoint<decScale> operator*(int32_t other) const {
		return (data * other.data) / decScale;
	}
	FixedPoint<decScale> operator/(int32_t other) const {
		return (data / other.data) * decScale;
	}*/
	FixedPoint<decScale> operator+(const FixedPoint<decScale>& other) const {
		FixedPoint<decScale> retValue; retValue.setRaw((data + other.data));
		return retValue;
	}
	FixedPoint<decScale> operator-(const FixedPoint<decScale>& other) const {
		FixedPoint<decScale> retValue; retValue.setRaw((data - other.data));
		return retValue;
	}
	FixedPoint<decScale> operator*(const FixedPoint<decScale>& other) const {
		FixedPoint<decScale> retValue; retValue.setRaw((int64_t)((int64_t)data * other.data) / decScale);
		return retValue;
	}
	FixedPoint<decScale> operator/(const FixedPoint<decScale>& other) const {
		FixedPoint<decScale> retValue; retValue.setRaw((int64_t)((int64_t)data * decScale) / other.data);
		return retValue;
	}
	inline void operator++() {
		data += decScale;
	}
	inline void operator--() {
		data -= decScale;
	}
	inline FixedPoint<decScale> operator-() const noexcept {
		FixedPoint retValue; retValue.setRaw(-data);
		return retValue;
	}
	inline bool operator<=(FixedPoint<decScale> other) const {
		if (this->data <= other.data) return true;
		return false;
	}
	inline bool operator>=(FixedPoint<decScale> other) const {
		if (this->data >= other.data) return true;
		return false;
	}
	inline bool operator==(FixedPoint<decScale> other) const {
		if (this->data == other.data) return true;
		return false;
	}
	inline bool operator<(FixedPoint<decScale> other) const {
		if (this->data < other.data) return true;
		return false;
	}
	inline bool operator>(FixedPoint<decScale> other) const {
		if (this->data > other.data) return true;
		return false;
	}
	inline bool operator!=(FixedPoint<decScale> other) const {
		if (this->data != other.data) return true;
		return false;
	}
	inline FixedPoint<decScale> getABS() const {
		if (this->data >= 0) return *this;
		FixedPoint<decScale> retValue; retValue.data = -retValue.data;
		return retValue;
	}
	inline int32_t getAsInt() const {
		return data / decScale;
	}
	inline float getAsFloat() const {
		if (data == 0) return 0;  //TODO: consider if this is needed and where else it may be needed to avoid runtime crashing.
		return ((float)(int32_t)data) / decScale;
	}
	inline int32_t getRaw() const {
		return data;
	}
	inline void setRaw(int32_t other) {
		data = other;
	}
	inline std::string getAsString() const {
		std::string retValue = "";
		int32_t whole = data / decScale;
		int32_t remainder = data % decScale;
		//float dec = remainder/decScale;
		int32_t dec = (data*100) / decScale;

		retValue += std::to_string(whole);
		retValue += ".";
		retValue += std::to_string(dec);
		std::cout << whole << std::endl;
		std::cout << remainder << std::endl;
		std::cout << dec << std::endl;
		std::cout << data << std::endl;
		return retValue;

		//data = 1300
		//decScale = 512
		//remainder = 276
		//realResult = 2.5390625
		//result = "253"

		//data = 33
		//decScale = 512
		//remainder = 33
		//realResult = 0.064453125
		//result = "6"
		
		//data = 30
		//decScale = 100
		//remainder = 30
		//result = "3"
	}
	inline bool fromString(const std::string& _str) {
		std::string str = _str;
		char lastChar = str[str.size() - 1];
		if (lastChar != 'f' && lastChar != 'F')
			return false;
		str.resize(str.size() - 1);
		int32_t dotPos = -1;
		uint32_t strSize = (uint32_t)str.size();
		for (uint32_t i = 0; i < strSize; i++) {
			if (str[i] == '.') {
				dotPos = i;
				break;
			}
		}
		if (dotPos == -1) return false;
		int32_t wholeValue = 0;
		for (uint32_t i = 0; i < dotPos; i++) {
			if (str[i] < '0' || str[i] > '9') return false;
			uint32_t digitCount = dotPos - i;
			int32_t digitValue = 1;
			for (uint32_t j = 1; j < digitCount; j++)
				digitValue *= 10;
			wholeValue += (digitValue)*(uint32_t)(str[i] - '0');
		}
		int32_t decimalNumbers = 0;
		/*for (uint32_t i = dotPos + 1; i < str.size(); i++) {
			if (str[i] < '0' || str[i] > '9') return false;
			uint32_t digitCount = (str.size() - 1) - i;
			int32_t digitValue = 1;
			for (uint32_t j = 0; j < digitCount; j++)
				digitValue *= 10;
			decimalNumbers += digitValue * (uint32_t)(str[i] - '0');
		}*/
		for (uint32_t i = dotPos + 1; i < str.size(); i++) {
			if (str[i] < '0' || str[i] > '9') return false;
			uint32_t digitCount = (str.size()) - i;
			int32_t digitValue = 1;
			for (uint32_t j = 1; j < digitCount; j++)
				digitValue *= 10;
			decimalNumbers += digitValue * (uint32_t)(str[i] - '0');
		}

		int32_t totalValue = wholeValue * decScale;
		int32_t decDigitOffset = 1;
		for (uint32_t i = dotPos+1; i < str.size(); i++)
			decDigitOffset *= 10;
		totalValue += ((decimalNumbers * decScale) / decDigitOffset);
		data = totalValue;
		return true;
	}
	inline const uint32_t getDecScale() const {
		return decScale;
	}
};


constexpr uint32_t _FP_MAX_SIN_INPUT = 7;
extern FixedPoint<256 * 256> _sinLookupTable16[256 * 256 * 2 * _FP_MAX_SIN_INPUT];
extern FixedPoint<256 * 256> _cosinLookupTable16[256 * 256 * 2 * _FP_MAX_SIN_INPUT];
extern FixedPoint<256> _sinLookupTable8[256 * 2 * _FP_MAX_SIN_INPUT];
extern FixedPoint<256> _cosinLookupTable8[256 * 2 * _FP_MAX_SIN_INPUT];
extern void initFixedPointUtilities();
extern void setupFixedPointTableFiles();

template<uint32_t decScale>
inline FixedPoint<decScale> factorial(FixedPoint<decScale> val) {
	if (val == 0) {
		return 1;
	}
	return val * (factorial(val - 1));
}
template<uint32_t decScale>
inline FixedPoint<decScale> power(FixedPoint<decScale> val, FixedPoint<decScale> power) {
	FixedPoint<decScale> result = val;
	for (FixedPoint<decScale> i = 1; i < power; i+=1) {
		result = val * result;
	}
	return result;
}
template<uint32_t decScale>  //Potentially still doesn't work
inline FixedPoint<decScale> fmod(FixedPoint<decScale> a, FixedPoint<decScale> b) {
	//FixedPoint<decScale> frac = a / b;
	//int32_t floor;
	//if (frac > 0)
	//	floor = frac.getAsInt();
	//else {
	//	frac.setRaw(frac.getRaw() - 1);
	//	floor = frac.getAsInt();
	//}
	//return a - b * floor;

	//return ((FixedPoint<decScale>)(a.getRaw() % b.getRaw())) / decScale;

	auto result = a;
	while (result >= b) {
		auto scaledB = b;
		while (scaledB * 10 < result)
			scaledB *= 10;
		while (result > scaledB)
			result -= scaledB;
	}
	return result;
}

inline FixedPoint<256 * 256> sin16(FixedPoint<256 * 256> a) {
	if (a > 7 || a < -7) {
		std::cout << "Error: sin16() input invalid, should be between -7 and 7, value is: "
			<< a.getAsString();
		throw;
	}
	return sin(a.getAsFloat());
	int32_t index;
	if (a < 0) {
		a = -a;
		index = a.getRaw() + 256 * 256 * _FP_MAX_SIN_INPUT;
	} else
		index = a.getRaw();
	return _sinLookupTable16[index];
}
inline FixedPoint<256 * 256> cosin16(FixedPoint<256 * 256> a) {
	if (a > 7 || a < -7) {
		std::cout << "Error: cosin16() input invalid, should be between -7 and 7, value is: "
			<< a.getAsString();
		throw;
	}
	float f = a.getAsFloat();
	float fresult = cos(f);
	FixedPoint<256 * 256> retValue = fresult;
	return retValue;
	int32_t index;
	if (a < 0) {
		a = -a;
		index = a.getRaw() + 256 * 256 * _FP_MAX_SIN_INPUT;
	}
	else
		index = a.getRaw();
	return _cosinLookupTable16[index];
}

/*FixedPoint<> sqrt(FixedPoint<> number) {
	//return fp_sqrt(number.getRaw(), 8);
	return sqrt(number.getAsFloat());
}*/
inline int32_t sqrtI32(int32_t v) {
	uint32_t b = 1 << 30, q = 0, r = v;
	while (b > r)
		b >>= 2;
	while (b > 0) {
		uint32_t t = q + b;
		q >>= 1;
		if (r >= t) {
			r -= t;
			q += b;
		}
		b >>= 2;
	}
	return q;
}
template<class T>
inline T sqrt(const T& var) {
	int32_t value = var.getAsInt();  //or value = input.getAsRaw()/256;
	return sqrtI32(value);  //or retValue.setRaw(sqrt(value)*256);
	
	//int32_t value = var.getRaw();
	//return sqrtI32(value)/var.getDecScale();

	//int32_t value = var.getRaw();
	//return sqrtI32(value);
	
	//FixedPoint<> value;
	//value.fromString(std::to_string(sqrt(var.getAsFloat())));
	//return value;

	//return sqrt(var.getAsFloat());
};



#ifdef TESTING
#include <iostream>
inline std::string T_FixedPointInitialize() {
	FixedPoint<256> value;
	std::string retValue = "";
	value.fromString("321.123f");
	if (value.getRaw() != 82207) {  //About 321.121 in float
		retValue += "Expected value 321123 but got ";
		retValue += std::to_string(value.getRaw());
		return retValue;
	}
	return "";
}
inline std::string T_FixedPointGetAsString() {
	FixedPoint<100> value;
	value.fromString("0.3f");
	if (value.getAsString() != "0.3f") {
		std::string retValue = "Expected string 0.3f but got ";
		retValue += value.getAsString();
		return retValue;
	}
	return "";
}
inline std::string T_FixedPointArithmatic() {
	FixedPoint<> value;
	std::string retValue = "Within...\n";
	{
		retValue += "=MULTIPLY\n";
		value.fromString("5.5f");
		value *= FixedPoint<>(2);
		if (value.getAsString() != "11.0f") {
			retValue += "5.5f*2 should = 11.0f not ";
			retValue += value.getAsString(); retValue += '\n';
		}
	}
	{
		retValue += "=DIVISION\n";
		value.fromString("11.0f");
		value /= FixedPoint<>(2);
		if (value.getAsString() != "5.5f") {
			retValue += "11.0f/2 should = 5.5f not ";
			retValue += value.getAsString(); retValue += '\n';
		}
	}
	{
		retValue += "=ADDITION\n";
		value.fromString("5.5f");
		value += value;
		if (value.getAsString() != "11.0f") {
			retValue += "5.5f+5.5f should = 11.0f not ";
			retValue += value.getAsString(); retValue += '\n';
		}
	}
	{
		retValue += "=SUBTRACTION\n";
		value.fromString("11.0f");
		FixedPoint<> value2; value2.fromString("5.5f");
		value -= value2;
		if (value.getAsString() != "5.5f") {
			retValue += "11.0f-5.5f should = 5.5f not ";
			retValue += value.getAsString(); retValue += '\n';
		}
	}
	{
		retValue += "MULTIPLY\n";
		value.fromString("5.5f");
		value = value * FixedPoint<>(2);
		if (value.getAsString() != "11.0f") {
			retValue += "5.5f*2 should = 11.0f not ";
			retValue += value.getAsString(); retValue += '\n';
		}
	}
	{
		retValue += "DIVISION\n";
		value.fromString("11.0f");
		value = value / FixedPoint<>(2);
		if (value.getAsString() != "5.5f") {
			retValue += "11.0f/2 should = 5.5f not ";
			retValue += value.getAsString(); retValue += '\n';
		}
	}
	{
		retValue += "ADDITION\n";
		value.fromString("5.5f");
		value = value + value;
		if (value.getAsString() != "11.0f") {
			retValue += "5.5f+5.5f should = 11.0f not ";
			retValue += value.getAsString(); retValue += '\n';
		}
	}
	{
		retValue += "SUBTRACTION\n";
		value.fromString("11.0f");
		FixedPoint<> value2; value2.fromString("5.5f");
		value = value - value2;
		if (value.getAsString() != "5.5f") {
			retValue += "11.0f-5.5f should = 5.5f not ";
			retValue += value.getAsString(); retValue += '\n';
		}
	}
	return retValue;
}
inline std::string T_FixedPointNegativeArithmatic() {
	FixedPoint<> value;
	std::string retValue = "Within...\n";
	{
		retValue += "=MULTIPLY\n";
		value.fromString("5.5f");
		value = -value;
		value *= FixedPoint<>(2);
		if (value.getAsString() != "-11.0f") {
			retValue += "-5.5f*2 should = -11.0f not ";
			retValue += std::to_string(value.getAsFloat()); retValue += '\n';
		}
	}
	{
		retValue += "=DIVISION\n";
		value.fromString("11.0f");
		value = -value;
		value /= FixedPoint<>(2);
		if (value.getAsString() != "-5.5f") {
			retValue += "-11.0f/2 should = -5.5f not ";
			retValue += std::to_string(value.getAsFloat()); retValue += '\n';
		}
	}
	{
		retValue += "=ADDITION\n";
		value.fromString("5.5f");
		value = -value;
		value += value;
		if (value.getAsString() != "11.0f") {
			retValue += "-5.5f+-5.5f should = -11.0f not ";
			retValue += std::to_string(value.getAsFloat()); retValue += '\n';
		}
	}
	{
		retValue += "=SUBTRACTION\n";
		value.fromString("11.0f");
		FixedPoint<> value2; value2.fromString("5.5f");
		value = -value;
		value -= value2;
		if (value.getAsString() != "-16.5f") {
			retValue += "-11.0f-5.5f should = -16.5f not ";
			retValue += std::to_string(value.getAsFloat()); retValue += '\n';
		}
	}
	return retValue;
}
inline std::string T_FixedPointSqrt() {
	FixedPoint<> value;
	value.fromString("9.5f"); 
	value = sqrt(value);
	float comparedTo = sqrt(9.5f);
	if (value.getAsString() != std::to_string(comparedTo)) {
		std::string retValue = "Expected value ";
		retValue += std::to_string(comparedTo);
		retValue += " instead got ";
		retValue += value.getAsString();
		return retValue;
	}
	return "";
}
#endif
#ifdef TESTING
inline void testFixedPoint() {
	std::string dbgStr = "";
	dbgStr = T_FixedPointInitialize();
	if (dbgStr != "") {
		std::cout << "T_FixedPointInitialize(): ";
		std::cout << dbgStr << std::endl;
		dbgStr = "";
	}
	dbgStr = T_FixedPointGetAsString();
	if (dbgStr != "") {
		std::cout << "T_FixedPointGetAsString(): ";
		std::cout << dbgStr << std::endl;
		dbgStr = "";
	}
	dbgStr = T_FixedPointArithmatic();
	if (dbgStr != "") {
		std::cout << "T_FixedPointArithmatic(): ";
		std::cout << dbgStr << std::endl;
	}
	dbgStr = T_FixedPointNegativeArithmatic();
	if (dbgStr != "") {
		std::cout << "T_FixedPointNegativeArithmatic(): ";
		std::cout << dbgStr << std::endl;
	}
	dbgStr = T_FixedPointSqrt();
	if (dbgStr != "") {
		std::cout << "T_FixedPointSqrt(): ";
		std::cout << dbgStr << std::endl;
	}
}
#endif