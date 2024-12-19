#include "gb_canvas.h"

gbe::gui::gb_canvas::gb_canvas(glm::ivec2 basis_resolution) : root(this)
{
	this->basis_resolution = basis_resolution;
	this->reference_resolution = basis_resolution;
}

gbe::gui::gb_rect* gbe::gui::gb_canvas::QueryAtPosition(glm::vec2 normalized_centered_position)
{
	return this->root.QueryAtPosition(normalized_centered_position);
}

void gbe::gui::gb_canvas::Draw(glm::ivec2 target_resolution)
{
	auto aspect_ratio = (float)target_resolution.x / target_resolution.y;
	auto x_based_res = glm::ivec2(basis_resolution.x, basis_resolution.x / aspect_ratio);
	auto y_based_res = glm::ivec2(basis_resolution.x * aspect_ratio, basis_resolution.y);

	this->reference_resolution = glm::mix(x_based_res, y_based_res, this->xbasis_scale_factor);

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
