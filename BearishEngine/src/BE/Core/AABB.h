#ifndef _BEARISH_CORE_AABB_H_
#define _BEARISH_CORE_AABB_H_

#include <BE/Types.h>
#include <BE/Math/vec3.h>
#include <BE/Core/IAllocatable.h>

namespace Bearish { namespace Core {
	class BEARISH_API AABB : IAllocatable<AABB> {
	public:
		AABB(Math::vec3 position = vec3(0.f), Math::vec3 halfBounds = vec3(0.01f)) : _position(position), _halfBounds(halfBounds), _scale(vec3(1.f)) {}
		~AABB() {}

		bool Intersects(const AABB& other) const {
			Math::vec3 p = other.GetPosition(), r = other.GetHalfBounds();
			bool x = std::fabs(_position.x - p.x) <= (_halfBounds.x + r.x);
			bool y = std::fabs(_position.y - p.y) <= (_halfBounds.y + r.y);
			bool z = std::fabs(_position.z - p.z) <= (_halfBounds.z + r.z);

			return x && y && z;
		}

		Math::vec3 GetPosition() const { return _position; }
		Math::vec3 GetHalfBounds() const { return _halfBounds; }
		Math::vec3 GetScale() const { return _scale; }

		void SetPosition(Math::vec3 position) { _position = position; }
		void SetHalfBounds(Math::vec3 halfBounds) { _halfBounds = halfBounds; }
		void SetScale(Math::vec3 scale) { _scale = scale; }
		void Fit(Math::vec3 max) { _halfBounds.Grow(max); }
	private:
		Math::vec3 _position, _halfBounds, _scale;
	};
} }

#endif // _BEARISH_CORE_AABB_H_
