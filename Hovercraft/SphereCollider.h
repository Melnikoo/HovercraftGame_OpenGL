#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#pragma once
class SphereCollider
{
private:
	glm::vec3 pos;
	
public:
	float radius;
	SphereCollider(float rad, glm::vec3 position);

	SphereCollider(float rad);

	bool CollideCheck(SphereCollider& col);//check if collision happened

	void UpdatePos(glm::vec3 position);

	void UpdateRadius(float rad);

	glm::vec3 GetPos();

	void Draw();
};

