#include "particle.h"


Particle::Particle(glm::vec3 position, float m) : GameObject(position)
{	
	mass = m;
	pos = position;
	velocity = glm::vec3(0, 0, 0);
	acceleration = glm::vec3(0, 0, 0);
	forces = glm::vec3(0, 0, 0);
}

Particle::~Particle()
{
}

void Particle::Draw()
{
	glPushMatrix();
	glColor3f(0.3, 0.4, 0.5);
	glTranslatef(pos.x, pos.y, pos.z);
	glutSolidSphere(0.5, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.f, 0.f, 1.f);
	glBegin(GL_LINES);
	glVertex3f(pos.x, pos.y, pos.z);
	glVertex3f(pos.x + velocity.x, pos.y + velocity.y, pos.z + velocity.z);
	glEnd();
	glPopMatrix();
}

void Particle::CalculateForces()
{
	forces = glm::vec3(0,0,0);
	forces += glm::vec3(0, -5, 0) * mass;   // SET TO 9.81 LATER!!!
}

void Particle::Update(float deltaTime)
{
	CalculateForces();

	acceleration = forces / mass;
	pos = pos + velocity * deltaTime;
	velocity = velocity + acceleration * deltaTime;
}
