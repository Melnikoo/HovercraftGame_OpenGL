#pragma once
#include "GameObject.h"

class Particle : public GameObject
{
private:
	float mass;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 forces;
public:
	Particle(glm::vec3 position, float m);
	~Particle();


	void Draw();

	void CalculateForces();

	void Update(float deltaTime);
};



