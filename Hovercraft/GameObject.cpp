#include "GameObject.h"
std::map<char, bool> GameObject::keys;
std::map<int, bool> GameObject::specialKeys;

GameObject::GameObject()
{
}

GameObject::GameObject(glm::vec3 position)
{
	pos = position;
}

GameObject::GameObject(glm::vec3 position, glm::vec3 color)
{
	pos = position;
	col = color;
}

GameObject::~GameObject()
{
}

void GameObject::attachCol(SphereCollider& col)
{
	sphereCol = &col;
}

glm::vec3 GameObject::GetPosition()
{
	glm::vec3 position = this->pos;
	return position;
}

SphereCollider * GameObject::GetCollider()
{
	return sphereCol;
}

void GameObject::Update(float)
{

}


