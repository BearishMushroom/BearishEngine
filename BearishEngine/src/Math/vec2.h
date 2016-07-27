#ifndef _BEARISH_MATH_VEC2_H_
#define _BEARISH_MATH_VEC2_H_
	
#include "..\Core\IAllocatable.h"
#include "..\Types.h"
#include "..\Utils.h"

namespace Bearish { namespace Math {
	template<class T>
	class vec2_t : public Core::IAllocatable<vec2_t<T>> {
	public:
		union {
			struct {
				T x, y;
			};

			struct {
				T r, g;
			};

			struct {
				T s, t;
			};

			struct {
				T v[2];
			};
		};

		vec2_t(T x, T y) {
			this->x = x;
			this->y = y;
		}

		vec2_t(T x = 0) {
			this->x = x;
			this->y = x;
		}

		inline vec2_t<T> operator+(const vec2_t<T>& other) const {
			return vec2_t<T>(x + other.x, y + other.y);
		}

		vec2_t<T> Add(vec2_t<T> other) {
			return vec2_t<T>(x + other.x, y + other.y);
		}

		inline vec2_t<T> operator+(const T scalar) const {
			return vec2_t<T>(x + scalar, y + scalar);
		}

		inline vec2_t<T> operator-(const vec2_t<T>& other) const {
			return vec2_t<T>(x - other.x, y - other.y);
		}

		vec2_t<T> Subtract(vec2_t<T> other) {
			return vec2_t<T>(x - other.x, y - other.y);
		}

		inline vec2_t<T> operator-(const T scalar) const {
			return vec2_t<T>(x - scalar, y - scalar);
		}

		inline vec2_t<T> operator*(const vec2_t<T>& other) const {
			return vec2_t<T>(x * other.x, y * other.y);
		}

		vec2_t<T> Multiply(vec2_t<T> other) {
			return vec2_t<T>(x * other.x, y * other.y);
		}

		inline vec2_t<T> operator*(const T scalar) const {
			return vec2_t<T>(x * scalar, y * scalar);
		}

		inline vec2_t<T> operator/(const vec2_t<T>& other) const {
			return vec2_t<T>(x / other.x, y / other.y);
		}

		vec2_t<T> Divide(vec2_t<T> other) {
			return vec2_t<T>(x / other.x, y / other.y);
		}

		inline vec2_t<T> operator/(const T scalar) const {
			return vec2_t<T>(x / scalar, y / scalar);
		}

		inline void operator+=(const vec2_t<T>& other) {
			x += other.x;
			y += other.y;
		}

		inline void operator+=(const T scalar) {
			x += scalar;
			y += scalar;
		}

		inline void operator-=(const vec2_t<T>& other) {
			x -= other.x;
			y -= other.y;
		}

		inline void operator-=(const T scalar) {
			x -= scalar;
			y -= scalar;
		}

		inline void operator*=(const vec2_t<T>& other) {
			x *= other.x;
			y *= other.y;
		}

		inline void operator*=(const T scalar) {
			x *= scalar;
			y *= scalar;
		}

		inline void operator/=(const vec2_t<T>& other) {
			x /= other.x;
			y /= other.y;
		}

		inline void operator/=(const T scalar) {
			x /= scalar;
			y /= scalar;
		}

		inline bool operator==(const vec2_t<T>& other) const {
			return (x == other.x && y == other.y);
		}

		inline T operator[](const usize index) const {
			return v[index];
		}

		inline T Max() const {
			return __max(x, y);
		}

		inline T Min() const {
			return __min(x, y);
		}

		inline T Dot(const vec2_t& other) const {
			return x * other.x + y * other.y;
		}

		inline T Cross(const vec2_t& other) const {
			return x * other.y - y * other.x;
		}

		inline T Length() const {
			return static_cast<T>(sqrt(x * x + y * y));
		}

		inline vec2_t<T> Normalize() {
			T length = Length();
			operator/=(length);
			return *this;
		}

		inline vec2_t<T> Rotate(T degrees) const {
			T rad = static_cast<T>(degrees * RADIANS);
			T cosr = static_cast<T>(cos(rad));
			T sinr = static_cast<T>(sin(rad));

			return vec2_t<T>(x * cosr - y * sinr, x * cosr + y * sinr);
		}

		inline vec2_t<T> Lerp(const vec2_t& destination, const T factor) const {
			return (destination - (*this)) * factor + (*this);
		}

		inline string ToString() {
			return "{x: " + std::to_string(x) + " y: " + std::to_string(y) + "}";
		}
	};

	typedef vec2_t<f32> vec2;
	typedef vec2_t<f64> vec2d;
	typedef vec2_t<i32> vec2i;
	typedef vec2_t<i64> vec2l;
}}
#endif