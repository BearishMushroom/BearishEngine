#ifndef _BEARISH_GRAPHICS_PARTICLESYSTEM_H_
#define _BEARISH_GRAPHICS_PARTICLESYSTEM_H_

#include "../Core/IAllocatable.h"
#include "Buffer/VBO.h"
#include "Mesh\Mesh.h"
#include "Particle.h"
#include "../Types.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"

namespace Bearish { namespace Graphics {
	enum class ParticleBlendMode {
		
	};

	class ParticleSystem : public Core::IAllocatable<ParticleSystem> {
	public:
		ParticleSystem(Texture* texture, std::function<Particle(const Math::vec3&)> emitFunc, std::function<void(Particle*, f32)> updateFunc);
		~ParticleSystem();

		void Emit(i32 count, const Math::vec3& position);
		void Update(Camera* camera, f32 dt);
		void Draw(Camera* camera);
	private:
		static const i32 MAX_PARTICLES;
		
		std::vector<Particle> _particles;

		std::function<Particle(const Math::vec3&)> _emit;
		std::function<void(Particle*, f32)> _update;
		Shader* _shader;
		Texture* _texture;
		VAO _state;
		VBO _renderData;
		i32 _aliveParticles;
	};
} }

#endif // _BEARISH_GRAPHICS_PARTICLESYSTEM_H_