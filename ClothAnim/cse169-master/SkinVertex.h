#include "Core.h"
#include "Skeleton.h"
struct attachment {
	int jointId;
	float weight;
};
class SkinVertex {
public:
	glm::vec3 localPosition;
	glm::vec3 WorldPosition;
	std::vector<attachment> attaches;
	glm::vec3 Worldnormal;
	glm::vec3 normal;
	glm::vec2 textcoord;
	int index;
	SkinVertex(float x, float y, float z, int i);
	void addAttach(int id, float weight);
	glm::vec3 calculatePos(std::vector<glm::mat4> &binds, Skeleton* skel);
	~SkinVertex();
};