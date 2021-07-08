#include "Obstacle.h"

Obstacle::Obstacle(glm::vec3 position)
{
	pos = position;

	collider = new SphereCollider(4.0, position);
	this->attachCol(*collider);
}

void Obstacle::LoadObject(unsigned int programId, unsigned int vao1, unsigned int buffer1, unsigned int objName)
{
	this->vao1 = vao1;

	obst.LoadModel((char*)("./Models/cuteBox.obj"));

	glBindVertexArray(vao1);
	glBindBuffer(GL_ARRAY_BUFFER, buffer1);   // Bind vertex buffer 
	glBufferData(GL_ARRAY_BUFFER, sizeof(fVertex) * obst.numVertices, obst.v, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(fVertex), 0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(fVertex), (void*)(sizeof(fVertex::coords)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(fVertex), (void*)(sizeof(fVertex::normals)));
	glEnableVertexAttribArray(5);

	modelMatLoc = glGetUniformLocation(programId, "modelViewMat");
	objLoc = glGetUniformLocation(programId, "object");
	this->objName = objName;
}

void Obstacle::Draw(glm::mat4 view)
{
	modelMat = glm::mat4(1.0);
	modelMat = view;
	modelMat = glm::translate(modelMat, pos);
	glm::vec3 scale = glm::vec3(2.5f, 2.5f, 2.5f);
	modelMat = glm::scale(modelMat, scale);
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, value_ptr(modelMat));
	glUniform1ui(objLoc, objName);  //if (object == CUBE)
	glBindVertexArray(vao1);
	glDrawArrays(GL_TRIANGLES, 0, obst.numVertices);
}


void Obstacle::Update(float deltaTime)
{	
	
	if (count == 250)
		increase = false;
	else if (count == 0)
		increase = true;

	if (count < 250 && increase)
	{
		pos.x += 0.1f;
		count++;
	}
	if (count > 0 && !increase)
	{
		pos.x -= 0.1f;
		count--;
	}
	collider->UpdatePos(pos);

	
	
}
void Obstacle::Draw()
{
}



Obstacle::~Obstacle()
{
	delete collider;
}
