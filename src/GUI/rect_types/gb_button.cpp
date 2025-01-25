#include "gb_button.h"
#include "../gbuiPipeline.h"
#include <glm/gtc/type_ptr.hpp>

gbe::gui::gb_button::gb_button(gb_image* ntarget_graphic) {
	this->target_graphic = ntarget_graphic;
	this->Set_handleType(PointerEventHandleType::BLOCK);
}

void gbe::gui::gb_button::Render() {
}

void gbe::gui::gb_button::Update(float delta)
{
	gbe::gui::gb_rect::Update(delta);

	auto target_color = this->normal_color;

	if (this->hovered) {
		target_color = this->hovered_color;
	}

	this->current_color = glm::mix((glm::vec4)this->current_color, (glm::vec4)target_color, delta * this->transition_speed);
	this->target_graphic->Set_tint(this->current_color);
}

void gbe::gui::gb_button::onEnter() {
	this->hovered = true;

	if (this->onHoverAction)
		this->onHoverAction();
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

void gbe::gui::gb_button::Set_onHoverAction(std::function<void()> new_action) {
	this->onHoverAction = new_action;
}