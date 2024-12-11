#include "Transform.h"

gbe::Transform::Transform()
{
	this->Right.Set(Vector3(1, 0, 0));
	this->Up.Set(Vector3(0, 1, 0));
	this->Forward.Set(Vector3(0, 0, 1));
	this->position.Set(Vector3::zero);
	this->scale.Set(Vector3(1.0f));
	this->rotation.Set(Quaternion());
}

gbe::Matrix4 gbe::Transform::GetMatrix(bool include_scale)
{
	if(include_scale)
		return this->updated_matrix_with_scale;
	else
		return this->updated_matrix_without_scale;
}

void gbe::Transform::OnComponentChange()
{
	if (this->onChange) 
		this->onChange();

	auto newmat = Matrix4();
	newmat = glm::translate(newmat, this->position.Get());
	newmat *= glm::toMat4(this->rotation.Get());

	this->updated_matrix_without_scale = newmat;

	newmat = glm::scale(newmat, this->scale.Get());

	this->updated_matrix_with_scale = newmat;
}

void gbe::Transform::UpdateAxisVectors()
{
	auto newbasismat = glm::toMat4(this->rotation.Get());
	this->Right.Set((Vector3)newbasismat[0]);
	this->Up.Set((Vector3)newbasismat[1]);
	this->Forward.Set((Vector3)newbasismat[2]);
}

gbe::Transform::Transform(std::function<void()> func) : gbe::Transform::Transform() {
	this->onChange = func;
}