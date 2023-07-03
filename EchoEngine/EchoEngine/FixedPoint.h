#pragma once
#include <cstdint>
#include <string>
#include <iostream>
#include <array>

#define FIXED_POINT_DEFAULT_SCALE (256*256)

template <uint32_t decScale = FIXED_POINT_DEFAULT_SCALE>
class FixedPoint
{
	int32_t data_;
public:
	FixedPoint() {
		data_ = 0;
	}
	constexpr FixedPoint(const char* str) {
		fromString(str);
	}
	FixedPoint(const FixedPoint<decScale>& other) {
		data_ = other.data_;
	}
	FixedPoint(uint32_t other) {
		data_ = other * decScale;
	}
	FixedPoint(int32_t other) {
		data_ = other * decScale;
	}
	FixedPoint(float other) {
		data_ = static_cast<uint32_t>(other * decScale);
	}
	inline void operator+=(const FixedPoint<decScale>& other) {
		data_ = data_ + other.data_;
	}
	inline void operator-=(const FixedPoint<decScale>& other) {
		data_ = data_ - other.data_;
	}
	inline void operator*=(const FixedPoint<decScale>& other) {
		//TODO: does data need to be (int64_t)?
		data_ = (int64_t)((int64_t)data_ * other.data_) / decScale;  //TODO: less accurate than the below?
		//auto value = (int64_t)(data * other.data) / (decScale / 2);
		//data = (value / 2) + (value % 2);
	}
	inline void operator/=(const FixedPoint<decScale>& other) {
		data_ = (int64_t)((int64_t)data_ * decScale) / other.data_;
	}
	FixedPoint<decScale> operator+(const FixedPoint<decScale>& other) const {
		FixedPoint<decScale> retValue; retValue.setRaw((data_ + other.data_));
		return retValue;
	}
	FixedPoint<decScale> operator-(const FixedPoint<decScale>& other) const {
		FixedPoint<decScale> retValue; retValue.setRaw((data_ - other.data_));
		return retValue;
	}
	FixedPoint<decScale> operator*(const FixedPoint<decScale>& other) const {
		FixedPoint<decScale> retValue; retValue.setRaw((int64_t)((int64_t)data_ * other.data_) / decScale);
		return retValue;
	}
	FixedPoint<decScale> operator/(const FixedPoint<decScale>& other) const {
		FixedPoint<decScale> retValue; retValue.setRaw((int64_t)((int64_t)data_ * decScale) / other.data_);
		return retValue;
	}
	inline void operator++() {
		data_ += decScale;
	}
	inline void operator--() {
		data_ -= decScale;
	}
	inline FixedPoint<decScale> operator-() const noexcept {
		FixedPoint retValue; retValue.setRaw(-data_);
		return retValue;
	}
	inline bool operator<=(FixedPoint<decScale> other) const {
		if (this->data_ <= other.data_) return true;
		return false;
	}
	inline bool operator>=(FixedPoint<decScale> other) const {
		if (this->data_ >= other.data_) return true;
		return false;
	}
	inline bool operator==(FixedPoint<decScale> other) const {
		if (this->data_ == other.data_) return true;
		return false;
	}
	inline bool operator<(FixedPoint<decScale> other) const {
		if (this->data_ < other.data_) return true;
		return false;
	}
	inline bool operator>(FixedPoint<decScale> other) const {
		if (this->data_ > other.data_) return true;
		return false;
	}
	inline bool operator!=(FixedPoint<decScale> other) const {
		if (this->data_ != other.data_) return true;
		return false;
	}
	inline FixedPoint<decScale> getABS() const {
		if (this->data_ >= 0) return *this;
		FixedPoint<decScale> retValue; retValue.data_ = -retValue.data_;
		return retValue;
	}
	inline int32_t getAsInt() const {
		return data_ / decScale;
	}
	inline float getAsFloat() const {
		if (data_ == 0) return 0;  //TODO: consider if this is needed and where else it may be needed to avoid runtime crashing.
		return ((float)(int32_t)data_) / decScale;
	}
	inline int32_t getRaw() const {
		return data_;
	}
	inline void setRaw(int32_t other) {
		data_ = other;
	}
	inline bool hasDecimal() {
		return data_ % decScale;
	}
	inline std::string getAsString() const {
		std::string retValue = "";
		int32_t whole = data_ / decScale;
		int32_t remainder = data_ % decScale;
		//float dec = remainder/decScale;
		int32_t dec = (data_ * 100) / decScale;

		retValue += std::to_string(whole);
		retValue += ".";
		retValue += std::to_string(dec);
		return retValue;
	}
	constexpr bool fromString(const std::string& _str) {
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
		for (uint32_t i = 0; i < (uint32_t)dotPos; i++) {
			if (str[i] < '0' || str[i] > '9') return false;
			uint32_t digitCount = (uint32_t)dotPos - i;
			int32_t digitValue = 1;
			for (uint32_t j = 1; j < digitCount; j++)
				digitValue *= 10;
			wholeValue += (digitValue)*(uint32_t)(str[i] - '0');
		}
		int32_t decimalNumbers = 0;
		for (uint32_t i = (uint32_t)dotPos + 1; i < (uint32_t)str.size(); i++) {
			if (str[i] < '0' || str[i] > '9') return false;
			uint32_t digitCount = ((uint32_t)str.size()) - i;
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
		data_ = totalValue;
		return true;
	}
	inline const uint32_t getDecScale() const {
		return decScale;
	}
};


constexpr uint32_t _FP_MAX_SIN_INPUT = 7;
extern std::array<FixedPoint<256 * 256>, 256 * 256 * 2 * _FP_MAX_SIN_INPUT> _sinLookupTable16;
extern std::array<FixedPoint<256 * 256>, 256 * 256 * 2 * _FP_MAX_SIN_INPUT> _cosinLookupTable16;
extern std::array<FixedPoint<256>, 256 * 2 * _FP_MAX_SIN_INPUT> _sinLookupTable8;
extern std::array<FixedPoint<256>, 256 * 2 * _FP_MAX_SIN_INPUT> _cosinLookupTable8;
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
};

