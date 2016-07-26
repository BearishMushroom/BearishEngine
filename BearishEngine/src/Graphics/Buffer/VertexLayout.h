#ifndef _BEARISH_GRAPHICS_VERTEXLAYOUT_H_
#define _BEARISH_GRAPHICS_VERTEXLAYOUT_H_

#include "../../Types.h"
#include "../../Core/IAllocatable.h"
#include "../../Math/vec2.h"
#include "../../Math/vec3.h"
#include "../../Math/vec4.h"

namespace Bearish { namespace Graphics {
	enum class VertexComponentType : u8 {
		Attribute,
		Vec2,
		Vec3,
		Vec4,
		Vec4i
	};

	class VertexComponent {
	public:
		VertexComponentType type;

		string name;
		i32 offset, size, stride;

		Math::vec2 v2d;
		Math::vec3 v3d;
		Math::vec4 v4d;
		Math::vec4i v4di;
	};

	template<typename T>
	class VertexLayout : Core::IAllocatable<VertexLayout<T>> {
	public:

		VertexLayout() : _size(sizeof(T)), _offset(0) {}
		~VertexLayout() {}

		void PushComponent(string name, i32 size) {
			VertexComponent cmp;
			cmp.type = VertexComponentType::Attribute;
			cmp.name = name;
			cmp.offset = _offset;
			cmp.size = size;
			cmp.stride = _size;

			_spec.push_back(cmp);
			_offset += size;
		}

		template<typename V>
		void PushComponent(string name = "") {
			PushComponent(name, sizeof(V));
		}

		void PushComponent(Math::vec2 value) {
			VertexComponent cmp;
			cmp.type = VertexComponentType::Vec2;
			cmp.v2d = value;
			_spec.push_back(cmp);
		}

		void PushComponent(Math::vec3 value) {
			VertexComponent cmp;
			cmp.type = VertexComponentType::Vec3;
			cmp.v3d = value;
			_spec.push_back(cmp);
		}

		void PushComponent(Math::vec4 value) {
			VertexComponent cmp;
			cmp.type = VertexComponentType::Vec4;
			cmp.v4d = value;
			_spec.push_back(cmp);
		}

		void PushComponent(Math::vec4i value) {
			VertexComponent cmp;
			cmp.type = VertexComponentType::Vec4i;
			cmp.v4di = value;
			_spec.push_back(cmp);
		}

		void PopComponent() {
			_spec.pop_back();
		}

		const std::vector<VertexComponent>& GetSpecification() const { return _spec; }
		i32 GetSize() const { return _size; }
	private:
		std::vector<VertexComponent> _spec;
		i32 _size, _offset;
	};
} }

#endif // _BEARISH_GRAPHICS_VERTEXLAYOUT_H_
