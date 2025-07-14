#include "ImageDropzone.h"

#include "../Utility/ModelExport.h"

#include <filesystem>

namespace fs = std::filesystem;

void gbe::editor::ImageDropzone::DrawSelf() {
	bool toggle = false;

	if (ImGui::Button("Upload Image (supported file type: PNG)")) {
		toggle = !toggle;
	}

	if (toggle) {
		openFileDialogue();
	}


	handleFileDialogueResult();


}

void gbe::editor::ImageDropzone::openFileDialogue() {
	//ImGui::SeparatorText("In openFileDialogue");

	ImGuiFileDialog::Instance()->OpenDialog(
		/*key id   */ "ChoosePNG",
		/*title    */ "Select a PNG file",
		/*filters  */ ".png",
		/*start dir*/{ "C:/" }
	);
}

void gbe::editor::ImageDropzone::handleFileDialogueResult() {
	//ImGui::SeparatorText("In fileDialogueResult");

	ImVec2 view = ImGui::GetIO().DisplaySize;
	ImVec2 size = ImVec2(view.x * 0.90f, view.y * 0.90f);
	ImVec2 pos = ImVec2((view.x - size.x) * 0.5f, (view.y - size.y) * 0.5f);

	ImGui::SetNextWindowSize(size);
	ImGui::SetNextWindowPos(pos);

	if (ImGuiFileDialog::Instance()->Display(
		"ChoosePNG",
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
		

		if (ImGuiFileDialog::Instance()->IsOk()) {
			// 1. Path chosen by the user
			fs::path src = ImGuiFileDialog::Instance()->GetFilePathName();

			// 2. Ensure destination folder exists
			fs::create_directories(bgImageDir);

			// 3. Compose dest path (preserve filename)
			fs::path dest = bgImageDir / src.filename();
			try
			{
				fs::copy_file(src, dest,
					fs::copy_options::overwrite_existing);
				// TODO: engine‑side code to register / hot‑reload `dest`
				std::cout << "Image copied to: " << dest << '\n';
			}
			catch (const fs::filesystem_error& e)
			{
				std::cerr << "Copy failed: " << e.what() << '\n';
			}
		}
			ImGuiFileDialog::Instance()->Close();
	}
}


std::string gbe::editor::ImageDropzone::GetWindowId()
{
	return "ImageDropzone";
}
