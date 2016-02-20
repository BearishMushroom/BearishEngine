#ifndef _BEARISH_SCRIPTING_ENGINE_H_
#define _BEARISH_SCRIPTING_ENGINE_H_
/*
#include <squirrel.h>
#include <sqstdio.h>
#include <sqstdaux.h>
#include <sqstdblob.h>
#include <sqstdmath.h>
#include <sqstdstring.h>
#include <sqstdsystem.h>
#include <sqplus.h>

#include <sqrat.h>

#include <stdio.h>
#include <stdarg.h>

#include <functional>

#include "../Types.h"
#include "../Core/Logger.h"

namespace Bearish { namespace Scripting {
	static HSQUIRRELVM Squirrel;
	static i32 INIT_STACK_SIZE = 1024;

	static void sq_print(HSQUIRRELVM v, const SQChar* s, ...) {
		va_list vl;
		va_start(vl, s);

		c8 buffer[2000];
		vsprintf_s(buffer, s, vl);
		
		Core::Logger::Info(buffer);
		
		va_end(vl);
	}

	static void sq_error(HSQUIRRELVM v, const SQChar* s, ...) {
		va_list vl;
		va_start(vl, s);

		c8 buffer[2000];
		vsprintf_s(buffer, s, vl);

		Core::Logger::Error(buffer);

		va_end(vl);
	}
	
	static void InitSquirrel() {
		// Create vm
		Squirrel = sq_open(INIT_STACK_SIZE);

		// Register stdlib.
		sq_pushroottable(Squirrel);
		sqstd_register_bloblib(Squirrel);
		sqstd_register_iolib(Squirrel);
		sqstd_register_mathlib(Squirrel);
		sqstd_register_stringlib(Squirrel);
		sqstd_register_systemlib(Squirrel);
		sq_pop(Squirrel, 1);

		sqstd_seterrorhandlers(Squirrel);
		sq_setprintfunc(Squirrel, sq_print, sq_error);

		// TODO: Register engine functions.

		// Push the global table to the stack, the VM is ready to use.
		sq_pushroottable(Squirrel);
	}


	static void KillSquirrel() {
		sq_pop(Squirrel, 1);
		sq_close(Squirrel);
	}

	static void RunFile(string file) {
		if (!SQ_SUCCEEDED(sqstd_dofile(Squirrel, _SC(file.c_str()), SQFalse, SQTrue))) {
			Core::Logger::Error("Erorr running script: %s", file.c_str());
		}
	}

	static void RunString(string cmd) {
		SQInteger oldtop = sq_gettop(Squirrel);
		sq_compilebuffer(Squirrel, cmd.c_str(), (i32)strlen(cmd.c_str()) * sizeof(SQChar), "EVAL", 1);
		sq_pushroottable(Squirrel);
		sq_call(Squirrel, 1, SQFalse, SQTrue);
		sq_settop(Squirrel, oldtop);
	}
} }
*/
#endif // _BEARISH_SCRIPTING_ENGINE_H_
