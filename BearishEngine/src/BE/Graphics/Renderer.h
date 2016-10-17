#ifndef _BEARISH_GRAPHICS_RENDERER_H_
#define _BEARISH_GRAPHICS_RENDERER_H_

#include <BE/Core/IAllocatable.h>
#include <BE/Types.h>
#include <BE/Math/vec2.h>
#include <BE/Math/vec3.h>
#include <BE/Math/vec4.h>
#include "OpenGL.h"

namespace Bearish { namespace Graphics {
	enum class AttributeType : u32 {
		Int32   = GL_INT,
		Float32 = GL_FLOAT,
	};

	enum class PrimitiveMode : u32 {
		Triangles = GL_TRIANGLES,
		Lines     = GL_LINES,
		Points    = GL_POINTS,
		LineStrip = GL_LINE_STRIP,
	};

	enum class DrawStyle : u32 {
		Static  = GL_STATIC_DRAW,
		Dynamic = GL_DYNAMIC_DRAW,
		Stream  = GL_STREAM_DRAW,
	};

	enum class BlendMode : u32 {
		One                      = GL_ONE,
		Zero                     = GL_ZERO,
		Source                   = GL_SRC_COLOR,
		SourceAlpha              = GL_SRC_ALPHA,
		Destination              = GL_DST_COLOR,
		DestinationAlpha         = GL_DST_ALPHA,
		OneMinusSource           = GL_ONE_MINUS_SRC_COLOR,
		OneMinusSourceAlpha      = GL_ONE_MINUS_SRC_ALPHA,
		OneMinusDestination      = GL_ONE_MINUS_DST_COLOR,
		OneMinusDestinationAlpha = GL_ONE_MINUS_DST_ALPHA,
	};

	enum class CullMode : u32 {
		Front = GL_FRONT,
		Back = GL_BACK,
	};

	enum class DepthMode : u32 {
		Equal = GL_EQUAL,
		LessOrEqual = GL_LEQUAL,
		GreaterOrEqual = GL_GEQUAL,
		Less = GL_LESS,
		Greater = GL_GREATER,
	};

	class BEARISH_API BlendState {
	public:
		static const BlendState AlphaBlend;
		static const BlendState Additive;

		BlendState(BlendMode src, BlendMode dst) : source(src), destination(dst) {}
		BlendMode source, destination;
	};

	enum class FillMode : u32 {
		Normal    = GL_FILL,
		Wireframe = GL_LINE,
		Points    = GL_POINT,
	};

	class BEARISH_API Renderer : public Core::IAllocatable<Renderer> {
	public:

		static const u32 POSITION_ATTRIBUTE    = 0;
		static const u32 TEXCOORD_ATTRIBUTE    = 1;
		static const u32 NORMAL_ATTRIBUTE      = 2;
		static const u32 TANGENT_ATTRIBUTE     = 3;
		static const u32 BONEID_ATTRIBUTE      = 4;
		static const u32 BONEWEIGHT_ATTRIBUTE  = 5;
		static const u32 WORLD_ATTRIBUTE       = 6;  // A matrix uses 4 attrib spots. 6, 7, 8, 9
		static const u32 MVP_ATTRIBUTE         = 10; // A matrix uses 4 attrib spots. 10, 11, 12, 13

		static const u32 UI_RESOLUTION_X       = 1920;
		static const u32 UI_RESOLUTION_Y       = 1080;

		static void EnableAttribArray(const u32 num);
		static void DisableAttribArray(const u32 num);
		static void SetAttribPointer(const u32 num, const u32 numElements, const u32 vertexSize, const u32 offset, const AttributeType type = AttributeType::Float32);
		static void SetAttribDivisor(const u32 num, const u32 divisor);

		static void SetAttribValue(const u32 num, Math::vec2& value);
		static void SetAttribValue(const u32 num, Math::vec3& value);
		static void SetAttribValue(const u32 num, Math::vec4& value);
		static void SetAttribValue(const u32 num, Math::vec4i& value);
		
		static void SetBlendMode(const BlendMode src, const BlendMode dst);
		static void SetBlendState(const BlendState state);
		static void SetFillMode(const FillMode mode);

		static void SetCullMode(const CullMode mode);
		static void SetDepthMode(const DepthMode mode);

		static FillMode GetFillMode() { return _renderMode; }
		static BlendState GetBlendState() { return _blendState; }

		static PrimitiveMode GetPrimitiveMode() { return _primitiveMode; }
		static void SetPrimitiveMode(const PrimitiveMode priMode);

		Renderer();
		~Renderer();
		
		void Init() const;

		void Clear() const;
	private:
		static BlendState _blendState;
		static FillMode _renderMode;
		static PrimitiveMode _primitiveMode;
	};
} }

#endif
