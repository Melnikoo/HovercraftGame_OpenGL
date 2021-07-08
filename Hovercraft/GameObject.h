#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include "SphereCollider.h"
#include<map>

class GameObject
{
protected:
	glm::vec3 pos;
	glm::vec3 col;
	SphereCollider* sphereCol;
public:
	static std::map<char, bool> keys;
	static std::map<int, bool> specialKeys;


	GameObject();
	GameObject(glm::vec3 position);
	GameObject(glm::vec3 position, glm::vec3 color);
	~GameObject();

	void attachCol(SphereCollider& col);
	glm::vec3 GetPosition();
	SphereCollider* GetCollider();
	

	virtual void Draw() = 0;
	virtual void Update(float);

};

