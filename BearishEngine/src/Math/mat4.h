#ifndef _BEARISH_MATH_MAT4_H_
#define _BEARISH_MATH_MAT4_H_

#include "..\Core\IAllocatable.h"
#include "..\Types.h"
#include "..\Utils.h"

namespace Bearish { namespace Math {
	template<class T>
	class vec3_t;

	template <class T>
	class vec2_t;

	template<class T>
	class mat4_t : public Core::IAllocatable<mat4_t<T>> {
	private:
		union {
			T _v[4][4];
		};
	public:
		inline T* operator[](size_t index) {
			return _v[index];
		}

		mat4_t() {}
		~mat4_t() {}

		inline mat4_t<T> CreateIdentity() {
			_v[0][0] = 1;	_v[0][1] = 0;	_v[0][2] = 0;	_v[0][3] = 0;
			_v[1][0] = 0;	_v[1][1] = 1;	_v[1][2] = 0;	_v[1][3] = 0;
			_v[2][0] = 0;	_v[2][1] = 0;	_v[2][2] = 1;	_v[2][3] = 0;
			_v[3][0] = 0;	_v[3][1] = 0;	_v[3][2] = 0;	_v[3][3] = 1;

			return *this;
		}

		inline mat4_t<T> CreateTranslation(T x, T y, T z) {
			_v[0][0] = 1;	_v[0][1] = 0;	_v[0][2] = 0;	_v[0][3] = x;
			_v[1][0] = 0;	_v[1][1] = 1;	_v[1][2] = 0;	_v[1][3] = y;
			_v[2][0] = 0;	_v[2][1] = 0;	_v[2][2] = 1;	_v[2][3] = z;
			_v[3][0] = 0;	_v[3][1] = 0;	_v[3][2] = 0;	_v[3][3] = 1;

			return *this;
		}

		inline mat4_t<T> CreateScale(T x, T y, T z) {
			_v[0][0] = x;	_v[0][1] = 0;	_v[0][2] = 0;	_v[0][3] = 0;
			_v[1][0] = 0;	_v[1][1] = y;	_v[1][2] = 0;	_v[1][3] = 0;
			_v[2][0] = 0;	_v[2][1] = 0;	_v[2][2] = z;	_v[2][3] = 0;
			_v[3][0] = 0;	_v[3][1] = 0;	_v[3][2] = 0;	_v[3][3] = 1;

			return *this;
		}

		inline mat4_t<T> CreateRotation(T x, T y, T z) {
			mat4_t<T> rx, ry, rz;

			x = x * (f32)RADIANS;
			y = y * (f32)RADIANS;
			z = z * (f32)RADIANS;

			rz[0][0] = static_cast<T>(cos(z));	      rz[0][1] = -static_cast<T>(sin(z));	  rz[0][2] = 0;	rz[0][3] = 0;
			rz[1][0] = static_cast<T>(sin(z));	      rz[1][1] = static_cast<T>(cos(z));	  rz[1][2] = 0;	rz[1][3] = 0;
			rz[2][0] = 0;						      rz[2][1] = 0;						      rz[2][2] = 1;	rz[2][3] = 0;
			rz[3][0] = 0;						      rz[3][1] = 0;						      rz[3][2] = 0;	rz[3][3] = 1;

			rx[0][0] = 1;	rx[0][1] = 0;					 	      rx[0][2] = 0;						      rx[0][3] = 0;
			rx[1][0] = 0;	rx[1][1] = static_cast<T>(cos(x));	      rx[1][2] = -static_cast<T>(sin(x));	  rx[1][3] = 0;
			rx[2][0] = 0;	rx[2][1] = static_cast<T>(sin(x));	      rx[2][2] = static_cast<T>(cos(x));	  rx[2][3] = 0;
			rx[3][0] = 0;	rx[3][1] = 0;						      rx[3][2] = 0;						      rx[3][3] = 1;
				
			ry[0][0] = static_cast<T>(cos(y));	      ry[0][1] = 0;	ry[0][2] = -static_cast<T>(sin(y));	ry[0][3] = 0;
			ry[1][0] = 0;						      ry[1][1] = 1;	ry[1][2] = 0;						      ry[1][3] = 0;
			ry[2][0] = static_cast<T>(sin(y));	      ry[2][1] = 0;	ry[2][2] = static_cast<T>(cos(y));	ry[2][3] = 0;
			ry[3][0] = 0;						      ry[3][1] = 0;	ry[3][2] = 0;						      ry[3][3] = 1;

			*this = rz * (ry * rx);
			return *this;
		}

		inline mat4_t<T> CreateProjection(T fov, T aspect, T zNear, T zFar) {
			T tanHalfFov = static_cast<T>(tan(fov / 2));
			T zRange = zNear - zFar;

			_v[0][0] = 1.0f / (tanHalfFov * aspect);	_v[0][1] = 0;					   _v[0][2] = 0;							      _v[0][3] = 0;
			_v[1][0] = 0;								      _v[1][1] = 1.0f / tanHalfFov;	_v[1][2] = 0;							      _v[1][3] = 0;
			_v[2][0] = 0;								      _v[2][1] = 0;					   _v[2][2] = (-zNear - zFar) / zRange;	_v[2][3] = 2 * zFar * zNear / zRange;
			_v[3][0] = 0;								      _v[3][1] = 0;					   _v[3][2] = 1;							      _v[3][3] = 0;

			return *this;
		}

