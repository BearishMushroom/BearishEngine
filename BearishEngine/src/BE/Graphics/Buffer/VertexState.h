#ifndef _BEARISH_GRAPHICS_VERTEXSTATE_H_
#define _BEARISH_GRAPHICS_VERTEXSTATE_H_

#include "../../Core/IAllocatable.h"
#include "../../Types.h"

namespace Bearish { namespace Graphics {
	class BEARISH_API VertexState : public Core::IAllocatable<VertexState> {
	public:
		VertexState();
		~VertexState();

		void Bind() const;
		static void Unbind();

		u32 GetID() { return _id; }
	private:
		u32 _id;
	};
} }

#endif // _BEARISH_GRAPHICS_VERTEXSTATE_H_