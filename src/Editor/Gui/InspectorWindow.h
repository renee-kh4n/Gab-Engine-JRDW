#pragma once

#include "GuiWindow.h"
#include "InspectorData.h"

#include "Engine/gbe_engine.h"

#include <string>

namespace gbe {
	namespace editor {

		class InspectorWindow : public GuiWindow {
			void DrawSelf() override;
			std::string GetWindowId() override;
		public:
			Time* mtime;
			std::vector<gbe::Object*>* selected;
		};
	}
}