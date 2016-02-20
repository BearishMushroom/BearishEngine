#ifndef _BEARISH_GRAPHICS_LIGHTS_H_
#define _BEARISH_GRAPHICS_LIGHTS_H_

#include "../Core/IAllocatable.h"
#include "../Math/vec3.h"
#include "../Math/quat.h"
#include "../Types.h"

namespace Bearish { namespace Graphics {
	enum class LightType : u8 {
		None,
		Directional,
		Point,
		Spot,
	};

	class Attenuation : public Core::IAllocatable<Attenuation> {
	public:
		Attenuation(f32 constant = 0, f32 linear = 0, f32 exponent = 1) : _constant(constant), _linear(linear), _exponent(exponent) {}
		~Attenuation() {}

		f32 GetConstant() const { return _constant; }
		f32 GetLinear() const { return _linear; }
		f32 GetExponent() const { return _exponent; }
	private:
		f32 _constant, _linear, _exponent;
	};

	class Light {
	public:
		Light(const Math::vec3& color = Math::vec3(0, 0, 0), f32 ambientIntensity = 0, f32 diffuseIntensity = 0, LightType type = LightType::None) :
			_color(color), _ambientIntensity(ambientIntensity), _diffuseIntensity(diffuseIntensity), _type(type) {}
		~Light() {}

		f32 GetAmbientIntensity() const { return _ambientIntensity; }
		f32 GetDiffuseIntensity() const { return _diffuseIntensity; }
		Math::vec3 GetColor() const { return _color; }
		LightType GetType() const { return _type;}
	protected:
		f32 _ambientIntensity, _diffuseIntensity;
		Math::vec3 _color;
		LightType _type;
	};

	class DirectionalLight : public Light {
	public:
		DirectionalLight(const Math::vec3& color = Math::vec3(0, 0, 0), f32 ambientIntensity = 0, const Math::quat& direction = Math::quat(0, 0, 0, 1), f32 diffuseIntensity = 0) :
			Light(color, ambientIntensity, diffuseIntensity, LightType::Directional), _direction(direction) {}
		~DirectionalLight() {}

		Math::quat GetDirection() const { return _direction; }
	protected:
		Math::quat _direction;
	};

	class PointLight : public Light {
	public:
		PointLight(const Math::vec3& color = Math::vec3(0, 0, 0), f32 ambientIntensity = 0, const Attenuation& atten = Attenuation(), f32 diffuseIntensity = 0, const Math::vec3& position = Math::vec3(0, 0, 0)) :
			Light(color, ambientIntensity, diffuseIntensity, LightType::Point), _atten(atten), _position(position) {}
		~PointLight() {}

		Attenuation GetAttenuation() const { return _atten; }
		Math::vec3 GetPosition() const { return _position; }
	protected:
		Attenuation _atten;
		Math::vec3 _position;
	};

	class SpotLight : public PointLight {
	public:
		SpotLight(const Math::vec3& color = Math::vec3(0, 0, 0), f32 ambientIntensity = 0, const Attenuation& atten = Attenuation(), f32 diffuseIntensity = 0, const Math::vec3& position = Math::vec3(0, 0, 0),
			const Math::quat& direction = Math::quat(0, 0, 0, 1), f32 cutoff = 0) : PointLight(color, ambientIntensity, atten, diffuseIntensity, position), _cutoff(cutoff), _direction(direction)
			{ _type = LightType::Spot; }

		~SpotLight() {}

		f32 GetCutoff() const { return _cutoff; }
		Math::quat GetDirection() const { return _direction; }
	protected:
		f32 _cutoff;
		Math::quat _direction;
	};
} }

#endif
