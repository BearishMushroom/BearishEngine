#ifndef _BEARISH_MAf32H_VEC4_H_
#define _BEARISH_MAf32H_VEC4_H_

#include "..\Core\IAllocatable.h"
#include "..\Core\IAligned.h"
#include "..\Types.h"
#include "..\Utils.h"
#include "vec3.h"

namespace Bearish { namespace Math {
	template <class f32>
	class vec2_t;

	template<class f32>
	class vec4_t : public Core::IAllocatable<vec4_t<f32>> {
	public:
		union {
			struct {
				f32 x, y, z, w;
			};

			struct {
				f32 r, g, b, a;
			};

			struct {
				f32 s, t, z, w;
			};

			struct {
				f32 v[4];
			};

			struct {
				vec2_t<f32> xy, zw;
			};

			struct {
				f32 x;
				vec2_t<f32> yz;
				f32 w;
			};

			struct {
				vec3_t<f32> xyz;
				f32 w;
			};

			struct {
				f32 x;
				vec3_t<f32> yzw;
			};
		};

		vec4_t(f32 x = 0, f32 y = 0, f32 z = 0, f32 w = 0) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		vec4_t(f32 x) {
			this->x = x;
			this->y = x;
			this->z = x;
			this->w = x;
		}

		vec4_t(u32 color, bool normalize = true) {
			f32 a = ((color & 0x000000FF)) / (normalize ? static_cast<f32>(255.0) : 1);
			f32 b = ((color & 0x0000FF00) >> 8) / (normalize ? static_cast<f32>(255.0) : 1);
			f32 g = ((color & 0x00FF0000) >> 16) / (normalize ? static_cast<f32>(255.0) : 1);
			f32 r = ((color & 0xFF000000) >> 24) / (normalize ? static_cast<f32>(255.0) : 1);
			x = r;
			y = g;
			z = b;
			w = a;
		}

		inline vec4_t<f32> operator+(const vec4_t& other) const {
			return vec4_t<f32>(x + other.x, y + other.y, z + other.z, w + other.w);
		}

		vec4_t<f32> Add(vec4_t other) {
			return vec4_t<f32>(x + other.x, y + other.y, z + other.z, w + other.w);
		}

		inline vec4_t<f32> operator+(const f32 scalar) const {
			return vec4_t<f32>(x + scalar, y + scalar, z + scalar, w + scalar);
		}

		inline vec4_t<f32> operator-(const vec4_t& other) const {
			return vec4_t<f32>(x - other.x, y - other.y, z - other.z, w - other.w);
		}

		vec4_t<f32> Subtract(vec4_t other) {
			return vec4_t<f32>(x - other.x, y - other.y, z - other.z, w - other.w);
		}

		inline vec4_t<f32> operator-(const f32 scalar) const {
			return vec4_t<f32>(x - scalar, y - scalar, z - scalar, w - scalar);
		}

		inline vec4_t<f32> operator*(const vec4_t& other) const {
			return vec4_t<f32>(x * other.x, y * other.y, z * other.z, w * other.w);
		}

		vec4_t<f32> Multiply(vec4_t other) {
			return vec4_t<f32>(x * other.x, y * other.y, z * other.z, w * other.w);
		}

		inline vec4_t<f32> operator*(const f32 scalar) const {
			return vec4_t<f32>(x * scalar, y * scalar, z * scalar, w * scalar);
		}

		inline vec4_t<f32> operator/(const vec4_t& other) const {
			return vec4_t<f32>(x / other.x, y / other.y, z / other.z, w / other.w);
		}

		vec4_t<f32> Divide(vec4_t other) {
			return vec4_t<f32>(x / other.x, y / other.y, z / other.z, w / other.w);
		}

		inline vec4_t<f32> operator/(const f32 scalar) const {
			return vec4_t<f32>(x / scalar, y / scalar, z / scalar, w / scalar);
		}

		inline void operator+=(const vec4_t& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
		}

