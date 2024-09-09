#include "Object.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

void gde::Object::UpdateTransform()
{
	auto newworld_matrix = this->parent_matrix * this->local_matrix;

	for (auto child : this->children)
	{
		child->parent_matrix = newworld_matrix;
		child->UpdateTransform();
	}

	MatToTrans(&this->world, newworld_matrix);
	MatToTrans(&this->local, this->local_matrix);
}

void gde::Object::MatToTrans(Transform* target, glm::mat4 mat)
{
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(mat, scale, rotation, translation, skew, perspective);

	target->position = translation;
	target->rotation = glm::eulerAngles(rotation) * (180.0f / (float)M_PI);
	target->scale = scale;
	target->Forward = Vector3(mat[2]);
	target->Up = Vector3(mat[1]);
	target->Right = Vector3(mat[0]);
}

gde::Object* gde::Object::Copy_self()
{
	return new Object(*this);
}

gde::Object::Object()
{
	this->parent_matrix = glm::mat4(1.0f);
	this->local_matrix = glm::mat4(1.0f);
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

gde::Transform* gde::Object::World()
{
	return &this->world;
}

gde::Transform* gde::Object::Local()
{
	return &this->local;
}

void gde::Object::SetPosition(Vector3 vector)
{
	auto world_pos = this->World()->position;

	this->TranslateWorld(-world_pos);
	this->TranslateWorld(vector);
}

void gde::Object::TranslateWorld(Vector3 vector)
{
	auto curloc = glm::vec3(this->local_matrix[3]);
	curloc += (glm::vec3)vector;
	auto newrow4 = glm::vec4(curloc, this->local_matrix[3][3]);

	this->local_matrix[3] = newrow4;

	UpdateTransform();
}

void gde::Object::TranslateLocal(Vector3 vector)
{
	this->local_matrix = glm::translate(this->local_matrix, (glm::vec3)vector);

	UpdateTransform();
}

void gde::Object::Scale(Vector3 vector)
{
	this->local_matrix = glm::scale(this->local_matrix, (glm::vec3)vector);

	UpdateTransform();
}

void gde::Object::SetScale(Vector3 vector)
{
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(this->local_matrix, scale, rotation, translation, skew, perspective);

	auto newmat = glm::mat4(1.0f);
	newmat = glm::translate(newmat, translation);
	newmat *= glm::toMat4(rotation);
	newmat = glm::scale(newmat, (glm::vec3)vector);

	this->local_matrix = newmat;

	UpdateTransform();
}

void gde::Object::Rotate(Vector3 axis, float deg_angle)
{
	this->local_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(deg_angle), (glm::vec3)axis) * this->local_matrix;
	UpdateTransform();
}

void gde::Object::SetRotation(Vector3 euler)
{
	auto newmat = glm::mat4(1.0f);
	newmat = glm::translate(newmat, (glm::vec3)Local()->position);
	newmat = glm::rotate(newmat, glm::radians(euler.y), glm::vec3(0, 1, 0));
	newmat = glm::rotate(newmat, glm::radians(euler.x), glm::vec3(1, 0, 0));
	newmat = glm::rotate(newmat, glm::radians(euler.z), glm::vec3(0, 0, 1));
	newmat = glm::scale(newmat,(glm::vec3)Local()->scale);

	this->local_matrix = newmat;

	UpdateTransform();
}

void gde::Object::Orient(Vector3 forward, Vector3 Up)
{
	if (abs(forward.SqrMagnitude() - 1.0f) > 0.001f || abs(Up.SqrMagnitude() - 1.0f) > 0.001f) {
		return;
	}

	auto lookatmat = glm::inverse(glm::lookAt((glm::vec3)Vector3::zero, (glm::vec3)forward, (glm::vec3)Up));

	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(this->local_matrix, scale, rotation, translation, skew, perspective);

	auto newmat = glm::mat4(1.0f);
	newmat = glm::translate(newmat, translation);
	newmat *= lookatmat;
	newmat = glm::scale(newmat, scale);

	this->local_matrix = newmat;

	UpdateTransform();
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

	propagate_upwards(newChild);
	
	UpdateTransform();
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

gde::Object* gde::Object::GetParent()
{
	return this->parent;
}

void gde::Object::SetParent(Object* newParent)
{
	if (parent != nullptr) {
		parent->OnExitHierarchy(this);
		parent->children.remove_if([this](Object* child) {return child == this; });
	}

	if (newParent != nullptr) {
		this->CallRecursively([newParent](Object* child) {newParent->OnEnterHierarchy(child); });
		newParent->children.push_back(this);
	}

	this->parent = newParent;
}

gde::Object* gde::Object::GetChildAt(size_t i)
{
	auto start = this->children.begin();

	for (int count = 0; count < i; count++)
		++start;

	return *start;
}

size_t gde::Object::GetChildCount()
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

void gde::Object::CallRecursively(std::function<void(Object*)> action)
{
	size_t childcount = GetChildCount();
	for (size_t i = 0; i < childcount; i++)
	{
		this->GetChildAt(i)->CallRecursively(action);
	}

	action(this);
}
