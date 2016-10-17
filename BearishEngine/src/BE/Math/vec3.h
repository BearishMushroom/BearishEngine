#ifndef _BEARISH_MATH_VEC3_H_
#define _BEARISH_MATH_VEC3_H_

#include "..\Types.h"
#include "vec2.h"
#include <cereal\archives\binary.hpp>

namespace Bearish { namespace Math {
	template<class T>
	class quat_t;

	template<class T>
	class vec3_t {
	public:
		union {
			struct {
				T x, y, z;
			};

			struct {
				T r, g, b;
			};

			struct {
				T s, t, z;
			};

			struct {
				T v[3];
			};

			struct {
				vec2_t<T> xy;
				T z;
			};

			struct {
				T x;
				vec2_t<T> yz;
			};
		};

		vec3_t(T x, T y, T z = 0) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		vec3_t(T x = 0) {
			this->x = x;
			this->y = x;
			this->z = x;
		}

		vec3_t(vec2_t<T> xy, T z = 0) {
			this->xy = xy;
			this->z = z;
		}

		vec3_t(const vec3_t& other) {
			this->x = (T)other.x;
			this->y = (T)other.y;
			this->z = (T)other.z;
		}

		vec3_t(u32 color, bool normalize) {
			T b = ((color & 0x0000FF00) >>  8) / (normalize ? static_cast<T>(255.0) : 1);
			T g = ((color & 0x00FF0000) >> 16) / (normalize ? static_cast<T>(255.0) : 1);
			T r = ((color & 0xFF000000) >> 24) / (normalize ? static_cast<T>(255.0) : 1);
			x = r;
			y = g;
			z = b;
		}

		inline vec3_t<T> operator+(const vec3_t& other) const {
			return vec3_t<T>(x + other.x, y + other.y, z + other.z);
		}

		vec3_t<T> Add(vec3_t other) {
			return vec3_t<T>(x + other.x, y + other.y, z + other.z);
		}

		inline vec3_t<T> operator+(const T scalar) const {
			return vec3_t<T>(x + scalar, y + scalar, z + scalar);
		}

		inline vec3_t<T> operator-(const vec3_t& other) const {
			return vec3_t<T>(x - other.x, y - other.y, z - other.z);
		}

		vec3_t<T> Subtract(vec3_t other) {
			return vec3_t<T>(x - other.x, y - other.y, z - other.z);
		}

		inline vec3_t<T> operator-(const T scalar) const {
			return vec3_t<T>(x - scalar, y - scalar, z - scalar);
		}

		inline vec3_t<T> operator*(const vec3_t& other) const {
			return vec3_t<T>(x * other.x, y * other.y, z * other.z);
		}

		vec3_t<T> Multiply(vec3_t other) {
			return vec3_t<T>(x * other.x, y * other.y, z * other.z);
		}

		inline vec3_t<T> operator*(const T scalar) const {
			return vec3_t<T>(x * scalar, y * scalar, z * scalar);
		}

		inline vec3_t<T> operator/(const vec3_t& other) const {
			return vec3_t<T>(x / other.x, y / other.y, z / other.z);
		}

		vec3_t<T> Divide(vec3_t other) {
			return vec3_t<T>(x / other.x, y / other.y, z / other.z);
		}

		inline vec3_t<T> operator/(const T scalar) const {
			return vec3_t<T>(x / scalar, y / scalar, z / scalar);
		}

		inline void operator+=(const vec3_t& other) {
			x += other.x;
			y += other.y;
			z += other.z;
		}

		inline void operator+=(const T scalar) {
			x += scalar;
			y += scalar;
			z += scalar;
		}

		inline void operator-=(const vec3_t& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
		}

		inline void operator-=(const T scalar) {
			x -= scalar;
			y -= scalar;
			z -= scalar;
		}

		inline void operator*=(const vec3_t& other) {
			x *= other.x;
			y *= other.y;
			z *= other.z;
		}

		inline void operator*=(const T scalar) {
			x *= scalar;
			y *= scalar;
			z *= scalar;
		}

		inline void operator/=(const vec3_t& other) {
			x /= other.x;
			y /= other.y;
			z /= other.z;
		}

		inline void operator/=(const T scalar) {
			x /= scalar;
			y /= scalar;
			z /= scalar;
		}

		inline bool operator==(const vec3_t& other) const {
			return (x == other.x && y == other.y && z == other.z);
		}

		inline T operator[](const usize index) const {
			return v[index];
		}

		inline T Max() const {
			return __max(x, __max(y, z));
		}

		inline T Min() const {
			return __min(x, __min(y, z));
		}

		inline T Length() const {
			return static_cast<T>(sqrt(x * x + y * y + z * z));
		}

		inline T LengthSq() const {
			return static_cast<T>(x * x + y * y + z * z);
		}

		inline vec3_t<T> Normalize() {
			T length = Length();
			x /= length;
			y /= length;
			z /= length;
			return (*this);
		}

		inline T Dot(const vec3_t& other) const {
			return x * other.x + y * other.y + z * other.z;
		}

		inline vec3_t<T> Rotate(T angle, const vec3_t& axis) const {
			quat_t<T> rotation = quat_t<T>().CreateRotation(axis, angle);
			quat_t<T> conjugate = rotation.Conjugate();
			quat_t<T> w = rotation * (*this) * conjugate;
			vec3_t<T> ret(w.x, w.y, w.z);

			return ret;
		}

		inline vec3_t<T> Rotate(const quat_t<T>& rotation) const {
			quat_t<T> conjugate = rotation.Conjugate();
			quat_t<T> w = rotation * (*this) * conjugate;
			vec3_t<T> ret(w.x, w.y, w.z);

			return ret;
		}

		inline vec3_t<T> Cross(const vec3_t& other) const {
			T x = y * other.z - z * other.y;
			T y = z * other.z - x * other.z;
			T z = x * other.y - y * other.x;

			return vec3_t<T>(x, y, z);
		}

		inline vec3_t<T> Lerp(const vec3_t& destination, const T factor) const {
			return (destination - (*this)) * factor + (*this);
		}

		inline vec3_t<T> Grow(const vec3_t& other) {
			x = x > other.x ? x : other.x;
			y = y > other.y ? y : other.y;
			z = z > other.z ? z : other.z;
			return *this;
		}

		inline string ToString() const {
			return "{x: " + std::to_string(x) + " y: " + std::to_string(y) + " z: " + std::to_string(z) + "}";
		}

		template<class Archive>
		void serialize(Archive& ar) {
			ar(x, y, z);
		}
	};

	typedef vec3_t<f32> vec3;
	typedef vec3_t<f64> vec3d;
	typedef vec3_t<i32> vec3i;
	typedef vec3_t<i64> vec3l;
}}

#endif