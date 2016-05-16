#ifndef _BEARISH_GRAPHICS_UBO_H_
#define _BEARISH_GRAPHICS_UBO_H_

#include "../../Types.h"
#include "../../Core/IAllocatable.h"

namespace Bearish { namespace Graphics {
	class UBO : public Core::IAllocatable<UBO> {
	public:
		UBO();
		~UBO();

		void SetData(void* data, u32 size);
		
		template<typename T>
		void SetData(T& data) {
			SetData((void*)&data, sizeof(T));
		}

		void UpdateData(void* data, u32 size);

		template<typename T>
		void UpdateData(T& data) {
			UpdateData((void*)&data, sizeof(T));
		}

		void Bind(i32 base = -1);
		void Unbind(i32 base = -1);

		void SetBase(i32 base) { _base = base; }
	private:
		u32 _id;
		i32 _base;
	};
} }

#endif // _BEARISH_GRAPHICS_UBO_H_