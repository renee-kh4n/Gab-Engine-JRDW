#include "ImageDropzone.h"

#include "../Utility/ModelExport.h"


#include <sstream>
#include <ostream>
#include <filesystem>


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
			std::filesystem::path src = ImGuiFileDialog::Instance()->GetFilePathName();

			// 2. Ensure destination folder exists
			std::filesystem::create_directories(bgImageDir);

			// 3. Compose dest path (preserve filename)
			std::filesystem::path dest = bgImageDir / src.filename();
			try
			{
				std::filesystem::copy_file(src, dest, std::filesystem::copy_options::overwrite_existing);
				// TODO: engine‑side code to register / hot‑reload `dest`
				std::cout << "Image copied to: " << dest << '\n';

				namespace fs = std::filesystem;

				fs::path assetRoot = fs::current_path();
				fs::create_directories(assetRoot);          // make the whole chain

				fs::path gbeFile = assetRoot / "input.img.gbe";
				std::ofstream out(gbeFile, std::ios::binary | std::ios::trunc);
				if (!out) throw std::runtime_error("Cannot open " + gbeFile.string());

				std::stringstream out_string;
				out_string << "{ \n"
					<< "\"asset_type\": \"texture\",\n"
					<< "\"asset_id\": \"input\",\n"
					<< "\"filename\": \"" << src.filename() << "\"\n"
					<< "}";
				out << out_string.str();                   
				// destructor flushes+closes
				out.close();
				std::cout << "Descriptor written to: " << fs::absolute(gbeFile) << '\n';

			}
			catch (const std::filesystem::filesystem_error& e)
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
