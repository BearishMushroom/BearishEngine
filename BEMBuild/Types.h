#pragma once

#include <string>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp/scene.h>

typedef signed char        i8;
typedef signed short       i16;
typedef signed int         i32;
typedef signed long long   i64;

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

typedef float              f32;
typedef double             f64;

typedef char               c8;
typedef std::string        string;
typedef const char*        cstring;

typedef bool					b8;

static const void* null = (void*)0;


template<class T>
class vec2_t {
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

	inline T operator[](const size_t index) const {
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

	vec3_t(u32 color, bool normalize) {
		T b = ((color & 0x0000FF00) >> 8) / (normalize ? static_cast<T>(255.0) : 1);
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

	inline T operator[](const size_t index) const {
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

	inline T Dot(const vec3_t& other) {
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

	inline string ToString() const {
		return "{x: " + std::to_string(x) + " y: " + std::to_string(y) + " z: " + std::to_string(z) + "}";
	}
};

typedef vec3_t<f32> vec3;
typedef vec3_t<f64> vec3d;
typedef vec3_t<i32> vec3i;
typedef vec3_t<i64> vec3l;


template <class T>
class vec2_t;

template<class T>
class vec4_t {
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
		T a = ((color & 0x000000FF)) / (normalize ? static_cast<T>(255.0) : 1);
		T b = ((color & 0x0000FF00) >> 8) / (normalize ? static_cast<T>(255.0) : 1);
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

template<class T>
class vec3_t;

template <class T>
class vec2_t;

template<class T>
class mat4_t {
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

	mat4_t(aiMatrix4x4 mat) {
		for (i32 i = 0; i < 4; i++) {
			for (i32 j = 0; j < 4; j++) {
				_v[i][j] = mat[i][j];
			}
		}
	}

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

		x = x * RADIANS;
		y = y * RADIANS;
		z = z * RADIANS;

		rz[0][0] = static_cast<T>(cos(z));	rz[0][1] = -static_cast<T>(sin(z));	rz[0][2] = 0;	rz[0][3] = 0;
		rz[1][0] = static_cast<T>(sin(z));	rz[1][1] = static_cast<T>(cos(z));	rz[1][2] = 0;	rz[1][3] = 0;
		rz[2][0] = 0;						      rz[2][1] = 0;						      rz[2][2] = 1;	rz[2][3] = 0;
		rz[3][0] = 0;						      rz[3][1] = 0;						      rz[3][2] = 0;	rz[3][3] = 1;

		rx[0][0] = 1;	rx[0][1] = 0;					 	      rx[0][2] = 0;						      rx[0][3] = 0;
		rx[1][0] = 0;	rx[1][1] = static_cast<T>(cos(x));	rx[1][2] = -static_cast<T>(sin(x));	rx[1][3] = 0;
		rx[2][0] = 0;	rx[2][1] = static_cast<T>(sin(x));	rx[2][2] = static_cast<T>(cos(x));	rx[2][3] = 0;
		rx[3][0] = 0;	rx[3][1] = 0;						      rx[3][2] = 0;						      rx[3][3] = 1;

		ry[0][0] = static_cast<T>(cos(y));	ry[0][1] = 0;	ry[0][2] = -static_cast<T>(sin(y));	ry[0][3] = 0;
		ry[1][0] = 0;						      ry[1][1] = 1;	ry[1][2] = 0;						      ry[1][3] = 0;
		ry[2][0] = static_cast<T>(sin(y));	ry[2][1] = 0;	ry[2][2] = static_cast<T>(cos(y));	ry[2][3] = 0;
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
