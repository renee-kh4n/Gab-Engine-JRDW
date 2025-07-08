#include "ColorpickerWindow.h"

void gbe::editor::ColorpickerWindow::DrawSelf()
{
	ImGui::ColorPicker3("Colorpicker", this->rgb);
}

std::string gbe::editor::ColorpickerWindow::GetWindowId()
{
	return "Color Picker";
}
