#ifndef _BEARISH_MATH_MATH_H_
#define _BEARISH_MATH_MATH_H_

#include "../Types.h"

namespace Bearish { namespace Math {
	const static f64 RADIANS = 3.14159265 / 180.0;
	const static f64 DEGREES = 180.0 / 3.14159265;

	template<typename T>
	static T AsDegrees(const T radians) {
		return radians * static_cast<T>(DEGREES);
	}

	template<typename T>
	static T AsRadians(const T degrees) {
		return degrees * static_cast<T>(RADIANS);
	}
} }

#endif // _BEARISH_MATH_MATH_H_