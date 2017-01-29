#ifndef _BEARISH_GRAPHICS_API_SURFACE_H_
#define _BEARISH_GRAPHICS_API_SURFACE_H_

#include <BE/Types.h>
#include <BE/Graphics/API/Device.h>

namespace Bearish { namespace Graphics { namespace API {
	// @PLATFORM: This is Win32 only.
	
	class Surface {
	public:
		Surface(const Device& device, HWND window);
		~Surface();

		operator VkSurfaceKHR() const { return _surface; }
	private:
		const Device& _device;
		VkSurfaceKHR _surface;
	};
} } }

#endif // _BEARISH_GRAPHICS_API_SURFACE_H_
