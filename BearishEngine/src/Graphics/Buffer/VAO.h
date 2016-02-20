#ifndef _BEARISH_GRAPHICS_VAO_H_
#define _BEARISH_GRAPHICS_VAO_H_

#include "../../Core/IAllocatable.h"
#include "../../Types.h"

namespace Bearish { namespace Graphics {
	class VAO : public Core::IAllocatable<VAO> {
	public:
		VAO();
		~VAO();

		void Bind() const;
		static void Unbind();

		u32 GetID() { return _id; }
	private:
		u32 _id;
	};
} }

#endif // _BEARISH_GRAPHICS_VAO_H_