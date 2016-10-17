#ifndef _BEARISH_GRAPHICS_INDEXBUFFER_H_
#define _BEARISH_GRAPHICS_INDEXBUFFER_H_

#include "../../Core/IAllocatable.h"
#include "../../Types.h"

namespace Bearish { namespace Graphics {
	class BEARISH_API IndexBuffer : public Core::IAllocatable<IndexBuffer> {
	public:
		IndexBuffer();
		~IndexBuffer();

		void Bind() const;
		static void Unbind();

		void SetData(u32* indices, const u32 numIndices);
		
		const i32 GetSize() const;
	private:
		u32 _id;
		i32 _size;
	};
} }

#endif
