#pragma once
#include "Core.h"
#include "Model.h"
#include "Particle.h"
class Triangle {
public:
	Particle *p1, *p2, *p3;
	glm::vec3 normal;
	std::vector<ModelVertex> vtx;
	std::vector<uint> idx;
	uint VertexBuffer;
	uint IndexBuffer;
	Triangle(Particle *p1, Particle *p2, Particle *p3);
	~Triangle();
	void Draw();
	void Update(glm::vec3 windDir);
	void UpdateVTX();
	void ComputeAeroForce(glm::vec3 airV);
	void computeNormal();
};