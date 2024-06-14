#include "ParticleSystem.h"

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

	this->particles.insert_or_assign(newparticle, this->start_lifetime.GetValue());
}

void gde::ParticleSystem::InvokeUpdate(float deltatime)
{
	this->time_last_spawned += deltatime;

	std::list<RigidObject*> toRemove;

	for (auto particle : this->particles)
	{
		particles[particle.first] -= deltatime;
		if (particles[particle.first] < 0) {
			particle.first->Destroy();
			toRemove.push_back(particle.first);
		}
	}

	for (auto removee : toRemove)
	{
		this->particles.erase(removee);
	}

	if (this->time_last_spawned > 1.0f / this->spawns_per_sec) {
		this->time_last_spawned = 0;

		this->Spawn();
	}
}
