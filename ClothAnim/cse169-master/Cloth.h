#pragma once
#include "Core.h"
#include "SpringDamper.h"
#include "Triangle.h"

class Cloth {
public:
	int height;
	int width;
	float prevtime;
	float Springconst;
	float blocksize;
	float Damperconst;
	glm::vec3 ori;
	glm::mat4 modelmat;
	std::vector<Triangle*> triangles;
	std::vector<SpringDamper*> springs;
	std::vector<Particle*> particles;
	Cloth(int height, int width, float blocksize, float Springconst, float Damperconst, glm::vec3 ori);
	~Cloth();
	void Draw(const glm::mat4 &viewProjMtx, uint shader);
	void Update(glm::vec3 windDir);
	void computeNormals();
	void updateChildren(glm::vec3 windDir, float timediff);
	void move(bool left, glm::vec3 dir);
	void reset();
};