#include "gb_canvas.h"

void gbe::gui::gb_canvas::DrawToCurrentBuffer(glm::ivec2 target_resolution)
{
	for (auto child : this->children)
	{
		child->Render(glm::mat4(1.0f));
	}
}

glm::ivec2 gbe::gui::gb_canvas::Get_reference_resolution()
{
	return this->reference_resolution;
}

std::list<gbe::gui::gb_rect*>& gbe::gui::gb_canvas::Get_children()
{
	return this->children;
}
