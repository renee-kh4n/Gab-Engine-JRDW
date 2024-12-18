
#include "gb_rect.h"
#include "gb_canvas.h"
#include "gbuiPipeline.h"
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

gbe::gui::gb_rect::gb_rect() {
    this->parent = nullptr;
    canvas = nullptr;

    bl_pivot = glm::vec2(-1, -1);
    tr_pivot = glm::vec2(1, 1);

    bl_offset = glm::vec2(0, 0);
    tr_offset = glm::vec2(0, 0);

    this->world_transform = glm::mat4(1.0f);
}

glm::mat4& gbe::gui::gb_rect::Get_world_transform()
{
    return this->world_transform;
}

gbe::gui::gb_rect::PointerEventHandleType gbe::gui::gb_rect::Get_handleType() {
    return this->pointerEventHandleType;
}

void gbe::gui::gb_rect::Set_handleType(gbe::gui::gb_rect::PointerEventHandleType value) {
    this->pointerEventHandleType = value;
}

gbe::gui::gb_rect* gbe::gui::gb_rect::QueryAtPosition(glm::vec2 normalized_centered_position) {
    auto this_relative_pos = glm::inverse(this->world_transform) * glm::vec4(normalized_centered_position, 0, 1.0f);
    auto inside = abs(this_relative_pos.x) < 1 && abs(this_relative_pos.y) < 1;

    gbe::gui::gb_rect* possible_query_result = nullptr;

    if (inside)
        possible_query_result = this;

    for (auto child : this->children)
    {
        auto possible_child_query_result = child->QueryAtPosition(normalized_centered_position);

        if (possible_child_query_result != nullptr) {
            possible_query_result = possible_child_query_result;
            break;
        }
    }

    return possible_query_result;
}

void gbe::gui::gb_rect::UpdateTransform() {
    auto parent_mat = glm::mat4(1.0f);

    if (this->parent != nullptr)
        parent_mat = this->parent->world_transform;

    glm::vec2 normalizer = this->canvas->Get_reference_resolution() / 2;
    normalizer = parent_mat * glm::vec4(normalizer, 1.0f, 1.0f);
    glm::vec3 final_bl = glm::vec3(bl_pivot.x, bl_pivot.y, 0) + glm::vec3(bl_offset.x / normalizer.x, bl_offset.y / normalizer.y, 0);
    glm::vec3 final_tr = glm::vec3(tr_pivot.x, tr_pivot.y, 0) + glm::vec3(tr_offset.x / normalizer.x, tr_offset.y / normalizer.y, 0);
    
    glm::vec3 rect_center = (final_tr + final_bl) * 0.5f;
    glm::vec3 rect_scale = (final_tr - final_bl) * 0.5f;
    rect_scale.z = 1.0f;

    glm::mat4 rect_transform = glm::mat4(1.0f);
    rect_transform = glm::translate(rect_transform, rect_center);
    rect_transform = glm::scale(rect_transform, rect_scale);

    this->world_transform = parent_mat * rect_transform;

    for (auto child : this->children)
    {
        child->UpdateTransform();
    }
}

void gbe::gui::gb_rect::Update(float delta) {
    for (auto child : this->children)
    {
        child->Update(delta);
    }
}
void gbe::gui::gb_rect::Render()
{
    if (this->canvas == nullptr)
        throw "Cannot be rendered if not part of a canvas.";
    
	for (auto rect : this->children)
	{
		rect->Render();
	}
}

void gbe::gui::gb_rect::SetParent(gbe::gui::gb_rect* other) {

    if (this->parent != nullptr) {
        parent->children.remove(this);
    }
    
    if (other != nullptr) {
        other->children.push_back(this);
        
        this->canvas = other->canvas;
    }

    if (other == nullptr) {
        this->canvas = nullptr;
    }

    this->parent = other;
}