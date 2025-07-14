#include "ImageDropzone.h"

#include "../Utility/ModelExport.h"

void gbe::editor::ImageDropzone::DrawSelf() {
	
		ImGui::Button("Upload Image (supported file type: PNG)");


}

std::string gbe::editor::ImageDropzone::GetWindowId()
{
	return "ImageDropzone";
}
