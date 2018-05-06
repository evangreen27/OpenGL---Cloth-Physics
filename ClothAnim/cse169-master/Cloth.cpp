#include "Cloth.h"

Cloth::Cloth(int height,int width, float blocksize,float Springconst, float Damperconst, glm::vec3 ori) {
	prevtime = ((float)glutGet(GLUT_ELAPSED_TIME)) / 1000.f;
	this->height = height;
	this->width = width;
	this->blocksize = blocksize;
	this->Springconst = Springconst;
	this->Damperconst = Damperconst;
	this->ori = ori;
	modelmat = glm::mat4(1.0f);
	for(int i = 0; i < height; i++) {
		bool stationary = false;
		if (i == 0) {
			stationary = true;
		}
		for (int j = 0; j < width; j++) {
			Particle* temp = new Particle(ori+glm::vec3(j*blocksize, -i*blocksize, 0), 0.01f, stationary);
			particles.push_back(temp);
		}
	}

	for (int i = 0; i < height - 1; i++) {
		for (int j = 0; j < width - 1; j++) {
			int upperleft = i*width + j;
			int upperright = upperleft + 1;
			int bottomleft = upperleft + width;
			int bottomright = bottomleft + 1;
			//std::cout << upperleft << " " << upperright << " " << bottomleft << " " << bottomright << std::endl;
			Triangle* temp = new Triangle(particles[upperleft], particles[bottomleft],  particles[upperright]);
			triangles.push_back(temp);
			temp = new Triangle(particles[bottomright], particles[upperright],  particles[bottomleft]);
			triangles.push_back(temp);
			SpringDamper* spr = new SpringDamper(particles[upperleft], particles[bottomright], (float)sqrt(2)*blocksize, Springconst, Damperconst);
			springs.push_back(spr);
			spr = new SpringDamper(particles[bottomleft], particles[upperright], (float)sqrt(2)*blocksize, Springconst, Damperconst);
			springs.push_back(spr);
			spr = new SpringDamper(particles[bottomright], particles[upperright], blocksize, Springconst, Damperconst);
			springs.push_back(spr);
			spr = new SpringDamper(particles[bottomright], particles[bottomleft], blocksize, Springconst, Damperconst);
			springs.push_back(spr);
		}
	}
	for (int i = 0; i < height - 1; i++) {
		SpringDamper* spr = new SpringDamper(particles[i*width], particles[(i+1)*width], blocksize, Springconst, Damperconst);
		springs.push_back(spr);
	}
	for (int j = 0; j < width - 1; j++) {
		SpringDamper* spr = new SpringDamper(particles[j], particles[(j + 1)], blocksize, Springconst, Damperconst);
		springs.push_back(spr);
	}
}

Cloth::~Cloth() {
	triangles.clear();
	particles.clear();
	springs.clear();
}

void Cloth::Draw(const glm::mat4 &viewProjMtx, uint shader) {
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "ModelMtx"), 1, false, (float*)&modelmat);
	glm::mat4 mvpMtx = viewProjMtx;
	glUniformMatrix4fv(glGetUniformLocation(shader, "ModelViewProjMtx"), 1, false, (float*)&mvpMtx);
	for (int i = 0; i < triangles.size(); i++) {
		triangles[i]->Draw();
	}
	glUseProgram(0);
}

void Cloth::Update(glm::vec3 windDir) {
	float currtime = ((float)glutGet(GLUT_ELAPSED_TIME)) / 2000.f;
	float timediff = currtime-prevtime;
	prevtime = currtime;
	computeNormals();
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->ApplyForce(9.8f*particles[i]->mass*glm::vec3(0,-1,0));
	}
	updateChildren(windDir, timediff);
}

void Cloth::computeNormals() {
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->normal = glm::vec3(0.0f);
	}
	for (int i = 0; i < triangles.size(); i++) {
		triangles[i]->computeNormal();
	}
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->normal = glm::normalize(particles[i]->normal);
	}
}

void Cloth::updateChildren(glm::vec3 windDir, float timediff) {
	for (int i = 0; i < triangles.size(); i++) {
		triangles[i]->Update(windDir);
	}
	for (int i = 0; i < springs.size(); i++) {
		springs[i]->ComputerForces();
	}
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->Update(timediff);
	}
}

void Cloth::move(bool left, glm::vec3 dir) {
	if (left) {
		for (int i = 0; i < width; i++) {
			particles[i]->position += dir*(((float)(width - i - 1)) / width);
		}
	}
	else {
		for (int i = 0; i < width; i++) {
			particles[i]->position += dir*((float)i / width);
		}
	}
}

void Cloth::reset() {
	particles.clear();
	triangles.clear();
	springs.clear();
	for (int i = 0; i < height; i++) {
		bool stationary = false;
		if (i == 0) {
			stationary = true;
		}
		for (int j = 0; j < width; j++) {
			Particle* temp = new Particle(ori + glm::vec3(j*blocksize, -i*blocksize, 0), 0.01f, stationary);
			particles.push_back(temp);
		}
	}

	for (int i = 0; i < height - 1; i++) {
		for (int j = 0; j < width - 1; j++) {
			int upperleft = i*width + j;
			int upperright = upperleft + 1;
			int bottomleft = upperleft + width;
			int bottomright = bottomleft + 1;
			//std::cout << upperleft << " " << upperright << " " << bottomleft << " " << bottomright << std::endl;
			Triangle* temp = new Triangle(particles[upperleft], particles[bottomleft], particles[upperright]);
			triangles.push_back(temp);
			temp = new Triangle(particles[bottomright], particles[upperright], particles[bottomleft]);
			triangles.push_back(temp);
			SpringDamper* spr = new SpringDamper(particles[upperleft], particles[bottomright], (float)sqrt(2)*blocksize, Springconst, Damperconst);
			springs.push_back(spr);
			spr = new SpringDamper(particles[bottomleft], particles[upperright], (float)sqrt(2)*blocksize, Springconst, Damperconst);
			springs.push_back(spr);
			spr = new SpringDamper(particles[bottomright], particles[upperright], blocksize, Springconst, Damperconst);
			springs.push_back(spr);
			spr = new SpringDamper(particles[bottomright], particles[bottomleft], blocksize, Springconst, Damperconst);
			springs.push_back(spr);
		}
	}
	for (int i = 0; i < height - 1; i++) {
		SpringDamper* spr = new SpringDamper(particles[i*width], particles[(i + 1)*width], blocksize, Springconst, Damperconst);
		springs.push_back(spr);
	}
	for (int j = 0; j < width - 1; j++) {
		SpringDamper* spr = new SpringDamper(particles[j], particles[(j + 1)], blocksize, Springconst, Damperconst);
		springs.push_back(spr);
	}
}