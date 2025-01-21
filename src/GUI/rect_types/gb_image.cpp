#include "gb_image.h"

#include "../gbuiPipeline.h"
#include <glm/gtc/type_ptr.hpp>

void gbe::gui::gb_image::Set_Image(asset::Texture* tex)
{
	this->sprite = tex;
}

void gbe::gui::gb_image::Render()
{
	glUseProgram(gbe::gui::gbuiPipeline::Get_current()->Get_gui_shader_id());
	auto loc = glGetUniformLocation(gbe::gui::gbuiPipeline::Get_current()->Get_gui_shader_id(), "color");
	glUniform4fv(loc, 1, glm::value_ptr(this->current_color));
	gbe::gui::gbuiPipeline::Get_current()->DrawElement(this);

	gbe::gui::gb_rect::Render();
}
