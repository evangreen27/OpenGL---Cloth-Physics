#include "SkinVertex.h"
SkinVertex::SkinVertex(float x, float y, float z,int i)
{
	WorldPosition = glm::vec3(0, 0, 0);
	localPosition = glm::vec3(x, y, z);
	index = i;
}
void SkinVertex::addAttach(int id, float weight)
{
	attaches.push_back({ id,weight });
}

glm::vec3 SkinVertex::calculatePos(std::vector<glm::mat4> &binds, Skeleton* skel) {
	WorldPosition = glm::vec3(0, 0, 0);
	Worldnormal = glm::vec3(0, 0, 0);
	for (int i = 0; i < attaches.size(); i++)
	{		
		WorldPosition += attaches[i].weight*glm::vec3((skel->GetWorldMatrix(attaches[i].jointId)*glm::inverse(binds[attaches[i].jointId]))*glm::vec4(localPosition,1));
		Worldnormal += attaches[i].weight*glm::vec3(glm::mat3(skel->GetWorldMatrix(attaches[i].jointId)*glm::inverse(binds[attaches[i].jointId]))*normal);
	}
	Worldnormal = glm::normalize(Worldnormal);
	return WorldPosition;
}

SkinVertex::~SkinVertex() {

}
