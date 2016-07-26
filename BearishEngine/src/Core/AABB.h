#ifndef _BEARISH_CORE_AABB_H_
#define _BEARISH_CORE_AABB_H_

#include "../Types.h"
#include "../Math/vec3.h"
#include "IAllocatable.h"

namespace Bearish { namespace Core {
	class AABB : IAllocatable<AABB> {
	public:
		AABB(Math::vec3 position, Math::vec3 halfBounds) : _position(position), _halfBounds(halfBounds) {}
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

		void SetPosition(Math::vec3 position) { _position = position; }
		void SetPosition(Math::vec3 halfBounds) { _halfBounds = halfBounds; }
	private:
		Math::vec3 _position, _halfBounds;
	};
} }

#endif // _BEARISH_CORE_AABB_H_
