#include "GuiWindow.h"

void gbe::editor::GuiWindow::Draw() {
	if (!this->ext_Begin()) {
		this->ext_End();
		return;
	}

	this->DrawSelf();

	this->ext_End();
}

bool gbe::editor::GuiWindow::ext_Begin() {

	bool began = ImGui::Begin(this->GetWindowId().c_str());
	
	const float label_width_base = ImGui::GetFontSize() * 12;               // Some amount of width for label, based on font size.
	const float label_width_max = ImGui::GetContentRegionAvail().x * 0.40f; // ...but always leave some room for framed widgets.
	const float label_width = label_width_base < label_width_max ? label_width_base : label_width_max;
	ImGui::PushItemWidth(-label_width);

	if (!began) {
		return false;
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu(this->GetWindowId().c_str()))
		{
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	return true;
}

void gbe::editor::GuiWindow::ext_End() {
	ImGui::PopItemWidth();
	ImGui::End();
}