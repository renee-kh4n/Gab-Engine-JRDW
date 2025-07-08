#pragma once

#include "GuiWindow.h"

#include <string>

namespace gbe {
	namespace editor {
		class ColorpickerWindow : public GuiWindow {
			void DrawSelf() override;
			std::string GetWindowId() override;
		public:
			float rgb[3];
		};
	}
}