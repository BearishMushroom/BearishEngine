#ifndef _BEARISH_GRAPHICS_PARTICLE_H_
#define _BEARISH_GRAPHICS_PARTICLE_H_

#include "../Core/IAllocatable.h"
#include "../Math/vec3.h"
#include "../Math/vec4.h"
#include "Renderer.h"
#include "../Types.h"

namespace Bearish { namespace Graphics {
	class Particle : public Core::IAllocatable<Particle> {
	public:
		Math::vec3 position;
		Math::vec3 velocity;
		Math::vec3 scale;
		f32 rotation;
		f32 angularVelocity;
		Math::vec4 color;
		f32 life;
		f32 cameraDistance;

		inline bool operator<(Particle& other) {
			return this->cameraDistance > other.cameraDistance;
		}
	};
} }

#endif // _BEARISH_GRAPHICS_PARTICLE_H_