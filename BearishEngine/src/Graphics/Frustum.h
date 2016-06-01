#ifndef _BEARISH_GRAPHICS_FRUSTUM_H_
#define _BEARISH_GRAPHICS_FRUSTUM_H_

#include "../Types.h"
#include "../Core/IAllocatable.h"
#include "../Math/vec3.h"
#include "../Math/mat4.h"

namespace Bearish { namespace Graphics {
	class Frustum : Core::IAllocatable<Frustum> {
	public:
		Frustum(const Math::mat4& mvp);

		bool PointIntersects(const Math::vec3& point) const;
		bool SphereIntersects(const Math::vec3& pos, f32 rad) const;
	private:
		enum class Plane {
			Top = 0, Bottom, Left, Right, Near, Far
		};

		struct {
		public:
			void Set(f32 a, f32 b, f32 c, f32 d) {
				normal = Math::vec3(a, b, c);
				f32 len = normal.Length();
				this->d = d / len;
				normal.Normalize();
			}

			f32 Distance(const Math::vec3& point) const {
				return d + normal.Dot(point);
			}

			Math::vec3 normal;
			f32 d;
		} _planes[6];
	};
} }

#endif // _BEARISH_GRAPHICS_FRUSTUM_H_
