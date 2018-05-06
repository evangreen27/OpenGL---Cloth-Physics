#pragma once
#include "Core.h"
#include "Joint.h"
class Skeleton {
public:
	int ite = -1;
	int index;
	float x, y, z;
	Joint * root;
	glm::mat4 translate;
	std::vector<Joint*> joints;
	Skeleton(const char * filename);
	void Load(const char * filename);
	void Update();
	void Draw(const glm::mat4 &viewProjMtx, uint shader);
	glm::mat4 GetWorldMatrix(int i);
	std::string UpdateJoint(int index, int dof, float amount);
	void setTranslate(float x, float y, float z);
	void calculateDOFs();
	void begin(int index, float x, float y, float z);
};