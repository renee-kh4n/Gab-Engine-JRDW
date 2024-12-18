#include "gb_canvas.h"

gbe::gui::gb_canvas::gb_canvas(glm::ivec2 reference_resolution) : root(this)
{
	this->reference_resolution = reference_resolution;
}

gbe::gui::gb_rect* gbe::gui::gb_canvas::QueryAtPosition(glm::vec2 normalized_centered_position)
{
	return this->root.QueryAtPosition(normalized_centered_position);
}

void gbe::gui::gb_canvas::Draw(glm::ivec2 target_resolution)
{
	this->root.UpdateTransform();
	this->root.Render();
}

glm::ivec2 gbe::gui::gb_canvas::Get_reference_resolution()
{
	return this->reference_resolution;
}

void gbe::gui::gb_canvas::Update(double delta) {
	this->root.Update(delta);
}

void gbe::gui::gb_canvas::AddRootChild(gb_rect* other)
{
	other->SetParent(&this->root);
}
