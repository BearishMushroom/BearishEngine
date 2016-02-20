#include "MeshAnimation.h"

using namespace Bearish;
using namespace Graphics;
using namespace Math;

bool MeshAnimation::Update(f32 time, MeshNode* root) {
	if (_active) {
		_time += time * _timeScale;
		f32 timeInTicks = _time * _tickRate;
		f32 animationTime = fmod(timeInTicks, _duration);

		_transforms.resize(_skeleton->GetNumBones());
		root->Animate(this, _skeleton, animationTime);
		return true;
	}

	return false;
}

MeshNodeAnimation* MeshAnimation::GetNodeAnimation(string name) {
	for (i32 i = 0; i < (i32)_channels.size(); i++) {
		if (_channels[i]->name == name) {
			return _channels[i];
		}
	}

	return 0;
}


void MeshAnimation::Start() {
	_active = true;
}

void MeshAnimation::Pause() {
	_active = false;
}

void MeshAnimation::Stop() {
	_active = false;
	_time = 0;
}

void MeshAnimation::SetTime(f32 time) {
	_time = _duration * time;
}

void MeshAnimation::SetTimeScale(f32 scale) {
	_timeScale = scale;
}