		inline void operator+=(const f32 scalar) {
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

		inline void operator-=(const f32 scalar) {
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

		inline void operator*=(const f32 scalar) {
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

		inline void operator/=(const f32 scalar) {
			x /= scalar;
			y /= scalar;
			z /= scalar;
			w /= scalar;
		}

		inline bool operator==(const vec4_t& other) const {
			return (x == other.x && y == other.y && z == other.z && w == other.w);
		}

		inline f32& operator[](const usize index) {
			return v[index];
		}

		inline f32 Max() const {
			return __max(x, __max(y, __max(z, w)));
		}

		inline f32 Min() const {
			return __min(x, __min(y, __min(z, w)));
		}

		inline f32 Length() const {
			return static_cast<f32>(sqrt(x * x + y * y + z * z + w * w));
		}

		inline vec4_t<f32> Normalize() {
			f32 length = Length();
			x /= length;
			y /= length;
			z /= length;
			w /= length;
			return (*this);
		}

		inline f32 Dot(const vec4_t& other) {
			return x * other.x + y * other.y + z * other.z + w * other.w;
		}

		inline vec4_t<f32> Lerp(const vec4_t& destination, const f32 factor) const {
			return (destination - (*this)) * factor + (*this);
		}

		inline string f32oString() {
			return "{x: " + std::to_string(x) + " y: " + std::to_string(y) + " z: " + std::to_string(z) + " w: " + std::to_string(w) + "}";
		}
	};

#ifdef BEARISH_SIMD
	template<>
	class vec4_t<f32> {
	public:
		union {
			struct {
				f32 x, y, z, w;
			};

			struct {
				f32 r, g, b, a;
			};

			struct {
				f32 s, t, z, w;
			};

			struct {
				f32 v[4];
			};

			struct {
				vec2_t<f32> xy, zw;
			};

			struct {
				f32 x;
				vec2_t<f32> yz;
				f32 w;
			};

			struct {
				vec3_t<f32> xyz;
				f32 w;
			};

			struct {
				f32 x;
				vec3_t<f32> yzw;
			};
		};

		vec4_t(f32 x = 0, f32 y = 0, f32 z = 0, f32 w = 0) {
			this->x = x; this->y = y;
			this->z = z; this->w = w;
		}

		vec4_t(f32 x) {
			this->x = x; this->y = x;
			this->z = x; this->w = x;
		}

		vec4_t(__m128 xmm) { _mm_store_ps(v, xmm); }

		vec4_t(u32 color, bool normalize = true) {
			f32 a = ((color & 0x000000FF)) / (normalize ? static_cast<f32>(255.0) : 1);
			f32 b = ((color & 0x0000FF00) >> 8) / (normalize ? static_cast<f32>(255.0) : 1);
			f32 g = ((color & 0x00FF0000) >> 16) / (normalize ? static_cast<f32>(255.0) : 1);
			f32 r = ((color & 0xFF000000) >> 24) / (normalize ? static_cast<f32>(255.0) : 1);
			x = r; y = g; z = b; w = a;
		}

		inline vec4_t<f32> operator+(const vec4_t<f32>& other) const { __m128 xmm = _mm_loadu_ps(v); __m128 xmm2 = _mm_loadu_ps(other.v); return _mm_add_ps(xmm, xmm2); }
		inline vec4_t<f32> Add(const vec4_t<f32>& other)       const { __m128 xmm = _mm_loadu_ps(v); __m128 xmm2 = _mm_loadu_ps(other.v); return _mm_add_ps(xmm, xmm2); }
		inline vec4_t<f32> operator-(const vec4_t<f32>& other) const { __m128 xmm = _mm_loadu_ps(v); __m128 xmm2 = _mm_loadu_ps(other.v); return _mm_sub_ps(xmm, xmm2); }
		inline vec4_t<f32> Subtract(const vec4_t<f32>& other)  const { __m128 xmm = _mm_loadu_ps(v); __m128 xmm2 = _mm_loadu_ps(other.v); return _mm_sub_ps(xmm, xmm2); }
		inline vec4_t<f32> operator*(const vec4_t<f32>& other) const { __m128 xmm = _mm_loadu_ps(v); __m128 xmm2 = _mm_loadu_ps(other.v); return _mm_mul_ps(xmm, xmm2); }
		inline vec4_t<f32> Multiply(const vec4_t<f32>& other)  const { __m128 xmm = _mm_loadu_ps(v); __m128 xmm2 = _mm_loadu_ps(other.v); return _mm_mul_ps(xmm, xmm2); }
		inline vec4_t<f32> operator/(const vec4_t<f32>& other) const { __m128 xmm = _mm_loadu_ps(v); __m128 xmm2 = _mm_loadu_ps(other.v); return _mm_div_ps(xmm, xmm2); }
		inline vec4_t<f32> Divide(const vec4_t<f32>& other)    const { __m128 xmm = _mm_loadu_ps(v); __m128 xmm2 = _mm_loadu_ps(other.v); return _mm_div_ps(xmm, xmm2); }
		
		inline vec4_t<f32> operator+(const f32 scalar) const { __m128 xmm = _mm_loadu_ps(v); return _mm_add_ps(xmm, _mm_set1_ps(scalar)); }
		inline vec4_t<f32> Add(const f32 scalar)       const { __m128 xmm = _mm_loadu_ps(v); return _mm_add_ps(xmm, _mm_set1_ps(scalar)); }
		inline vec4_t<f32> operator-(const f32 scalar) const { __m128 xmm = _mm_loadu_ps(v); return _mm_sub_ps(xmm, _mm_set1_ps(scalar)); }
		inline vec4_t<f32> Subtract(const f32 scalar)  const { __m128 xmm = _mm_loadu_ps(v); return _mm_sub_ps(xmm, _mm_set1_ps(scalar)); }
		inline vec4_t<f32> operator*(const f32 scalar) const { __m128 xmm = _mm_loadu_ps(v); return _mm_mul_ps(xmm, _mm_set1_ps(scalar)); }
		inline vec4_t<f32> Multiply(const f32 scalar)  const { __m128 xmm = _mm_loadu_ps(v); return _mm_mul_ps(xmm, _mm_set1_ps(scalar)); }
		inline vec4_t<f32> operator/(const f32 scalar) const { __m128 xmm = _mm_loadu_ps(v); return _mm_div_ps(xmm, _mm_set1_ps(scalar)); }
		inline vec4_t<f32> Divide(const f32 scalar)    const { __m128 xmm = _mm_loadu_ps(v); return _mm_div_ps(xmm, _mm_set1_ps(scalar)); }

		inline vec4_t<f32>& operator+=(const vec4_t<f32>& other) { __m128 xmm = _mm_loadu_ps(v); __m128 xmm2 = _mm_loadu_ps(other.v); *this = _mm_add_ps(xmm, xmm2); return *this; };
		inline vec4_t<f32>& operator-=(const vec4_t<f32>& other) { __m128 xmm = _mm_loadu_ps(v); __m128 xmm2 = _mm_loadu_ps(other.v); *this = _mm_sub_ps(xmm, xmm2); return *this; };
		inline vec4_t<f32>& operator*=(const vec4_t<f32>& other) { __m128 xmm = _mm_loadu_ps(v); __m128 xmm2 = _mm_loadu_ps(other.v); *this = _mm_mul_ps(xmm, xmm2); return *this; };
		inline vec4_t<f32>& operator/=(const vec4_t<f32>& other) { __m128 xmm = _mm_loadu_ps(v); __m128 xmm2 = _mm_loadu_ps(other.v); *this = _mm_div_ps(xmm, xmm2); return *this; };

		inline vec4_t<f32>& operator+=(const f32 scalar) { __m128 xmm = _mm_loadu_ps(v); *this = _mm_add_ps(xmm, _mm_set1_ps(scalar)); return *this; };
		inline vec4_t<f32>& operator-=(const f32 scalar) { __m128 xmm = _mm_loadu_ps(v); *this = _mm_sub_ps(xmm, _mm_set1_ps(scalar)); return *this; };
		inline vec4_t<f32>& operator*=(const f32 scalar) { __m128 xmm = _mm_loadu_ps(v); *this = _mm_mul_ps(xmm, _mm_set1_ps(scalar)); return *this; };
		inline vec4_t<f32>& operator/=(const f32 scalar) { __m128 xmm = _mm_loadu_ps(v); *this = _mm_div_ps(xmm, _mm_set1_ps(scalar)); return *this; };

		inline bool operator==(const vec4_t<f32>& other) const { 
			__m128 xmm = _mm_loadu_ps(v); __m128 xmm2 = _mm_loadu_ps(other.v); 
			return (_mm_movemask_ps(_mm_cmpeq_ps(xmm, xmm2)) & 0x7) == 0x7; 
		}

		inline bool operator!=(const vec4_t<f32>& other) const { return !(*this == other); }

		inline vec4_t<f32> operator-() const { __m128 xmm = _mm_loadu_ps(v); return _mm_xor_ps(xmm, SIMD_SIGN32); }

		inline f32& operator[](const usize index) {
			return v[index];
		}

		inline f32 Max() const {
			return __max(x, __max(y, __max(z, w)));
		}

		inline f32 Min() const {
			return __min(x, __min(y, __min(z, w)));
		}

		inline f32 Length() const {
			return sqrt(Dot(*this));
		}

		inline vec4_t<f32> Normalize() {
			return operator/=(Length());
		}

		inline f32 Dot(const vec4_t& other) const {
			__m128 v0 = _mm_loadu_ps(v);
			__m128 v1 = _mm_loadu_ps(other.v);
			v0 = _mm_mul_ps(v0, v1);

			v1 = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(2, 3, 0, 1));
			v0 = _mm_add_ps(v0, v1);
			v1 = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0, 1, 2, 3));
			v0 = _mm_add_ps(v0, v1);

			return _mm_cvtss_f32(v0);
		}

		inline vec4_t<f32> Lerp(const vec4_t& destination, const f32 factor) const {
			return (destination - (*this)) * factor + (*this);
		}

		inline string ToString() {
			return "{x: " + std::to_string(x) + " y: " + std::to_string(y) + " z: " + std::to_string(z) + " w: " + std::to_string(w) + "}";
		}
	};
#endif

	typedef vec4_t<f32> vec4;
	typedef vec4_t<f64> vec4d;
	typedef vec4_t<i32> vec4i;
	typedef vec4_t<i64> vec4l;
}}

#endif 