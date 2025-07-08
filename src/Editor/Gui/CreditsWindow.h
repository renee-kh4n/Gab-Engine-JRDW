#pragma once

#include "GuiWindow.h"

#include <string>

namespace gbe {
	namespace editor {
		class CreditsWindow : public GuiWindow {
			std::string GetWindowId() override;
		public:
			void DrawSelf() override;
		};
	}
}