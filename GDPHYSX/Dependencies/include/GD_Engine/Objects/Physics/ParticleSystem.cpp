#include "ParticleSystem.h"
#include "../../PhysicsPipeline.h"

gde::ParticleSystem::ParticleSystem(RigidObject particle)
{
	this->particle = particle;
}

void gde::ParticleSystem::Spawn()
{
	auto newparticle = new RigidObject(this->particle);
	PhysicsPipeline::main->Register(newparticle);

	newparticle->AddForce(this->start_force.GetValue());
}

void gde::ParticleSystem::InvokeUpdate(float deltatime)
{
	this->time_last_spawned += deltatime;

	if (this->time_last_spawned > 1.0f / this->spawns_per_sec) {
		this->time_last_spawned = 0;

		this->Spawn();
	}
}
