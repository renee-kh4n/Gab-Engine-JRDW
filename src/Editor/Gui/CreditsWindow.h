#pragma once

#include "GuiWindow.h"

#include "Graphics/gbe_graphics.h"

#include <imgui_impl_vulkan.h>

#include <string>

namespace gbe {
	namespace editor {
		class CreditsWindow : public GuiWindow {
			std::string GetWindowId() override;

			gbe::asset::Texture* logo_tex;
			VkDescriptorSet logo_tex_DS;

		public:
			CreditsWindow();
			void DrawSelf() override;
		};
	}
}