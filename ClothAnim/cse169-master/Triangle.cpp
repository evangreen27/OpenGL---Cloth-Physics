#include"Triangle.h"
Triangle::Triangle(Particle *p1, Particle *p2, Particle *p3) {
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
	idx.push_back(0);
	idx.push_back(1);
	idx.push_back(2);
	idx.push_back(0);
	idx.push_back(2);
	idx.push_back(1);
	glGenBuffers(1, &VertexBuffer);
	glGenBuffers(1, &IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(uint), &idx[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	UpdateVTX();
}

Triangle::~Triangle() {
	delete p1;
	delete p2;
	delete p3;
}

void Triangle::Draw() {
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	uint posLoc = 0;
	glEnableVertexAttribArray(posLoc);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), 0);
	uint normLoc = 1;
	glEnableVertexAttribArray(normLoc);
	glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)12);
	glDrawElements(GL_TRIANGLES, idx.size(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Triangle::Update(glm::vec3 windDir) {
	UpdateVTX();
	ComputeAeroForce(windDir);
}

void Triangle::UpdateVTX() {
	vtx.clear();
	vtx.push_back({ p1->position,p1->normal });
	vtx.push_back({ p2->position,p2->normal });
	vtx.push_back({ p3->position,p3->normal });
	//std::cout << p1->position.x << " " << p1->position.y << " " << p1->position.z << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vtx.size() * sizeof(ModelVertex), &vtx[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Triangle::ComputeAeroForce(glm::vec3 airV) {
	glm::vec3 v = (p1->velocity + p2->velocity + p3->velocity) / 3.0f;
	v = v - airV;
	float area=0.5f*glm::length(glm::cross(p1->position - p2->position, p1->position - p3->position));
	area = area*glm::dot(v, normal) / glm::length(v);
	float pc = 1;
	glm::vec3 force = -0.5f*pc*glm::length(v)*glm::length(v)*area*normal;
	p1->ApplyForce(force / 3.0f);
	p2->ApplyForce(force / 3.0f);
	p3->ApplyForce(force / 3.0f);
}

void Triangle::computeNormal() {
	normal = glm::cross(p1->position - p2->position, p1->position - p3->position);
	normal = glm::normalize(normal);
	p1->normal += normal;
	p2->normal += normal;
	p3->normal += normal;
}