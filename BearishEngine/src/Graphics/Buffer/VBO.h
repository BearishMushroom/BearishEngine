#ifndef _BEARISH_GRAPHICS_VBO_H_
#define _BEARISH_GRAPHICS_VBO_H_
	
#include "../../Core/IAllocatable.h"
#include "../../Types.h"
#include "../Renderer.h"
#include "../Vertex.h"

namespace Bearish { namespace Graphics {
	class VBO : public Core::IAllocatable<VBO> {
	public:
		VBO();
		~VBO();

 	    void Bind() const;
		void BindForFeedback() const;
		static void Unbind();

		void SetData(void* data, const u64 dataSize, const DrawStyle style = DrawStyle::Static) const;
		void SetData(Vertex* vertices, const u64 numVertices, const DrawStyle style = DrawStyle::Static) const;
		void SubData(void* data, const u64 start, const u64 dataSize);
	private:
		u32 _id;
	};
} }

#endif