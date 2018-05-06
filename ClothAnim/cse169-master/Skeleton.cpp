#include "Skeleton.h"
#include "Tokenizer.h"
Skeleton::Skeleton(const char * filename)
{
	Load(filename);
	translate = glm::mat4(1.0f);
}

void Skeleton::Load(const char * filename)
{
	Tokenizer scanner;
	scanner.Open(filename);
	char buffer[64];
	scanner.GetToken(buffer);
	if (std::string(buffer) == "balljoint")
	{
		Joint * temp = new Joint(NULL);
		temp->name = "root";
		joints.push_back(temp);
		temp->Load(scanner, joints);
		root = temp;
	}
	scanner.Close();
}

void Skeleton::Draw(const glm::mat4 &viewProjMtx, uint shader) {
	if (ite != -1) {
		calculateDOFs();
	}
	root->Draw(viewProjMtx,shader);
}

void Skeleton::Update()
{
	root->Update(translate);
}

glm::mat4 Skeleton::GetWorldMatrix(int i) {
	return joints[i]->WorldMtx;
}

std::string Skeleton::UpdateJoint(int index, int dof, float amount) {
	joints[index]->UpdateDOF(dof, amount);
	return joints[index]->name;
}

void Skeleton::setTranslate(float x, float y, float z) {
	translate = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

void Skeleton::calculateDOFs() {
	glm::vec3 prepos = joints[index]->calculatePos();
	if (abs(glm::dot(glm::normalize(joints[index]->calculatePos()), glm::normalize(glm::vec3(x, y, z))) + 1) < 0.001f) {
		z = z + 0.02f;
	}
	for (int i = 0; i < index; i++) {
		if (joints[i]->approachPos(glm::vec3(x, y, z), joints[index], this)) {
			ite = -1;
			return;
		}
	}
	if (glm::length(joints[index]->calculatePos() - prepos) < 0.01f) {
		ite = -1;
		return;
	}
	prepos = joints[index]->calculatePos();
	ite++;
	//std::cout << joints[index]->calculatePos().x << " " << joints[index]->calculatePos().y << " " << joints[index]->calculatePos().z << std::endl;
}
void Skeleton::begin(int index, float x, float y, float z) {
	this->index = index;
	this->x = x;
	this->y = y;
	this->z = z;
	this->ite = 0;
}