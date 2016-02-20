#ifndef _BEARISH_MATH_QUAT_H_
#define _BEARISH_MATH_QUAT_H_

#include "..\Core\IAllocatable.h"
#include "..\Types.h"
#include "..\Utils.h"
#include "mat4.h"

namespace Bearish { namespace Math {
	template<class T>
	class vec3_t;

	template<class T>
	class quat_t : public Core::IAllocatable<quat_t<T>> {
	public:
		T x, y, z, w;
		
		quat_t(T x = 0, T y = 0, T z = 0, T w = 1) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		quat_t(mat4_t<T> m) {
			T trace = m[0][0] + m[1][1] + m[2][2];

			if (trace > 0) {
				T s = 0.5f / static_cast<T>(sqrt(trace + 1.0f));
				w = 0.25f / s;
				x = (m[1][2] - m[2][1]) * s;
				y = (m[2][0] - m[0][2]) * s;
				z = (m[0][1] - m[1][0]) * s;
			} else if (m[0][0] > m[1][1] && m[0][0] > m[2][2]) {
				T s = 2.0f * sqrtf(1.0f + m[0][0] - m[1][1] - m[2][2]);
				w = (m[1][2] - m[2][1]) / s;
				x = 0.25f * s;
				y = (m[1][0] + m[0][1]) / s;
				z = (m[2][0] + m[0][2]) / s;
			} else if (m[1][1] > m[2][2]) {
				T s = 2.0f * sqrtf(1.0f + m[1][1] - m[0][0] - m[2][2]);
				w = (m[2][0] - m[0][2]) / s;
				x = (m[1][0] + m[0][1]) / s;
				y = 0.25f * s;
				z = (m[2][1] + m[1][2]) / s;
			} else {
				T s = 2.0f * static_cast<T>(sqrt(1.0f + m[2][2] - m[1][1] - m[0][0]));
				w = (m[0][1] - m[1][0]) / s;
				x = (m[2][0] + m[0][2]) / s;
				y = (m[1][2] + m[2][1]) / s;
				z = 0.25f * s;
			}

			T length = Length();
			w = w / length;
			x = x / length;
			y = y / length;
			z = z / length;
		}

		inline quat_t<T> operator*(const quat_t& other) const {
			T newW = w * other.w - x * other.x - y * other.y - z * other.z;
			T newX = x * other.w + w * other.x + y * other.z - z * other.y;
			T newY = y * other.w + w * other.y + z * other.x - x * other.z;
			T newZ = z * other.w + w * other.z + x * other.y - y * other.x;

			return quat_t<T>(newX, newY, newZ, newW);
		}

		inline quat_t<T> operator*(const vec3_t<T>& other) const {
			T newW = -x * other.x - y * other.y - z * other.z;
			T newX = w * other.x + y * other.z - z * other.y;
			T newY = w * other.y + z * other.x - x * other.z;
			T newZ = w * other.z + x * other.y - y * other.x;

			return quat_t<T>(newX, newY, newZ, newW);
		}

		inline quat_t<T> operator*(T scalar) const {
			return quat_t<T>(x * scalar, y * scalar, z * scalar, w * scalar);
		}

		inline quat_t<T> operator+(const quat_t& other) const {
			return quat_t<T>(x + other.x, y + other.y, z + other.z, w + other.w);
		}

		inline quat_t<T> operator-(const quat_t& other) const {
			return quat_t<T>(x - other.x, y - other.y, z - other.z, w - other.w);
		}

		inline bool operator==(const quat_t& other) const {
			return (x == other.x && y == other.y && z == other.z && w == other.w);
		}

		inline void operator+=(const quat_t& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
		}

		inline void operator-=(const quat_t& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
		}

		inline void operator*=(const quat_t& other) {
			T newW = w * other.w - x * other.x - y * other.y - z * other.z;
			T newX = x * other.w + w * other.x + y * other.z - z * other.y;
			T newY = y * other.w + w * other.y + z * other.x - x * other.z;
			T newZ = z * other.w + w * other.z + x * other.y - y * other.x;

			x = newX; y = newY; z = newZ; w = newW;
		}

		inline void operator*=(const vec3_t<T>& other) {
			T newW = -x * other.x - y * other.y - z * other.z;
			T newX = w * other.x + y * other.z - z * other.y;
			T newY = w * other.y + z * other.x - x * other.z;
			T newZ = w * other.z + x * other.y - y * other.x;

			x = newX; y = newY; z = newZ; w = newW;
		}
		
