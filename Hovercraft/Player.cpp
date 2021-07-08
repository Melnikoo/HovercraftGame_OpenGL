#include "Player.h"

//SphereCollider* playerCol = new SphereCollider(0.5f, glm::vec3(0, 0, 0));

Player::Player(glm::vec3 position, float m)
{
	pos = position;
	mass = m;
	acceleration = glm::vec3(0, 0, 0);
	velocity = glm::vec3(0, 0, 0);
	forces = glm::vec3(0, 0, 0);

	playerCol = new SphereCollider(5.0f, position);
	this->attachCol(*playerCol);
	
}

void Player::collides(SphereCollider* object2)
{

	glm::vec3 delta = pos - object2->GetPos();
	delta = glm::normalize(delta);

	velocity += delta * 0.01f;

	//std::cout << "Doing a collision!" << std::endl;
}

Player::~Player()
{
	delete playerCol;
}

void Player::LoadObject(unsigned int programId, unsigned int vao1, unsigned int buffer1, unsigned int texture1, unsigned int objName)
{
	texture[0] = texture1;
	
	this->vao1 = vao1;
	

	hovercraft.LoadModel((char*)"./Models/HoverCraft.obj");

	//bind hovercraft
	glBindVertexArray(vao1);
	glBindBuffer(GL_ARRAY_BUFFER, buffer1);   // Bind vertex buffer 
	glBufferData(GL_ARRAY_BUFFER, sizeof(fVertex) * hovercraft.numVertices, hovercraft.v, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(fVertex), 0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(fVertex), (void*)(sizeof(fVertex::coords)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(fVertex), (void*)(sizeof(fVertex::normals)));
	glEnableVertexAttribArray(5);
	
	modelMatLoc = glGetUniformLocation(programId, "modelViewMat");
	objLoc = glGetUniformLocation(programId, "object");
	texLoc = glGetUniformLocation(programId, "metalDots");

	this->objName = objName;
}

void Player::Draw(mat4 view)
{
	//sphereCol->Draw();
	modelMat = mat4(1.0);
	modelMat = view;

	modelMat = glm::translate(modelMat, pos);

	modelMat = glm::rotate(modelMat, glm::radians(rotationAngle), glm::vec3(0, 1, 0));
	modelMat = translate(modelMat, vec3(0.0, 2.0, 0.0));

	vec3 scale = glm::vec3(1.5f, 1.5f, 1.5f);
	modelMat = glm::scale(modelMat, scale);
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, value_ptr(modelMat));  //send to the shader
	glUniform1ui(objLoc, objName);  //if (object == HOVER)
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBindVertexArray(vao1);
	glDrawArrays(GL_TRIANGLES, 0, hovercraft.numVertices);

	sphereCol->UpdatePos(pos);
}

void Player::Draw()
{
}

void Player::CalculateForces()
{
	forces = glm::vec3(0, 0, 0);
}


void Player::Update(float deltaTime)
{
	forces = glm::vec3(0, 0, 0);

	float speed = glm::length(velocity);

	acceleration = direction * accelDirection * force / mass;

	if (speed < maxVelocity)
	{
		velocity += acceleration * deltaTime;
	}

	if (GameObject::specialKeys[GLUT_KEY_UP] == true && speed < maxVelocity)
	{
		accelDirection = -1;
		pos += velocity;

	}
	else if (GameObject::specialKeys[GLUT_KEY_DOWN] == true && speed < maxVelocity)
	{
		accelDirection = +1;
		pos += velocity;

	}
	else
	{
		accelDirection = 0; //acceleration will drop to 0 as well
		velocity *= friction;
		pos += velocity;
	}
	if (GameObject::specialKeys[GLUT_KEY_LEFT] == true)
	{
		rotationAngle += turnSpeed / friction;
		velocity *= friction;
	}

	else if (GameObject::specialKeys[GLUT_KEY_RIGHT] == true)
	{
		rotationAngle -= turnSpeed / friction;
		velocity *= friction;
	}

	glm::mat4 rotation = glm::mat4(1.0);
	rotation = glm::rotate(rotation, glm::radians(rotationAngle), glm::vec3(0, 1, 0));
	direction = glm::normalize(glm::vec3(rotation[2]));
}
