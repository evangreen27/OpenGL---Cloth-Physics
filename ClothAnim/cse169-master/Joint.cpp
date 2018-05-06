#include "Joint.h"
#include "Skeleton.h"

Joint::Joint(Joint* parent) {
	LocalMtx = glm::mat4(1.0f);
	WorldMtx = glm::mat4(1.0f);
	DOF *xDOF = new DOF(0, -100000, 100000);
	DOF *yDOF = new DOF(0, -100000, 100000);
	DOF *zDOF = new DOF(0, -100000, 100000);
	DOFs.push_back(xDOF);
	DOFs.push_back(yDOF);
	DOFs.push_back(zDOF);
}

void Joint::Load(Tokenizer &scanner, std::vector<Joint*> &joints)
{
	bool end = false;
	while (!end)
	{
		char buffer[64];
		end = !scanner.GetToken(buffer);
		//std::cout << buffer << std::endl;
		if (std::string(buffer) == "balljoint")
		{
			Joint * temp = new Joint(this);
			joints.push_back(temp);
			temp->Load(scanner, joints);
			children.push_back(temp);
		}
		else if (std::string(buffer) == "}")
		{
			model.MakeBox(minBox, maxBox);
			return;
		}
		else if (std::string(buffer) == "offset")
		{
			float x = scanner.GetFloat();
			float y = scanner.GetFloat();
			float z = scanner.GetFloat();
			offset = glm::vec3(x, y, z);
			//std::cout << "offset " <<" "<<x <<" "<<y<<" "<<z << std::endl;
		}
		else if (std::string(buffer) == "boxmin")
		{
			float x = scanner.GetFloat();
			float y = scanner.GetFloat();
			float z = scanner.GetFloat();
			minBox = glm::vec3(x,y,z);
			//std::cout << "boxmin " << " " << x << " " << y << " " << z << std::endl;
		}
		else if (std::string(buffer) == "boxmax")
		{
			float x = scanner.GetFloat();
			float y = scanner.GetFloat();
			float z = scanner.GetFloat();
			maxBox = glm::vec3(x, y, z);
			//std::cout << "boxmax " << " " << x << " " << y << " " << z << std::endl;
		}
		else if (std::string(buffer) == "pose")
		{
			float x = scanner.GetFloat();
			float y = scanner.GetFloat();
			float z = scanner.GetFloat();
			DOFs[0]->setval(x);
			DOFs[1]->setval(y);
			DOFs[2]->setval(z);
			//std::cout << "pose " << " " << x << " " << y << " " << z << std::endl;
		}
		else if (std::string(buffer) == "rotxlimit")
		{
			float min = scanner.GetFloat();
			float max = scanner.GetFloat();
			DOFs[0]->min = min;
			DOFs[0]->max = max;
			//std::cout << "rotx " << " " << min << " " <<max << std::endl;
		}
		else if (std::string(buffer) == "rotylimit")
		{
			float min = scanner.GetFloat();
			float max = scanner.GetFloat();
			DOFs[1]->min = min;
			DOFs[1]->max = max;
			//std::cout << "roty " << " " << min << " " << max << std::endl;
		}
		else if (std::string(buffer) == "rotzlimit")
		{
			float min = scanner.GetFloat();
			float max = scanner.GetFloat();
			DOFs[2]->min = min;
			DOFs[2]->max = max;
			//std::cout << "rotx " << " " << min << " " << max << std::endl;
		}
		else if(std::string(buffer)!="{")
		{
			name = std::string(buffer);
			//std::cout << name << std::endl;
		}
	}
}
void Joint::Draw(const glm::mat4 &viewProjMtx, uint shader) {
	model.Draw(WorldMtx , viewProjMtx, shader);
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->Draw(viewProjMtx, shader);
	}
}

void Joint::Update(const glm::mat4 &parentMat)
{
	computeMatrices(parentMat);
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->Update(WorldMtx);
	}
}

void Joint::UpdateDOF(int dof, float amount)
{
	DOFs[dof]->setval(DOFs[dof]->val + amount);
}
void Joint::UpdateDOF(float x,float y,float z)
{
	//std::cout << DOFs.size() << std::endl;
	DOFs[0]->setval(x);
	//std::cout << 123456 << std::endl;
	DOFs[1]->setval(y);
	DOFs[2]->setval(z);
	//std::cout <<123456 << std::endl;

}

glm::vec3 Joint::calculatePos() {
	glm::vec3 pos = WorldMtx*glm::vec4(0, 0, 0, 1);
	return pos;
}

bool Joint::approachPos(glm::vec3 pos, Joint* target, Skeleton* skel) {
	glm::vec3 r = calculatePos();
	glm::vec3 e = target->calculatePos();
	glm::mat4 zrot = glm::rotate(glm::mat4(1.0f), DOFs[2]->val, glm::vec3(0, 0, 1));
	glm::mat4 yrot = glm::rotate(glm::mat4(1.0f), DOFs[1]->val, glm::vec3(0, 1, 0));
	glm::vec3 prepos = e;
	glm::vec3 step = 0.1f*(pos - e);
	//x dof
	glm::vec3 a = WorldMtx*zrot*yrot*glm::vec4(1, 0, 0, 0);
	glm::vec3 column = glm::cross(a, e - r);
	float change = glm::dot(column, step);
	DOFs[0]->setval(DOFs[0]->val + change);
	skel->Update();
	if (glm::length(target->calculatePos() - pos) < 0.01f) {
		//std::cout << "x" << std::endl;
		return true;
	}
	else {
		e = target->calculatePos();
	}
	//y dof
	a = WorldMtx*zrot*glm::vec4(0, 1, 0, 0);
	column = glm::cross(a, e - r);
	change = glm::dot(column, step);
	DOFs[1]->setval(DOFs[1]->val + change);
	skel->Update();
	if (glm::length(target->calculatePos() - pos) < 0.01f) {
		//std::cout << e.x << " " << e.y << " " << e.z << std::endl;
		//std::cout << "y" << std::endl;
		return true;
	}
	else {
		e = target->calculatePos();
	}
	//z dof
	a = WorldMtx*glm::vec4(0, 0, 1, 0);
	column = glm::cross(a, e - r);
	change = glm::dot(column, step);
	DOFs[2]->setval(DOFs[2]->val + change);
	skel->Update();
	if (glm::length(target->calculatePos() - pos) < 0.01f) {
		//std::cout << "z" << std::endl;
		return true;
	}
	else {
		e = target->calculatePos();
	}
	//std::cout << glm::length(prepos - e) << std::endl;

	if (glm::length(prepos - e) < 0.001f) {
		return false;
	}
	prepos = e;
	return false;
}

void Joint::computeMatrices(const glm::mat4 &parentMat) {
	LocalMtx = glm::mat4(1.0f);
	LocalMtx = LocalMtx*glm::rotate(glm::mat4(1.0f), DOFs[2]->val, glm::vec3(0, 0, 1));
	LocalMtx = LocalMtx*glm::rotate(glm::mat4(1.0f), DOFs[1]->val, glm::vec3(0, 1, 0));
	LocalMtx = LocalMtx*glm::rotate(glm::mat4(1.0f), DOFs[0]->val, glm::vec3(1, 0, 0));
	LocalMtx = glm::translate(glm::mat4(1.0f), offset)*LocalMtx;
	WorldMtx = parentMat*LocalMtx;
}
