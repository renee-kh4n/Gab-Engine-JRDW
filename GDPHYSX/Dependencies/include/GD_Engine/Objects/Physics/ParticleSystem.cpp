#include "ParticleSystem.h"
#include "../../PhysicsPipeline.h"

#include <iostream>

gde::ParticleSystem::ParticleSystem(std::function<RigidObject* (void)> particle_function)
{
	this->particle_function = particle_function;
}

void gde::ParticleSystem::Spawn()
{
	auto newparticle = this->particle_function();
	auto new_start_force = this->start_force.GetValue();
	newparticle->AddForce(new_start_force);



	std::cout << new_start_force.ToString() << std::endl;
	std::cout << newparticle->velocity.ToString() << std::endl;
}

void gde::ParticleSystem::InvokeUpdate(float deltatime)
{
	this->time_last_spawned += deltatime;

	if (this->time_last_spawned > 1.0f / this->spawns_per_sec) {
		this->time_last_spawned = 0;

		this->Spawn();
	}
}
