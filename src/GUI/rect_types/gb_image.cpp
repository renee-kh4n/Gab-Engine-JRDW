#include "gb_image.h"

#include "../gbuiPipeline.h"
#include <glm/gtc/type_ptr.hpp>

void gbe::gui::gb_image::Set_Image(asset::Texture* tex)
{
	this->sprite = tex;
	this->current_color = Vector4(1, 1, 1, 1);
}

void gbe::gui::gb_image::Render()
{
	gbuiPipeline::Get_current()->Get_gui_shader()->SetOverride("has_sprite", true);

	gbuiPipeline::Get_current()->Get_gui_shader()->SetOverride("color", (Vector4)this->current_color);
	gbuiPipeline::Get_current()->Get_gui_shader()->SetTextureOverride("sprite", this->sprite);

	gbe::gui::gbuiPipeline::Get_current()->DrawElement(this);
	gbuiPipeline::Get_current()->Get_gui_shader()->SetOverride("has_sprite", false);

	gbe::gui::gb_rect::Render();
}
