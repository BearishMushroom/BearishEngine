#ifndef _BEARISH_TYPES_H_
#define _BEARISH_TYPES_H_

#include <pch.h>

namespace Bearish {
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

	typedef bool               b8;

#ifdef BEARISH_X86
	typedef i32                isize;
	typedef u32                usize;
#elif  BEARISH_X64
	typedef i64                isize;
	typedef u64                usize;
#else
	#pragma(error, "Found no platform macro.")
#endif

#if !defined(BEARISH_SIMD) && (defined(BEARISH_X86) || defined(BEARISH_X64))
	// Think about how to solve this properly as the Lua values aren't always 16-byte aligned.

//	#define BEARISH_SIMD
//	#include <xmmintrin.h>
//	static const __m128 SIMD_SIGN32 = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));
#endif

}

#ifdef BEARISH_MSVC
	#ifdef BEARISH_DLL
		#define BEARISH_API __declspec(dllexport)
	#else
		#define BEARISH_API __declspec(dllimport)
	#endif
#else
	#define BEARISH_API
#endif

#endif