#include "CollisionContact.h"

void gbe::CollisionContact::Resolve(float time)
{
	this->ResolveVelocity(time);
	this->ResolveInterpenetration(time);
}

float gbe::CollisionContact::GetSeperatingSpeed()
{
	Vector3 velocity = this->objects[0]->velocity;
	if (this->objects[1]) velocity -= this->objects[1]->velocity;
	return velocity.Dot(contactNormal);
}

void gbe::CollisionContact::ResolveVelocity(float time)
{
	float separatingSpeed = GetSeperatingSpeed();

	if (separatingSpeed > 0)
		return;

	float newSS = -restitution * separatingSpeed;
	float deltaSpeed = newSS - separatingSpeed;

	float totalMass = 1.0f / this->objects[0]->mass;
	if (this->objects[1]) totalMass += 1.0f / this->objects[1]->mass;

	if (totalMass <= 0) return;

	float impule_mag = deltaSpeed / totalMass;
	Vector3 impulse = contactNormal * impule_mag;
	Vector3 V_a = impulse * (1.0f / this->objects[0]->mass);
	this->objects[0]->velocity = this->objects[0]->velocity + V_a;

	if (this->objects[1]) {
		Vector3 V_b = impulse * (-1.0f / this->objects[1]->mass);
		this->objects[1]->velocity = this->objects[1]->velocity + V_b;
	}
}

void gbe::CollisionContact::ResolveInterpenetration(float time)
{
	if (depth > 0) return;

	float totalMass = 1.0f / this->objects[0]->mass;
	if (this->objects[1]) totalMass += 1.0f / this->objects[1]->mass;

	if (totalMass <= 0) return;

	float moveForce	= depth / totalMass;
	Vector3 impulse = contactNormal * moveForce;

	Vector3 V_a = impulse * (1.0f / this->objects[0]->mass);
	this->objects[0]->TranslateWorld(V_a);

	if (this->objects[1]) {
		Vector3 V_b = impulse * (-1.0f / this->objects[1]->mass);
		this->objects[0]->TranslateWorld(V_b);
	}

	depth = 0;
}
