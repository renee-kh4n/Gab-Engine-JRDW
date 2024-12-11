#include "Object.h"

#define _USE_MATH_DEFINES
#include <math.h>

void gbe::Object::UpdateTransform()
{
	auto worldmat = this->parent_matrix * this->local_matrix;

	for (auto child : this->children)
	{
		child->parent_matrix = worldmat;
		child->UpdateTransform();
	}

	MatToTrans(&this->world, worldmat);
	MatToTrans(&this->local, this->local_matrix);

	OnChangeMatrix();

	if (isnan(this->local_matrix[0][0]))
		throw "NAN transform";
}

void gbe::Object::MatToTrans(Transform* target, Matrix4 mat)
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

gbe::Object* gbe::Object::Copy_self()
{
	return new Object(*this);
}

void gbe::Object::OnChangeMatrix()
{
}

gbe::Object::Object()
{
	this->parent_matrix = Matrix4(1.0f);
	this->local_matrix = Matrix4(1.0f);
	this->parent = nullptr;
}

gbe::Object::~Object(){
	this->SetParent(nullptr);
}

gbe::Object* gbe::Object::Copy()
{
	auto copy = this->Copy_self();
	copy->children.clear();

	for (auto child : this->children)
	{
		copy->children.push_back(child->Copy());
	}

	return copy;
}

gbe::Transform& gbe::Object::World()
{
	return this->world;
}

gbe::Transform& gbe::Object::Local()
{
	return this->local;
}

gbe::Matrix4 gbe::Object::GetLocalMatrix()
{
	return this->local_matrix;
}

gbe::Matrix4 gbe::Object::GetWorldMatrix()
{
	return this->parent_matrix * this->local_matrix;
}

void gbe::Object::SetMatrix(Matrix4 mat)
{
	this->local_matrix = mat;

	UpdateTransform();
}

void gbe::Object::SetPosition(Vector3 vector)
{
	auto world_pos = this->World().position;

	this->TranslateWorld(-world_pos);
	this->TranslateWorld(vector);
}

void gbe::Object::TranslateWorld(Vector3 vector)
{
	auto curloc = glm::vec3(this->local_matrix[3]);
	curloc += (glm::vec3)vector;
	auto newrow4 = glm::vec4(curloc, this->local_matrix[3][3]);

	this->local_matrix[3] = newrow4;

	UpdateTransform();
}

void gbe::Object::TranslateLocal(Vector3 vector)
{
	this->local_matrix = glm::translate(this->local_matrix, (glm::vec3)vector);

	UpdateTransform();
}

void gbe::Object::Scale(Vector3 vector)
{
	this->local_matrix = glm::scale(this->local_matrix, (glm::vec3)vector);

	UpdateTransform();
}

void gbe::Object::SetScale(Vector3 vector)
{
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(this->local_matrix, scale, rotation, translation, skew, perspective);

	auto newmat = Matrix4();
	newmat = glm::translate(newmat, translation);
	newmat *= glm::toMat4(rotation);
	newmat = glm::scale(newmat, (glm::vec3)vector);

	this->local_matrix = newmat;

	UpdateTransform();
}

void gbe::Object::Rotate(Vector3 axis, float deg_angle)
{
	this->local_matrix = glm::rotate(Matrix4(), glm::radians(deg_angle), (glm::vec3)axis) * this->local_matrix;
	UpdateTransform();
}

void gbe::Object::SetRotation(Vector3 euler)
{
	auto newmat = Matrix4();
	newmat = glm::translate(newmat, (glm::vec3)Local().position);
	newmat = glm::rotate(newmat, glm::radians(euler.y), Vector3(0, 1, 0));
	newmat = glm::rotate(newmat, glm::radians(euler.x), Vector3(1, 0, 0));
	newmat = glm::rotate(newmat, glm::radians(euler.z), Vector3(0, 0, 1));
	newmat = glm::scale(newmat, Local().scale);

	this->local_matrix = newmat;

	UpdateTransform();
}

void gbe::Object::Orient(Vector3 forward, Vector3 Up)
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

	auto newmat = Matrix4();
	newmat = glm::translate(newmat, translation);
	newmat *= lookatmat;
	newmat = glm::scale(newmat, scale);

	this->local_matrix = newmat;

	UpdateTransform();
}

void gbe::Object::OnEnterHierarchy(Object* newChild)
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

void gbe::Object::OnExitHierarchy(Object* newChild)
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

gbe::Object* gbe::Object::GetParent()
{
	return this->parent;
}

void gbe::Object::SetParent(Object* newParent)
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

gbe::Object* gbe::Object::GetChildAt(size_t i)
{
	auto start = this->children.begin();

	for (int count = 0; count < i; count++)
		++start;

	return *start;
}

size_t gbe::Object::GetChildCount()
{
	return this->children.size();
}

void gbe::Object::Destroy()
{
	this->isDestroyQueued = true;

	for (auto child : this->children) {
		child->Destroy();
	}
}

bool gbe::Object::get_isDestroyed()
{
	return this->isDestroyQueued;
}

void gbe::Object::CallRecursively(std::function<void(Object*)> action)
{
	size_t childcount = GetChildCount();
	for (size_t i = 0; i < childcount; i++)
	{
		this->GetChildAt(i)->CallRecursively(action);
	}

	action(this);
}
