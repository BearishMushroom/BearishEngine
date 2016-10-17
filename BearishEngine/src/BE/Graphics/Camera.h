#ifndef _BEARISH_GRAPHICS_CAMERA_H_
#define _BEARISH_GRAPHICS_CAMERA_H_

#include <BE/Core/IAllocatable.h>
#include <BE/Core/Transform.h>
#include <BE/Types.h>

namespace Bearish { namespace Graphics {
	class BEARISH_API Camera : public Core::IAllocatable<Camera> {
	public:
		static Camera* Identity;

		Camera(const f32 fov, const f32 aspect, const f32 nearB, const f32 farB, const Core::Transform& transform = Core::Transform());
		Camera(const Math::mat4& proj = Math::mat4(), const Core::Transform& transform = Core::Transform());
		~Camera();

		Core::Transform& GetTransform();
		Math::mat4 GetProjection() const;

		void SetTransform(const Core::Transform& value);
		void SetProjection(const Math::mat4& value);
		
		Math::mat4 GetViewMatrix() const;

		const f32 GetNear()   const { return _near; }
		const f32 GetFar()    const { return _far; }
		const f32 GetAspect() const { return _aspect; }
		const f32 GetFOV()    const { return _fov;  }
	private:
		f32 _near, _far, _aspect, _fov;
		Math::mat4 _projection;
		Core::Transform _transform;
	};
} }

#endif
