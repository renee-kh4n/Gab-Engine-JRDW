#include "ParticleSystem.h"
#include "ParticleSystem.h"
#include "ParticleSystem.h"
#include "ParticleSystem.h"

gbe::ParticleSystem::ParticleSystem(std::function<RigidObject* (ParticleSystem*)> nparticle)
{
	this->particle_create = nparticle;
}

void gbe::ParticleSystem::InvokeUpdate(float deltatime)
{
	if (this->enabled) {
		this->time_lastspawned += deltatime;
	}

	if (time_lastspawned > this->time_perspawn) {
		this->time_lastspawned = 0.0f;

		gbe::Vector3 d = this->localbound_from - this->localbound_to;
		auto rand_01 = []() {
			return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			};

		auto random_local_pos = gbe::Vector3(d.x * rand_01(), d.y * rand_01(), d.z * rand_01());

		auto new_particle = this->particle_create(this);
		new_particle->SetWorldPosition(this->World().position.Get() + random_local_pos);

		this->particles.insert_or_assign(new_particle, 0);
	}

	std::vector<RigidObject*> expired;

	for (auto& particle : this->particles)
	{
		if (this->particles.size() == 0)
			break;

		particle.second += deltatime;

		if (particle.second > this->life_time) {
			particle.first->Destroy();
			expired.push_back(particle.first);

		}
		else {
			particle.first->GetRigidbody()->AddForce((physics::PhysicsVector3)(this->continuous_force * deltatime));
		}
	}

	for (auto erasable : expired)
	{
		this->particles.erase(erasable);
	}
}

void gbe::ParticleSystem::SetBounds(Vector3 from, Vector3 to)
{
	this->localbound_from = from;
	this->localbound_to = to;
}

void gbe::ParticleSystem::Set_force(Vector3 newforce)
{
	this->continuous_force = newforce;
}

void gbe::ParticleSystem::Set_rate(float rate)
{
	this->time_perspawn = 1.0f / rate;
}

void gbe::ParticleSystem::Set_lifetime(float len)
{
	this->life_time = len;
}

void gbe::ParticleSystem::Set_enabled(bool to)
{
	this->enabled = to;
}
