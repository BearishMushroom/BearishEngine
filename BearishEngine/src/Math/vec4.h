#ifndef _BEARISH_MATH_VEC4_H_
#define _BEARISH_MATH_VEC4_H_

#include "..\Core\IAllocatable.h"
#include "..\Types.h"
#include "..\Utils.h"
#include "vec3.h"

namespace Bearish { namespace Math {
	template <class T>
	class vec2_t;

	template<class T>
	class vec4_t : public Core::IAllocatable<vec4_t<T>> {
	public:
		union {
			struct {
				T x, y, z, w;
			};

			struct {
				T r, g, b, a;
			};

			struct {
				T s, t, z, w;
			};

			struct {
				T v[4];
			};

			struct {
				vec2_t<T> xy, zw;
			};

			struct {
				T x;
				vec2_t<T> yz;
				T w;
			};

			struct {
				vec3_t<T> xyz;
				T w;
			};

			struct {
				T x;
				vec3_t<T> yzw;
			};
		};
		
		vec4_t(T x = 0, T y = 0, T z = 0, T w = 0) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		vec4_t(T x) {
			this->x = x;
			this->y = x;
			this->z = x;
			this->w = x;
		}

		vec4_t(u32 color, bool normalize = true) {
			T a = ((color & 0x000000FF)      ) / (normalize ? static_cast<T>(255.0) : 1);
			T b = ((color & 0x0000FF00) >>  8) / (normalize ? static_cast<T>(255.0) : 1);
			T g = ((color & 0x00FF0000) >> 16) / (normalize ? static_cast<T>(255.0) : 1);
			T r = ((color & 0xFF000000) >> 24) / (normalize ? static_cast<T>(255.0) : 1);
			x = r;
			y = g;
			z = b;
			w = a;
		}

		inline vec4_t<T> operator+(const vec4_t& other) const {
			return vec4_t<T>(x + other.x, y + other.y, z + other.z, w + other.w);
		}

		vec4_t<T> Add(vec4_t other) {
			return vec4_t<T>(x + other.x, y + other.y, z + other.z, w + other.w);
		}

		inline vec4_t<T> operator+(const T scalar) const {
			return vec4_t<T>(x + scalar, y + scalar, z + scalar, w + scalar);
		}

		inline vec4_t<T> operator-(const vec4_t& other) const {
			return vec4_t<T>(x - other.x, y - other.y, z - other.z, w - other.w);
		}

		vec4_t<T> Subtract(vec4_t other) {
			return vec4_t<T>(x - other.x, y - other.y, z - other.z, w - other.w);
		}

		inline vec4_t<T> operator-(const T scalar) const {
			return vec4_t<T>(x - scalar, y - scalar, z - scalar, w - scalar);
		}

		inline vec4_t<T> operator*(const vec4_t& other) const {
			return vec4_t<T>(x * other.x, y * other.y, z * other.z, w * other.w);
		}

		vec4_t<T> Multiply(vec4_t other) {
			return vec4_t<T>(x * other.x, y * other.y, z * other.z, w * other.w);
		}

		inline vec4_t<T> operator*(const T scalar) const {
			return vec4_t<T>(x * scalar, y * scalar, z * scalar, w * scalar);
		}

		inline vec4_t<T> operator/(const vec4_t& other) const {
			return vec4_t<T>(x / other.x, y / other.y, z / other.z, w / other.w);
		}

		vec4_t<T> Divide(vec4_t other) {
			return vec4_t<T>(x / other.x, y / other.y, z / other.z, w / other.w);
		}

		inline vec4_t<T> operator/(const T scalar) const {
			return vec4_t<T>(x / scalar, y / scalar, z / scalar, w / scalar);
		}

		inline void operator+=(const vec4_t& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
		}

		inline void operator+=(const T scalar) {
			x += scalar;
			y += scalar;
			z += scalar;
			w += scalar;
		}

		inline void operator-=(const vec4_t& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
		}

		inline void operator-=(const T scalar) {
			x -= scalar;
			y -= scalar;
			z -= scalar;
			w -= scalar;
		}

		inline void operator*=(const vec4_t& other) {
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;
		}

		inline void operator*=(const T scalar) {
			x *= scalar;
			y *= scalar;
			z *= scalar;
			w *= scalar;
		}

		inline void operator/=(const vec4_t& other) {
			x /= other.x;
			y /= other.y;
			z /= other.z;
			w /= other.w;
		}

		inline void operator/=(const T scalar) {
			x /= scalar;
			y /= scalar;
			z /= scalar;
			w /= scalar;
		}

		inline bool operator==(const vec4_t& other) const {
			return (x == other.x && y == other.y && z == other.z && w == other.w);
		}

		inline T& operator[](const size_t index) {
			return v[index];
		}

		inline T Max() const {
			return __max(x, __max(y, __max(z, w)));
		}

		inline T Min() const {
			return __min(x, __min(y, __min(z, w)));
		}

		inline T Length() const {
			return static_cast<T>(sqrt(x * x + y * y + z * z + w * w));
		}

		inline vec4_t<T> Normalize() {
			T length = Length();
			x /= length;
			y /= length;
			z /= length;
			w /= length;
			return (*this);
		}

		inline T Dot(const vec4_t& other) {
			return x * other.x + y * other.y + z * other.z + w * other.w;
		}

		inline vec4_t<T> Lerp(const vec4_t& destination, const T factor) const {
			return (destination - (*this)) * factor + (*this);
		}

		inline string ToString() {
			return "{x: " + std::to_string(x) + " y: " + std::to_string(y) + " z: " + std::to_string(z) + " w: " + std::to_string(w) + "}";
		}
	};

	typedef vec4_t<f32> vec4;
	typedef vec4_t<f64> vec4d;
	typedef vec4_t<i32> vec4i;
	typedef vec4_t<i64> vec4l;
}}

#endif 