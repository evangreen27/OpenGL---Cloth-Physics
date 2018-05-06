#pragma once
#include "Core.h"
class Particle
{
public:
	float mass;
	bool stationary;
	glm::vec3 force;
	glm::vec3 velocity;
	glm::vec3 position;
	glm::vec3 normal;
	Particle(glm::vec3 pos,float mass,bool stationary);
	void Update(float time);
	void ApplyForce(glm::vec3 newforce);
	~Particle();
};

