#include "SphereCollider.h"
#pragma once

SphereCollider::SphereCollider(float rad, glm::vec3 position)
{
	radius = rad;
	pos = position;
}

SphereCollider::SphereCollider(float rad)
{
	radius = rad;
	
}

bool SphereCollider::CollideCheck(SphereCollider& col)//check if collision happened
{
	glm::vec3 distance = col.pos - pos;
	float distLength = glm::length(distance);
	float radTotal = radius + col.radius;

	if (distLength <= radTotal)
	{
		return true;
	}
	return false;
}

void SphereCollider::UpdatePos(glm::vec3 position)
{
	pos = position;
}

void SphereCollider::UpdateRadius(float rad)
{
	radius = rad;
}

glm::vec3 SphereCollider::GetPos()
{
	return pos;
}

void SphereCollider::Draw()
{
	/*glPushMatrix();
	glColor3f(1.f, 0.f, 1.0f);
	glTranslatef(pos.x, pos.y, pos.z);
	glutWireSphere(radius, 10, 10);
	glPopMatrix();*/
}
