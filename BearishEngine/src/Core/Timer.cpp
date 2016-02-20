#include "Timer.h"

using namespace Bearish;
using namespace Core;

Timer::Timer() {
	_lastTime = 0;
}

Timer::~Timer() {
}

void Timer::Start() {
	_lastTime = (f32)glfwGetTime();
}

const f32 Timer::Loop() {
	const f32 res = (f32)glfwGetTime() - _lastTime;
	_lastTime = (f32)glfwGetTime();
	return res;
}

const f32 Timer::LoopMS() {
	const f32 res = (f32)(glfwGetTime() - _lastTime) * 1000.f;
	_lastTime = (f32)glfwGetTime();
	return res;
}

const f32 Timer::Delta() const {
	return (f32)(glfwGetTime() - _lastTime);
}

const f32 Timer::DeltaMS() const {
	return (f32)((glfwGetTime() - _lastTime) * 1000.f);
}