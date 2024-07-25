#include "Object.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

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
	target->Forward = Vector3(this->GetWorldSpaceMatrix()[2]);
	target->Up = Vector3(this->GetWorldSpaceMatrix()[1]);
	target->Right = Vector3(this->GetWorldSpaceMatrix()[0]);
}

glm::mat4 gde::Object::TransToMat(Transform* trans)
{
	auto newmat = glm::mat4(1.0f);
	newmat = glm::scale(newmat, (glm::vec3)trans->scale);
	//newmat *= ;
	newmat = glm::translate(newmat, (glm::vec3)trans->position);
}

gde::Object* gde::Object::Copy_self()
{
	return new Object(*this);
}

gde::Object::Object()
{
	this->local_transform = glm::mat4(1.0f);
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

	return parentmat * this->local_transform;
}

gde::Transform* gde::Object::World()
{
	this->MatToTrans(&this->world, this->GetWorldSpaceMatrix());
	return &this->world;
}

gde::Transform* gde::Object::Local()
{
	this->MatToTrans(&this->local, this->local_transform);
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
	/*OLD
	auto localdelta = glm::vec3(glm::vec4((glm::vec3)vector, 1) * this->GetWorldSpaceMatrix());
	this->local_transform = glm::translate(this->local_transform, (glm::vec3)localdelta);
	*/

	auto curloc = glm::vec3(this->local_transform[3]);
	curloc += (glm::vec3)vector;
	auto newrow4 = glm::vec4(curloc, this->local_transform[3][3]);

	this->local_transform[3] = newrow4;
}

void gde::Object::TranslateLocal(Vector3 vector)
{
	this->local_transform = glm::translate(this->local_transform, (glm::vec3)vector);
}

void gde::Object::Scale(Vector3 vector)
{
	this->local_transform = glm::scale(this->local_transform, (glm::vec3)vector);
}

void gde::Object::SetScale(Vector3 vector)
{
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(this->local_transform, scale, rotation, translation, skew, perspective);

	auto newmat = glm::mat4(1.0f);
	newmat = glm::translate(newmat, translation);
	newmat *= glm::toMat4(rotation);
	newmat = glm::scale(newmat, (glm::vec3)vector);

	this->local_transform = newmat;
}

void gde::Object::Rotate(Vector3 axis, float deg_angle)
{
	this->local_transform = glm::rotate(glm::mat4(1.0f), glm::radians(deg_angle), (glm::vec3)axis) * this->local_transform;
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
	glm::decompose(this->local_transform, scale, rotation, translation, skew, perspective);

	auto newmat = glm::mat4(1.0f);
	newmat = glm::translate(newmat, translation);
	newmat *= lookatmat;
	newmat = glm::scale(newmat, scale);

	this->local_transform = newmat;
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
