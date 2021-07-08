#pragma once
#include "GameEngine.h"
#include "objReader.h"
class Player : public GameObject
{
private:
	objReader hovercraft;//holds hover's vertices
	float rad = 0.5f;
	SphereCollider* playerCol;
	unsigned int texture[2]; // Array of texture indices.
	unsigned int modelMatLoc, objLoc, texLoc;
	unsigned int vao1, vao2, objName;
	glm::mat4 modelMat;
	float accelDirection = 0;
	
public:

	float turnSpeed = 1.5;
	float mass = 1;
	float maxVelocity = 0.5;
	float force = 0.8;
	glm::vec3 direction = glm::vec3(0.0, 0.0, -1);  ///<Current heading (direction) the GameObject is facing.
	float rotationAngle;
	float rotation;
	float friction = 0.99;
	glm::vec3 pos;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 forces = glm::vec3(0,0,0);
	Player(glm::vec3 position, float m);
	void collides(SphereCollider* object2);
	~Player();

	void LoadObject(unsigned int programId, unsigned int vao1, unsigned int buffer1, unsigned int texture1, unsigned int objName);

	void Draw(glm::mat4 view);

	void Draw();

	void CalculateForces();

	void Update(float deltaTime);

};

