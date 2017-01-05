#ifndef _BEARISH_CORE_TRANSFORM_H_
#define _BEARISH_CORE_TRANSFORM_H_

#include "IAllocatable.h"
#include <BE/Math/vec3.h>
#include <BE/Math/quat.h>
#include <BE/Math/mat4.h>
#include <BE/Types.h>
#include <BE/Serialization/Serialization.h>

namespace Bearish {	namespace Core {
	class BEARISH_API Transform : public IAllocatable<Transform> {
	public:
		Transform(Math::vec3 translation = Math::vec3(0, 0, 0), Math::vec3 scale = Math::vec3(1, 1, 1), Math::quat rotation = Math::quat(0, 0, 0, 1));
		Transform(const Math::mat4& transformation);
		~Transform();

		Math::vec3& GetTranslation();
		void SetTranslation(Math::vec3 value);
		
		Math::quat& GetRotation();
		void SetRotation(Math::quat value);
		
		Math::vec3& GetScale();
		void SetScale(Math::vec3 value);
		
		void Rotate(const Math::vec3& axis, f32 angle);
		void Rotate(const Math::quat& rotation);
		void LookAt(const Math::vec3& point, const Math::vec3& up);
		
		Math::mat4 GetTransformation() const;
		Math::mat4 GetCameraTransformation() const;

		template <typename Archive>
		void serialize(Archive& ar) {
			ar(CEREAL_NVP(_translation), CEREAL_NVP(_scale), CEREAL_NVP(_rotation));
		}
	private:
		Math::vec3 _translation;
		Math::vec3 _scale;
		Math::quat _rotation;
	};
} }

#endif