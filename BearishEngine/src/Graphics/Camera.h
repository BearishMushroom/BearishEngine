#ifndef _BEARISH_GRAPHICS_CAMERA_H_
#define _BEARISH_GRAPHICS_CAMERA_H_

#include "../Core/IAllocatable.h"
#include "../Core/Transform.h"
#include "../Types.h"

namespace Bearish { namespace Graphics {
	class Camera : public Core::IAllocatable<Camera> {
	public:
		Camera(const Math::mat4& projection = Math::mat4(), const Core::Transform& transform = Core::Transform());
		~Camera();

		Core::Transform& GetTransform();
		Math::mat4 GetProjection() const;

		void SetTransform(const Core::Transform& value);
		void SetProjection(const Math::mat4& value);
		
		Math::mat4 GetViewMatrix() const;
	private:
		Math::mat4 _projection;
		Core::Transform _transform;
	};
} }

#endif
