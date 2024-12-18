#include "gb_root.h"
#include "gb_root.h"
#include "gb_root.h"

gbe::gui::gb_root::gb_root(gb_canvas* canvas)
{
	this->canvas = canvas;
}

void gbe::gui::gb_root::Render()
{
	gbe::gui::gb_rect::Render();
}