#include "gbuiPipeline.h"

gbe::gui::gbuiPipeline::gbuiPipeline(unsigned int quad_vao, unsigned int gui_shader_id)
{
	this->quad_vao = quad_vao;
	this->gui_shader_id = gui_shader_id;
}

void gbe::gui::gbuiPipeline::PassScreenSpaceMousePos(glm::vec2 mousepos)
{
	this->mousepos_screen = mousepos;
}

void gbe::gui::gbuiPipeline::SetActiveCanvas(gb_canvas* canvas)
{
	this->active_canvas = canvas;
}

void gbe::gui::gbuiPipeline::Draw(glm::ivec2 target_resolution)
{
	this->active_canvas->DrawToCurrentBuffer(target_resolution);
}

void gbe::gui::gbuiPipeline::Click()
{
	
}
