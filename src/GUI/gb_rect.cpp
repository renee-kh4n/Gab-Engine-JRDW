
#include "gb_rect.h"
#include "gb_canvas.h"
#include <glm/ext/matrix_transform.hpp>

void gbe::gui::gb_rect::Render(glm::mat4& parent_transform)
{
    glm::vec2 bl_pivot = glm::vec2(-1, -1);
    glm::vec2 tr_pivot = glm::vec2(-1, -1);

    glm::vec2 bl_offset = glm::vec2(50, 50);
    glm::vec2 tr_offset = glm::vec2(200, 100);

    glm::vec2 normalizer = this->belongs_to->Get_reference_resolution() / 2;
    glm::vec3 final_bl = glm::vec3(bl_pivot.x, bl_pivot.y, 0) + glm::vec3(bl_offset.x / normalizer.x, bl_offset.y / normalizer.y, 0);
    glm::vec3 final_tr = glm::vec3(tr_pivot.x, tr_pivot.y, 0) + glm::vec3(tr_offset.x / normalizer.x, tr_offset.y / normalizer.y, 0);
    glm::vec3 rect_center = (final_tr + final_bl) * 0.5f;
    glm::vec3 rect_scale = (final_tr - final_bl) * 0.5f;

    glm::mat4 rect_transform = glm::mat4(1.0f);
    rect_transform = glm::translate(rect_transform, rect_center);
    rect_transform = glm::scale(rect_transform, rect_scale);

    rect_transform = parent_transform * rect_transform;



	for (auto rect : this->children)
	{
		rect->Render(rect_transform);
	}
}