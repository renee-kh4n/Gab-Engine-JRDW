#pragma once

#include <typeinfo>
#include <string>

#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_sdl2.h>

namespace gbe {
	namespace editor {
		class GuiWindow {
		public:
			void Draw();
		protected:
			virtual void DrawSelf() = 0;
			virtual std::string GetWindowId() = 0;
		private:
			void ext_Begin();
			void ext_End();
		};
	}
}