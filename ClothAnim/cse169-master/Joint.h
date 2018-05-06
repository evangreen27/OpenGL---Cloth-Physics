#pragma once
#include "core.h"
#include "DOF.h"
#include "Model.h"
#include "Tokenizer.h"
class Skeleton;
class Joint {
public:
	glm::mat4x4 WorldMtx;
	glm::mat4x4 LocalMtx;
	Joint* parent;
	Model model;
	glm::vec3 minBox;
	glm::vec3 maxBox;
	glm::vec3 offset;
	std::vector<DOF*> DOFs;
	std::string name;
	std::vector<Joint*> children;
	Joint(Joint * parent);
	void Draw(const glm::mat4 &viewProjMtx, uint shader);
	void Update(const glm::mat4 &parentMat);
	void Load(Tokenizer &scanner, std::vector<Joint*> &joints);
	void UpdateDOF(int dof, float amount);
	void UpdateDOF(float x, float y, float z);
	glm::vec3 calculatePos();
	void computeMatrices(const glm::mat4 &parentMat);
	bool approachPos(glm::vec3 pos, Joint* target, Skeleton* skel);
};
