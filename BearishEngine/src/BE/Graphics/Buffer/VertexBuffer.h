#ifndef _BEARISH_GRAPHICS_VERTEXBUFFER_H_
#define _BEARISH_GRAPHICS_VERTEXBUFFER_H_
	
#include <BE/Core/IAllocatable.h>
#include <BE/Types.h>
#include <BE/Graphics/Renderer.h>
#include <BE/Graphics/Vertex.h>
#include <BE/Graphics/Buffer/VertexLayout.h>

namespace Bearish { namespace Graphics {
	class BEARISH_API VertexBuffer : public Core::IAllocatable<VertexBuffer> {
	public:
		VertexBuffer();
		~VertexBuffer();

 	    void Bind() const;
		void BindForFeedback() const;
		static void Unbind();

		void SetData(void* data, const u64 dataSize, const DrawStyle style = DrawStyle::Static) const;
		void SetData(Vertex* vertices, const u64 numVertices, const DrawStyle style = DrawStyle::Static) const;
		void SubData(void* data, const u64 start, const u64 dataSize);

		template<typename T>
		void SetLayout(VertexLayout<T>& layout) {
			auto& spec = layout.GetSpecification();
			i32 size = layout.GetSize();

			for (u32 i = 0; i < spec.size(); i++) {
				auto item = spec.at(i);

				switch (item.type) {
				case VertexComponentType::Attribute: {
					Renderer::EnableAttribArray(i);
					Renderer::SetAttribPointer(i, item.size / sizeof(f32), item.stride, item.offset);
				} break;
				default: {
					_attribLocs.push_back(i);
					_attribConstants.push_back(new VertexComponent(item));
				} break;
				}
			}
		}

		void Apply() const {
			for (u32 i = 0; i < _attribConstants.size(); i++) {
				auto& attrib = _attribConstants.at(i);
				i32 loc = _attribLocs.at(i);
				switch (attrib->type) {
				case VertexComponentType::Vec2:
					Renderer::SetAttribValue(loc, attrib->v2d);
					break;
				case VertexComponentType::Vec3:					
					Renderer::SetAttribValue(loc, attrib->v3d);
					break;
				case VertexComponentType::Vec4:
					Renderer::SetAttribValue(loc, attrib->v4d);
					break;
				case VertexComponentType::Vec4i:
					Renderer::SetAttribValue(loc, attrib->v4di);
					break;
				}
			}
		}
	private:
		std::vector<i32> _attribLocs;
		std::vector<VertexComponent*> _attribConstants;
		u32 _id;
	};
} }

#endif