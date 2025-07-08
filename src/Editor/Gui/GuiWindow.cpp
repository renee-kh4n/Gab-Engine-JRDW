#include "GuiWindow.h"

void gbe::editor::GuiWindow::Draw() {
	this->ext_Begin();

	this->DrawSelf();

	this->ext_End();
}

void gbe::editor::GuiWindow::ext_Begin() {

	if (!ImGui::Begin(this->GetWindowId().c_str())) {
		ImGui::End();

		return;
	}
	
	const float label_width_base = ImGui::GetFontSize() * 12;               // Some amount of width for label, based on font size.
	const float label_width_max = ImGui::GetContentRegionAvail().x * 0.40f; // ...but always leave some room for framed widgets.
	const float label_width = label_width_base < label_width_max ? label_width_base : label_width_max;
	ImGui::PushItemWidth(-label_width);                                   // Right-align: framed items will leave 'label_width' available for the label.

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu(this->GetWindowId().c_str()))
		{
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

void gbe::editor::GuiWindow::ext_End() {
	ImGui::PopItemWidth();
	ImGui::End();
}