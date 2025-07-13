#include "CreditsWindow.h"

gbe::editor::CreditsWindow::CreditsWindow()
{
	this->logo_tex = new gbe::asset::Texture("DefaultAssets/Tex/UI/logo.img.gbe");
	TextureData& logo_tex_data = TextureLoader::GetAssetData(this->logo_tex);
	this->logo_tex_DS = logo_tex_data.DS;
}

void gbe::editor::CreditsWindow::DrawSelf() {
	ImGui::Image(this->logo_tex_DS, { 128, 128 }, { 0, 1 }, { 1, 0 });
	ImGui::Text("About");
	ImGui::Text("GabEngine v0.1");
	ImGui::Text("Developed by Gabriel Rayo");
	ImGui::Text("Acknowledgements:");
	ImGui::Text("De La Salle University");
	ImGui::Text("GDGRAP");
	ImGui::Text("GDPHYSX");
	ImGui::Text("GDENG03");
}

std::string gbe::editor::CreditsWindow::GetWindowId()
{
	return "Credits";
}