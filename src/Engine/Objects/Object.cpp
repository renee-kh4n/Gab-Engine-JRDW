#include "Object.h"

#define _USE_MATH_DEFINES
#include <math.h>

void gbe::Object::MatToTrans(Transform* target, Matrix4 mat)
{
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(mat, scale, rotation, translation, skew, perspective);

	target->position.Set(translation);
	target->rotation.Set(rotation);
	target->scale.Set(scale);
}

gbe::Object* gbe::Object::Copy_self()
{
	return new Object(*this);
}

void gbe::Object::OnLocalTransformationChange(TransformChangeType changetype)
{
	auto worldmat = this->parent_matrix * this->local.GetMatrix();

	for (auto child : this->children)
	{
		child->OnExternalTransformationChange(changetype, worldmat);
	}

	MatToTrans(&this->world, worldmat);

	if (isnan(this->local.GetMatrix()[0][0]))
		throw "NAN transform";
}

void gbe::Object::OnExternalTransformationChange(TransformChangeType changetype, Matrix4 newparentmatrix)
{
	this->parent_matrix = newparentmatrix;
	auto worldmat = this->parent_matrix * this->local.GetMatrix();

	for (auto child : this->children)
	{
		child->OnExternalTransformationChange(changetype, worldmat);
	}

	MatToTrans(&this->world, worldmat);
}

gbe::Object::Object()
{
	this->parent_matrix = Matrix4(1.0f);
	this->parent = nullptr;

	OnLocalTransformationChange(TransformChangeType::ALL);
	OnExternalTransformationChange(TransformChangeType::ALL, this->parent_matrix);
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

gbe::Matrix4 gbe::Object::GetWorldMatrix(bool include_local_scale)
{
	return this->parent_matrix * this->Local().GetMatrix(include_local_scale);
}

void gbe::Object::SetLocalMatrix(Matrix4 mat)
{
	MatToTrans(&this->local, mat);

	OnLocalTransformationChange(TransformChangeType::ALL);
}

void gbe::Object::SetWorldPosition(Vector3 vector)
{
	auto world_pos = this->World().position.Get();

	this->TranslateWorld(-world_pos);
	this->TranslateWorld(vector);
}

void gbe::Object::TranslateWorld(Vector3 vector)
{
	auto curmat = this->local.GetMatrix();

	auto curloc = glm::vec3(curmat[3]);
	curloc += (glm::vec3)vector;
	auto newrow4 = glm::vec4(curloc, curmat[3][3]);

	curmat[3] = newrow4;

	MatToTrans(&this->local, curmat);

	OnLocalTransformationChange(TransformChangeType::TRANSLATION);
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

	newChild->CallRecursively([propagate_upwards](Object* child) {
		propagate_upwards(child);
	});
}

void gbe::Object::OnExitHierarchy(Object* newChild)
{
	auto propagate_upwards = [this](Object* message) {
		Object* current = this->parent;

		while (current != nullptr)
		{
			current->OnExitHierarchy(message);
			current = current->parent;
		}
	};

	newChild->CallRecursively([propagate_upwards](Object* child) {
		propagate_upwards(child);
	});
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

		this->parent_matrix = Matrix4(1.0f);
	}

	if (newParent != nullptr) {
		newParent->OnEnterHierarchy(this);
		newParent->children.push_back(this);

		this->parent_matrix = newParent->GetWorldMatrix();
	}

	this->parent = newParent;

	OnLocalTransformationChange(TransformChangeType::ALL);
	OnExternalTransformationChange(TransformChangeType::ALL, this->parent_matrix);
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
