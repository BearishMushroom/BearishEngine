#ifndef _BEARISH_UTIL_SERIALPORT_H_
#define _BEARISH_UTIL_SERIALPORT_H_

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include <BE\Utils.h>

namespace Bearish { namespace Util {
	class SerialPort {
	public:
		SerialPort(string portName);
		~SerialPort();
		i32 ReadData(u8* buffer, i32 size);
		bool WriteData(u8* buffer, i32 size);
		bool IsConnected();
	private:
		HANDLE _hSerial;
		bool _connected;
		COMSTAT _status;
		DWORD _errors;
	};
} }

#endif // _BEARISH_UTIL_SERIALPORT_H_