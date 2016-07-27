#ifndef _BEARISH_TYPES_H_
#define _BEARISH_TYPES_H_

#include "pch.h"

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
	typedef i32                size;
	typedef u32                usize;
#elif BEARISH_X64
	typedef i64                size;
	typedef u64                usize;
#else
	#pragma(error, "Found no platform macro.")
#endif

	static const void* null = (void*)0;
}

#endif