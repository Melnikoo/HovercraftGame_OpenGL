#include <string>
#include <iostream>
#include "GameObject.h"
#include "SphereCollider.h"
class RigidBody2D : public GameObject
{
private:
	float inertia;
	float mass;
	float orient = 0;

	float length;
	float width;

	glm::vec3 testPos = glm::vec3(1, 1, 0);
	glm::vec3 testForce = glm::vec3(2, 0, 0);
	

	glm::vec3 forces;
	glm::vec3 accel;
	glm::vec3 velocity;
	glm::vec3 angVelocity;
	glm::vec3 angAccel;
	glm::vec3 angForces;

	std::string shape;

	

public:
	glm::vec3 pos;
	RigidBody2D();

	RigidBody2D(glm::vec3 position, float m, float length, float width);

	~RigidBody2D();

	glm::vec3 ReturnPos();

	void CalculateForces();

	void Draw();

	void Update(float delta);
};
