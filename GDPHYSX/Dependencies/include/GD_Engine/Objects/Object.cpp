#include "Object.h"
#include <glm/gtx/matrix_decompose.hpp>

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
	auto trans_mat = this->GetWorldSpaceMatrix();

	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(trans_mat, scale, rotation, translation, skew, perspective);

	this->world.position = translation;
	this->world.rotation = glm::eulerAngles(rotation) * 3.14159f / 180.f;
	this->world.scale = scale;

	return &this->world;
}

gde::Transform* gde::Object::Local()
{
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(this->transform, scale, rotation, translation, skew, perspective);

	this->local.position = translation;
	this->local.rotation = glm::eulerAngles(rotation) * 3.14159f / 180.f;
	this->local.scale = scale;

	return &this->local;
}

gde::Vector3 gde::Object::Forward()
{
	return Vector3(this->GetWorldSpaceMatrix()[2]);
}

void gde::Object::Translate(Vector3 vector)
{
	this->transform = glm::translate(this->transform, (glm::vec3)vector);
}

void gde::Object::Scale(Vector3 vector)
{
	this->transform = glm::scale(this->transform, (glm::vec3)vector);
}

void gde::Object::Rotate(Vector3 vector)
{
	this->transform = glm::rotate(this->transform, glm::radians(vector.y), glm::vec3(0, 1, 0));
	this->transform = glm::rotate(this->transform, glm::radians(vector.x), glm::vec3(1, 0, 0));
	this->transform = glm::rotate(this->transform, glm::radians(vector.z), glm::vec3(0, 0, 1));
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
