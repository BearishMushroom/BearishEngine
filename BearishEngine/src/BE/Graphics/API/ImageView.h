#ifndef _BEARISH_GRAPHICS_API_IMAGE_VIEW_H_
#define _BEARISH_GRAPHICS_API_IMAGE_VIEW_H_

#include <BE/Types.h>

#include <BE/Graphics/API/Device.h>
#include <BE/Core/Logger.h>

namespace Bearish { namespace Graphics { namespace API {
	class ImageView {
	public:
		enum class Swizzle : u32 {
			Zero = 0x01,
			One = 0x02,
			R = 0x03,
			G = 0x04,
			B = 0x05,
			A = 0x06,

			RGBA = (Swizzle::R << 24) | (Swizzle::G << 16) | (Swizzle::B << 8) | Swizzle::A,
		};

		ImageView() { _view = 0; _device = 0; }

		ImageView(const Device* device, VkImage image, VkFormat format, Swizzle swizzle = Swizzle::RGBA) {
			_device = device;
			_swizzle = swizzle;

			VkImageViewCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			info.image = image;
			info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			info.format = format;

			info.components.g = (VkComponentSwizzle)(((u32)swizzle & 0x00FF0000) >> 16);
			info.components.r = (VkComponentSwizzle)(((u32)swizzle & 0xFF000000) >> 24);
			info.components.b = (VkComponentSwizzle)(((u32)swizzle & 0x0000FF00) >> 8 );
			info.components.a = (VkComponentSwizzle)(((u32)swizzle & 0x000000FF) >> 0 );

			info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			info.subresourceRange.baseMipLevel = 0;
			info.subresourceRange.levelCount = 1;
			info.subresourceRange.baseArrayLayer = 0;
			info.subresourceRange.layerCount = 1;

			if (VkError(vkCreateImageView(*_device, &info, nullptr, &_view))) {
				Core::Logger::Fatal("Failed to create Vulkan image view!");
			}
		}

		~ImageView() {
			if (_view) {
				vkDestroyImageView(*_device, _view, nullptr);
				_view = 0;
			}
		}

		operator VkImageView() const { return _view; }
	private:
		VkImageView _view;
		Swizzle _swizzle;
		const Device* _device;
	};
} } }

#endif // _BEARISH_GRAPHICS_API_IMAGE_VIEW_H_