		inline mat4_t<T> CreateRotation(const vec3_t<T>& forward, const vec3_t<T>& up) {
			vec3_t<T> f = forward;
			f.Normalize();

			vec3_t<T> r = up;
			r.Normalize();
			r = r.Cross(f);

			vec3_t<T> u = f.Cross(r);

			_v[0][0] = r.x;	_v[0][1] = r.y;	_v[0][2] = r.z;	_v[0][3] = 0;
			_v[1][0] = u.x;	_v[1][1] = u.y;	_v[1][2] = u.z;	_v[1][3] = 0;
			_v[2][0] = f.x;	_v[2][1] = f.y;	_v[2][2] = f.z;	_v[2][3] = 0;
			_v[3][0] = 0;	   _v[3][1] = 0;	   _v[3][2] = 0;	   _v[3][3] = 1;

			return *this;
		}

		inline mat4_t<T> CreateRotation(const vec3_t<T>& forward, const vec3_t<T>& up, const vec3_t<T>& right) {
			vec3_t<T> f = forward;
			vec3_t<T> r = right;
			vec3_t<T> u = up;

			_v[0][0] = r.x;	_v[0][1] = r.y;	_v[0][2] = r.z;	_v[0][3] = 0;
			_v[1][0] = u.x;	_v[1][1] = u.y;	_v[1][2] = u.z;	_v[1][3] = 0;
			_v[2][0] = f.x;	_v[2][1] = f.y;	_v[2][2] = f.z;	_v[2][3] = 0;
			_v[3][0] = 0;	   _v[3][1] = 0;     _v[3][2] = 0;	   _v[3][3] = 1;

			return *this;
		}

		inline mat4_t<T> CreateOrthographic(T left, T right, T bottom, T top, T nearPlane, T farPlane) {
			T width = right - left;
			T height = top - bottom;
			T depth = farPlane - nearPlane;

			_v[0][0] = 2.f / width; _v[0][1] = 0;			   _v[0][2] = 0;			  _v[0][3] = -(right + left) / width;
			_v[1][0] = 0;			 _v[1][1] = 2.f / height;	_v[1][2] = 0;			  _v[1][3] = -(top + bottom) / height;
			_v[2][0] = 0;			 _v[2][1] = 0;			   _v[2][2] = -2.f / depth;   _v[2][3] = -(farPlane + nearPlane) / depth;
			_v[3][0] = 0;			 _v[3][1] = 0;			   _v[3][2] = 0;			  _v[3][3] = 1;

			return *this;
		}

		inline mat4_t<T> operator*(const mat4_t& o) const {
			mat4_t<T> result;

			for (i32 i = 0; i < 4; i++) {
				for (i32 j = 0; j < 4; j++) {
					result[i][j] = (_v[i][0] * o._v[0][j] +
						_v[i][1] * o._v[1][j] +
						_v[i][2] * o._v[2][j] +
						_v[i][3] * o._v[3][j]);
				}
			}

			return result;
		}

		inline void operator*=(const mat4_t& o) {
			mat4_t<T> result;

			for (i32 i = 0; i < 4; i++) {
				for (i32 j = 0; j < 4; j++) {
					result[i][j] = (_v[i][0] * o._v[0][j] +
						_v[i][1] * o._v[1][j] +
						_v[i][2] * o._v[2][j] +
						_v[i][3] * o._v[3][j]);
				}
			}

			(*this) = result;
		}

		inline vec3_t<T> Transform(const vec3_t<T>& o) const {
			return vec3_t<T>(
				_v[0][0] * o.x + _v[0][1] * o.y + _v[0][2] * o.z + _v[0][3],
				_v[1][0] * o.x + _v[1][1] * o.y + _v[1][2] * o.z + _v[1][3],
				_v[2][0] * o.x + _v[2][1] * o.y + _v[2][2] * o.z + _v[2][3]
				);
		}

		inline mat4_t<T> Transpose() const {
			mat4_t<T> result;
			for (i32 i = 0; i < 4; i++) {
				for (i32 j = 0; j < 4; j++) {
					result._v[j][i] = _v[i][j];
				}
			}

			return result;
		}

		inline T Get(i32 x, i32 y) const {
			return _v[x][y];
		}

		inline void Set(i32 x, i32 y, T value) {
			_v[x][y] = value;
		}

		inline string ToString() const {
			return "{ " + std::to_string(_v[0][0]) + " " + std::to_string(_v[0][1]) + " " + std::to_string(_v[0][2]) + " " + std::to_string(_v[0][3]) + "\n" +
				"  " + std::to_string(_v[1][0]) + " " + std::to_string(_v[1][1]) + " " + std::to_string(_v[1][2]) + " " + std::to_string(_v[1][3]) + "\n" +
				"  " + std::to_string(_v[2][0]) + " " + std::to_string(_v[2][1]) + " " + std::to_string(_v[2][2]) + " " + std::to_string(_v[2][3]) + "\n" +
				"  " + std::to_string(_v[3][0]) + " " + std::to_string(_v[3][1]) + " " + std::to_string(_v[3][2]) + " " + std::to_string(_v[3][3]) + " }";
		}
	};

	typedef mat4_t<f32> mat4;
	typedef mat4_t<f64> mat4d;
	typedef mat4_t<i32> mat4i;
	typedef mat4_t<i64> mat4l;
}}

#endif