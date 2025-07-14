#pragma once

#include "GuiWindow.h"

#include "Engine/gbe_engine.h"

#include "ImGuiFileDialog/ImGuiFileDialog.h"

#include <string>

namespace gbe {
	namespace editor {

		class ImageDropzone : public GuiWindow {
			void DrawSelf() override;
			std::string GetWindowId() override;
		
		private:
			void openFileDialogue();
			void handleFileDialogueResult();
			const std::filesystem::path bgImageDir = std::filesystem::path("/Gab-Engine-JRDW/DefaultAssets/Tex/UI");
			
		
		};
	}
}
