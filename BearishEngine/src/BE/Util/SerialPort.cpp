#include "SerialPort.h"

using namespace Bearish;
using namespace Util;

SerialPort::SerialPort(string portName) {
	_connected = false;

	_hSerial = CreateFile(portName.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (_hSerial == INVALID_HANDLE_VALUE) {
		if (GetLastError() == ERROR_FILE_NOT_FOUND) {
			printf("ERROR: Handle was not attached. Reason: %s not available.\n", portName.c_str());
			return;
		} else {
			printf("ERROR!!!");
			return;
		}
	} else {
		DCB dcbSerialPortParams = { 0 };

		if (!GetCommState(_hSerial, &dcbSerialPortParams)) {
			printf("failed to get current SerialPort parameters!");
			return;
		} else {
			dcbSerialPortParams.BaudRate = CBR_9600;
			dcbSerialPortParams.ByteSize = 8;
			dcbSerialPortParams.StopBits = ONESTOPBIT;
			dcbSerialPortParams.Parity = NOPARITY;
			dcbSerialPortParams.fDtrControl = DTR_CONTROL_ENABLE;

			if (!SetCommState(_hSerial, &dcbSerialPortParams)) {
				printf("ALERT: Could not set SerialPort Port parameters\n");
				return;
			} else {
				_connected = true;
				PurgeComm(_hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
				Sleep(2000);
			}
		}
	}

}

SerialPort::~SerialPort(){
	if (_connected) {
		_connected = false;
		CloseHandle(_hSerial);
	}
}

i32 SerialPort::ReadData(u8* buffer, i32 nbChar) {
	DWORD bytesRead;
	u32 toRead;

	ClearCommError(_hSerial, &_errors, &_status);

	if (_status.cbInQue > 0) {
		if (_status.cbInQue > nbChar) {
			toRead = nbChar;
		} else {
			toRead = _status.cbInQue;
		}

		if (ReadFile(_hSerial, buffer, toRead, &bytesRead, NULL)) {
			return bytesRead;
		}

	}

	return 0;
}


bool SerialPort::WriteData(u8* buffer, i32 nbChar) {
	DWORD bytesSend;

	if (!WriteFile(_hSerial, (void *)buffer, nbChar, &bytesSend, 0)) {
		ClearCommError(_hSerial, &_errors, &_status);
		return false;
	}
	else {
		return true;
	}
}

bool SerialPort::IsConnected() {
	return _connected;
}