#include "gb_button.h"
#include "../gbuiPipeline.h"
#include <glm/gtc/type_ptr.hpp>

void gbe::gui::gb_button::Render() {
	glUseProgram(gbe::gui::gbuiPipeline::Get_current()->Get_gui_shader_id());
	auto loc = glGetUniformLocation(gbe::gui::gbuiPipeline::Get_current()->Get_gui_shader_id(), "color");
	glUniform4fv(loc, 1, glm::value_ptr(this->current_color));
	gbe::gui::gbuiPipeline::Get_current()->DrawElement(this);

	gbe::gui::gb_rect::Render();
}

void gbe::gui::gb_button::Update(float delta)
{
	gbe::gui::gb_rect::Update(delta);

	auto target_color = this->normal_color;

	if (this->hovered) {
		target_color = this->hovered_color;
	}

	this->current_color = glm::mix(this->current_color, target_color, delta * this->transition_speed);
}

void gbe::gui::gb_button::onEnter() {
	this->hovered = true;
}
void gbe::gui::gb_button::onExit() {
	this->hovered = false;
}
void gbe::gui::gb_button::onClick() {
	this->current_color = this->pressed_color;

	if(this->onClickAction)
		this->onClickAction();

	//more stuff here
}

void gbe::gui::gb_button::Set_onClickAction(std::function<void()> new_action) {
	this->onClickAction = new_action;
}