#include "Object.h"
#include <glm/gtx/matrix_decompose.hpp>

void gde::Object::MatToTrans(Transform* target, glm::mat4 mat)
{
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(mat, scale, rotation, translation, skew, perspective);

	target->position = translation;
	target->rotation = glm::eulerAngles(rotation) * 3.14159f / 180.f;
	target->scale = scale;
	target->Forward = Vector3(this->GetWorldSpaceMatrix()[2]);
	target->Up = Vector3(this->GetWorldSpaceMatrix()[1]);
	target->Right = Vector3(this->GetWorldSpaceMatrix()[0]);
}

gde::Object* gde::Object::Copy_self()
{
	return new Object(*this);
}

gde::Object::Object()
{
	this->transform = glm::mat4(1.0f);
	this->parent = nullptr;
}

gde::Object::~Object(){
	this->SetParent(nullptr);
}

gde::Object* gde::Object::Copy()
{
	auto copy = this->Copy_self();
	copy->children.clear();

	for (auto child : this->children)
	{
		copy->children.push_back(child->Copy());
	}

	return copy;
}

glm::mat4 gde::Object::GetWorldSpaceMatrix()
{
	glm::mat4 parentmat = glm::mat4(1.0f);

	if (this->parent != nullptr)
		parentmat = this->parent->GetWorldSpaceMatrix();

	return parentmat * this->transform;
}

gde::Transform* gde::Object::World()
{
	this->MatToTrans(&this->world, this->GetWorldSpaceMatrix());
	return &this->world;
}

gde::Transform* gde::Object::Local()
{
	this->MatToTrans(&this->local, this->transform);
	return &this->local;
}

void gde::Object::SetPosition(Vector3 vector)
{
	this->transform = glm::translate(this->transform, -(glm::vec3)this->Local()->position);
	this->transform = glm::translate(this->transform, (glm::vec3)vector);
}

void gde::Object::TranslateWorld(Vector3 vector)
{
	auto localdelta = glm::vec3(glm::vec4((glm::vec3)vector, 1) * this->GetWorldSpaceMatrix());
	this->transform = glm::translate(this->transform, (glm::vec3)localdelta);
}

void gde::Object::TranslateLocal(Vector3 vector)
{
	this->transform = glm::translate(this->transform, (glm::vec3)vector);
}

void gde::Object::Scale(Vector3 vector)
{
	this->transform = glm::scale(this->transform, (glm::vec3)vector);
}

void gde::Object::Rotate(Vector3 axis, float deg_angle)
{
	this->transform = glm::rotate(glm::mat4(1.0f), glm::radians(deg_angle), (glm::vec3)axis) * this->transform;
}

void gde::Object::Orient(Vector3 forward, Vector3 Up)
{
	this->transform[2] = glm::vec4((glm::vec3)forward, 0);
	this->transform[1] = glm::vec4((glm::vec3)Up, 0);
	this->transform[0] = glm::vec4((glm::vec3)Up.Cross(forward), 0);
}

void gde::Object::OnEnterHierarchy(Object* newChild)
{
	auto propagate_upwards = [this](Object* object) {
		Object* current = this->parent;

		while (current != nullptr)
		{
			current->OnEnterHierarchy(object);
			current = current->parent;
		}
	};

	for (auto subchild : newChild->children)
	{
		propagate_upwards(subchild);
	}

	propagate_upwards(newChild);
}

void gde::Object::OnExitHierarchy(Object* newChild)
{
	auto propagate_upwards = [this](Object* object) {
		Object* current = this->parent;

		while (current != nullptr)
		{
			current->OnExitHierarchy(object);
			current = current->parent;
		}
	};

	for (auto subchild : newChild->children)
	{
		propagate_upwards(subchild);
	}

	propagate_upwards(newChild);
}

void gde::Object::SetParent(Object* newParent)
{
	if (parent != nullptr) {
		parent->OnExitHierarchy(this);
		parent->children.remove_if([this](Object* child) {return child == this; });
	}

	if (newParent != nullptr) {
		newParent->OnEnterHierarchy(this);
		newParent->children.push_back(this);
	}

	this->parent = newParent;
}

gde::Object* gde::Object::GetChildAt(int i)
{
	auto start = this->children.begin();

	for (int count = 0; count < i; count++)
		++start;

	return *start;
}

int gde::Object::GetChildCount()
{
	return this->children.size();
}

void gde::Object::Destroy()
{
	this->isDestroyQueued = true;

	for (auto child : this->children) {
		child->Destroy();
	}
}

bool gde::Object::get_isDestroyed()
{
	return this->isDestroyQueued;
}
