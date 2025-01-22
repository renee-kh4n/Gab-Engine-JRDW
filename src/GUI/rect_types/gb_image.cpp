#include "gb_image.h"

#include "../gbuiPipeline.h"
#include <glm/gtc/type_ptr.hpp>

void gbe::gui::gb_image::Set_Image(asset::Texture* tex)
{
	this->sprite = tex;
}

void gbe::gui::gb_image::Render()
{
	gbuiPipeline::Get_current()->Get_gui_shader()->SetOverride("color", this->current_color);
	gbuiPipeline::Get_current()->Get_gui_shader()->SetTextureOverride("sprite", this->sprite);

	gbe::gui::gbuiPipeline::Get_current()->DrawElement(this);

	gbe::gui::gb_rect::Render();
}
