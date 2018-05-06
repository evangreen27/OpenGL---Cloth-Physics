#include "SpringDamper.h"
SpringDamper::SpringDamper(Particle* p1, Particle* p2, float length, float SpringConst, float DamperConst) {
	this->p1 = p1;
	this->p2 = p2;
	this->restLength = length;
	this->SpringConst = SpringConst;
	this->DamperConst = DamperConst;
}

SpringDamper::~SpringDamper() {
}

void SpringDamper::ComputerForces() {
	//spring force
	glm::vec3 e = glm::normalize(p1->position - p2->position);
	float x=glm::length((p1->position - p2->position)) - this->restLength;
	//std::cout << x << std::endl;
	p1->ApplyForce(-SpringConst*x*e);
	p2->ApplyForce(SpringConst*x*e);

	//damper force
	float v=glm::dot(e, p1->velocity) - glm::dot(e,p2->velocity);
	//std::cout << glm::length(p1->velocity)<<" "<<glm::length(p2->velocity) << std::endl;
	p1->ApplyForce(-DamperConst*v*e);
	p2->ApplyForce(DamperConst*v*e);
}
