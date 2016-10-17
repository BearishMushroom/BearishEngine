#include "Timer.h"
#include <chrono>

using namespace Bearish;
using namespace Core;

Timer::Timer() {
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	_freq = 1.0 / frequency.QuadPart;
}

Timer::~Timer() {
}

void Timer::Start() {
	QueryPerformanceCounter(&_start);
}

const f32 Timer::Loop() {
	const f32 res = Delta();
	Start();
	return res;
}

const f32 Timer::LoopMS() {
	return Loop() * 1000;
}

const f32 Timer::Delta() const {
	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);
	u64 cycles = current.QuadPart - _start.QuadPart;
	return (float)(cycles * _freq);
}

const f32 Timer::DeltaMS() const {
	return Delta() * 1000;
}