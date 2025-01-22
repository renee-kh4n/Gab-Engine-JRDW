#include "gbuiPipeline.h"
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

gbe::gui::gbuiPipeline* gbe::gui::gbuiPipeline::instance = nullptr;

gbe::gui::gbuiPipeline::gbuiPipeline(unsigned int quad_vao, asset::Shader* new_gui_shader)
{
	this->active_canvas = nullptr;
	this->current_object_on_cursor = nullptr;

	this->quad_vao = quad_vao;
	this->gui_shader = new_gui_shader;

	this->instance = this;
}

gbe::gui::gbuiPipeline* gbe::gui::gbuiPipeline::Get_current()
{
	return gbe::gui::gbuiPipeline::instance;
}

void gbe::gui::gbuiPipeline::Set_target_resolution(glm::ivec2 res) {
	this->target_resolution = res;
}
void gbe::gui::gbuiPipeline::PassScreenSpaceMousePos(glm::ivec2 mousepos)
{
	this->mousepos_screen_centered = mousepos - (this->target_resolution / 2);

	if (this->active_canvas == nullptr)
		return;

	auto normalized_centered_mousepos = this->mousepos_screen_centered / (glm::vec2)(this->target_resolution / 2);

	auto on_pointer = this->active_canvas->QueryAtPosition(normalized_centered_mousepos);

	if (on_pointer != this->current_object_on_cursor) {
		//trigger enter
		if (on_pointer != nullptr) {
			auto enter_comp = dynamic_cast<gbe::gui::gb_onEnter*>(on_pointer);

			if (enter_comp != nullptr)
				enter_comp->onEnter();
		}
		
		//trigger exit
		if (this->current_object_on_cursor != nullptr) {
			auto enter_comp = dynamic_cast<gbe::gui::gb_onExit*>(this->current_object_on_cursor);

			if (enter_comp != nullptr)
				enter_comp->onExit();
		}

		this->current_object_on_cursor = on_pointer;
	}
}

unsigned int gbe::gui::gbuiPipeline::Get_quad_vao() {
	return this->quad_vao;
}
gbe::asset::Shader* gbe::gui::gbuiPipeline::Get_gui_shader()
{
	return this->gui_shader;
}
void gbe::gui::gbuiPipeline::SetActiveCanvas(gb_canvas* canvas)
{
	this->active_canvas = canvas;
}

void gbe::gui::gbuiPipeline::DrawActiveCanvas()
{
	this->active_canvas->Draw(this->target_resolution);
}

void gbe::gui::gbuiPipeline::DrawElement(gbe::gui::gb_rect* element)
{
	this->gui_shader->SetOverride("transform", (gbe::Matrix4)element->Get_world_transform());

	glBindVertexArray(this->quad_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void gbe::gui::gbuiPipeline::Update(double delta) {
	if (this->active_canvas == nullptr)
		return;

	this->active_canvas->Update(delta);
}

void gbe::gui::gbuiPipeline::Click()
{
	if (this->current_object_on_cursor == nullptr)
		return;

	auto cast = dynamic_cast<gb_onClick*>(this->current_object_on_cursor);

	if (cast == nullptr)
		return;

	cast->onClick();
}
