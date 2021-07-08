#include "RigidBody2D.h"

RigidBody2D::RigidBody2D()
{
}

RigidBody2D::RigidBody2D(glm::vec3 position, float m, float leng, float wid) : GameObject(position)
{
	pos = position;
	mass = m;
	length = leng;
	width = wid;

	inertia = mass / 12 * (length * length + width * width);

	std::cout << inertia;
}

RigidBody2D::~RigidBody2D()
{


}

glm::vec3 RigidBody2D::ReturnPos()
{
	return glm::vec3(pos.x, pos.y, pos.z);
}

void RigidBody2D::Draw()
{
	glPushMatrix();

	glTranslatef(pos.x, pos.y, pos.z);
	glRotatef(orient, 0, 0, 1);
	glColor3f(1.f, 1.f, 1.f);

	glBegin(GL_QUADS);

	glVertex3f(-length, width, 0);
	glVertex3f(length, width, 0);
	glVertex3f(length, -width, 0);
	glVertex3f(-length, -width, 0);

	glEnd();

	glPointSize(5.0f);

	glColor3f(0, 0, 0);
	glBegin(GL_POINTS);

	glVertex3f(0, 0, 0);
	glEnd();

	glPopMatrix();

}

void RigidBody2D::CalculateForces()
{
	forces = glm::vec3(0, 0, 0);
	angForces = glm::vec3(0, 0, 0);
	//forces += (glm::vec3(0, -9.8, 0) + testForce)  * mass;
	forces += testForce * mass;
	angForces = glm::cross(testPos, testForce);

}

void RigidBody2D::Update(float delta)
{
	CalculateForces();

	angAccel = angForces / inertia;
	orient += glm::degrees(angVelocity.z * delta);
	angVelocity += angAccel * delta;

	accel = forces / mass;
	pos += velocity * delta;
	velocity += accel * delta;

	angVelocity *= pow(0.2, delta);

}
