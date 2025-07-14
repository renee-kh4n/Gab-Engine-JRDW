#pragma once

#include "GuiWindow.h"

#include "Engine/gbe_engine.h"

#include <string>

namespace gbe {
	namespace editor {

		class ImageDropzone : public GuiWindow {
			void DrawSelf() override;
			std::string GetWindowId() override;
		public:
			Time* mtime;
			std::vector<gbe::Object*>* selected;
		};
	}
}