		inline T Length() const {
			return static_cast<T>(sqrt(x * x + y * y + z * z + w * w));
		}

		inline T Dot(const quat_t& other) const {
			return x * other.x + y * other.y + z * other.z + w * other.w;
		}

		inline quat_t<T> Normalize() {
			T length = Length();
			x /= length;
			y /= length;
			z /= length;
			w /= length;
			return (*this);
		}

		inline quat_t<T> Conjugate() const {
			return quat_t<T>(-x, -y, -z, w);
		}

		inline quat_t<T> CreateRotation(const vec3_t<T>& axis, T angle) {
			T sinHalfAngle = static_cast<T>(sin(angle / 2));
			T cosHalfAngle = static_cast<T>(cos(angle / 2));

			x = axis.x * sinHalfAngle;
			y = axis.y * sinHalfAngle;
			z = axis.z * sinHalfAngle;
			w = cosHalfAngle;

			return *this;
		}

		inline vec3_t<T> Forward() const {
			return vec3_t<T>(2.0f * (x * z - w * y), 2.0f * (y * z + w * x), 1.0f - 2.0f * (x * x + y * y));
		}

		inline vec3_t<T> Back() const {
			return vec3_t<T>(-2.0f * (x * z - w * y), -2.0f * (y * z + w * x), -(1.0f - 2.0f * (x * x + y * y)));
		}

		inline vec3_t<T> Up() const {
			return vec3_t<T>(2.0f * (x*y + w*z), 1.0f - 2.0f * (x*x + z*z), 2.0f * (y*z - w*x));
		}

		inline vec3_t<T> Down() const {
			return vec3_t<T>(-2.0f * (x*y + w*z), -(1.0f - 2.0f * (x*x + z*z)), -2.0f * (y*z - w*x));
		}

		inline vec3_t<T> Right() const {
			return vec3_t<T>(1.0f - 2.0f * (y*y + z*z), 2.0f * (x*y - w*z), 2.0f * (x*z + w*y));
		}

		inline vec3_t<T> Left() const {
			return vec3_t<T>(-(1.0f - 2.0f * (y*y + z*z)), -2.0f * (x*y - w*z), -2.0f * (x*z + w*y));
		}

		inline quat_t<T> NLerp(const quat_t& dest, T lerpFactor, bool shortestPath) const {
			quat_t<T> correctedDest = dest;

			if (shortestPath && Dot(dest) < 0)
				correctedDest = quat_t<T>(-dest.x, -dest.y, -dest.z, -dest.w);

			return (correctedDest + ((*this) * lerpFactor) + (*this)).Normalize();
		}

		inline quat_t<T> SLerp(const quat_t& r, T lerpFactor, bool shortestPath) const {
			static const f64 EPSILON = 1e3;

			T cos = Dot(r);
			quat_t<T> correctedDest = r;

			if (shortestPath && cos < 0) {
				cos = -cos;
				correctedDest = quat_t<T>(-r.x, -r.y, -r.z, -r.w);
			}

			if (abs(cos) >= 1 - EPSILON)
				return NLerp(correctedDest, lerpFactor, false);

			T sin = static_cast<T>(sqrt(1.0f - cos * cos));
			T angle = static_cast<T>(atan2(sin, cos));
			T invSin = static_cast<T>(1.0 / sin);

			T srcFactor = static_cast<T>(sin((1.0f - lerpFactor) * angle) * invSin);
			T destFactor = static_cast<T>(sin((lerpFactor)* angle) * invSin);

			return (*this) * srcFactor + (correctedDest * destFactor);
		}

		static inline const quat_t<T> GetLookAtDirection(const vec3_t<T>& position, const vec3_t<T>& point, const vec3_t<T>& up) {
			return quat_t<T>(mat4_t<T>().CreateRotation((point - position).Normalize(), up));
		}

		inline string ToString() {
			return "{x: " + std::to_string(x) + " y: " + std::to_string(y) + " z: " + std::to_string(z) + " w: " + std::to_string(w) + "}";
		}
	};

	typedef quat_t<f32> quat;
	typedef quat_t<f64> quatd;
	typedef quat_t<i32> quati;
	typedef quat_t<i64> quatl;
}}
#endif