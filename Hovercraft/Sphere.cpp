
#include "vertex.h"
#include "sphere.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

//SphereCollider* sphereCol = new SphereCollider(0.5f, glm::vec3(0, 0, 0));

Sphere::Sphere()
{
	//counter = counter + 1;
	stacks = 10;
	slices = 10;
	radius = 1.0f;

	sphereVerticesNor = (VertexWtihNormal*)malloc(sizeof(VertexWtihNormal) * 121);
	sphereIndices = (unsigned int*)malloc(sizeof(unsigned int) * 660);
	CreateSpherewithNormal();
}

Sphere::Sphere(glm::vec3 position, int i) : GameObject(position)
{
	stacks = 10;
	slices = 10;
	radius = 1.0f;

	positionC = pos;
	directionC = glm::vec3(0.0f, 0.0f, 0.0f);

	idd = i;

	sphereVerticesNor = (VertexWtihNormal*)malloc(sizeof(VertexWtihNormal) * 121);
	sphereIndices = (unsigned int*)malloc(sizeof(unsigned int) * 660);
	CreateSpherewithNormal();

	collider = new SphereCollider(0.5f, glm::vec3(0, 0, 0));
	this->attachCol(*collider);
}

void Sphere::CreateSpherewithNormal(void)
{
	int count;
	count = 0;
	for (int i = 0; i <= stacks; ++i) {

		GLfloat V = i / (float)stacks;
		GLfloat phi = V * glm::pi <float>();

		// Loop Through Slices
		for (int j = 0; j <= slices; ++j) {

			GLfloat U = j / (float)slices;
			GLfloat theta = U * (glm::pi <float>() * 2);

			// Calc The Vertex Positions
			GLfloat x = cosf(theta) * sinf(phi);
			GLfloat y = cosf(phi);
			GLfloat z = sinf(theta) * sinf(phi);

			sphereVerticesNor[count].coords = glm::vec4(x * radius, y * radius + 6.0, z * radius, 1.0);
			sphereVerticesNor[count].normals = glm::vec3(x, y, z); ///Sphere normals
			count++;
		}
	}

	count = 0;
	// Calc The Index Positions
	for (int i = 0; i < slices * stacks + slices; ++i) {

		sphereIndices[count] = i;
		count++;
		sphereIndices[count] = i + slices + 1;
		count++;
		sphereIndices[count] = i + slices;
		count++;

		sphereIndices[count] = i + slices + 1;
		count++;
		sphereIndices[count] = i;
		count++;
		sphereIndices[count] = i + 1;
		count++;
	}

	count = 0;


}
void Sphere::loadObject(unsigned int programId, unsigned int vao1, unsigned int buffer1,unsigned int buffer2)
{
	this->vao1 = vao1;
	int verCount, triCount;
	sphereVerticesNor = GetVerData(verCount);
	sphereIndices = GetTriData(triCount);

	glBindVertexArray(vao1);
	glBindBuffer(GL_ARRAY_BUFFER, buffer1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexWtihNormal) * verCount, sphereVerticesNor, GL_STATIC_DRAW);  ///please note the change
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * triCount, sphereIndices, GL_STATIC_DRAW); ///please note the change
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(sphereVerticesNor[0]), 0);  //layout(location=4) in vec4 fieldCoords;
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(sphereVerticesNor[0]), (GLvoid*)sizeof(sphereVerticesNor[0].normals));
	glEnableVertexAttribArray(5);

	modelMatLoc = glGetUniformLocation(programId, "modelMat");
	objLoc = glGetUniformLocation(programId, "obj");

}
VertexWtihNormal* Sphere::GetVerData(int& verNum)
{
	verNum = 121;
	return sphereVerticesNor;
}

unsigned int* Sphere::GetTriData(int& triNum)
{
	triNum = 660;
	return sphereIndices;
}

Sphere::~Sphere()
{
	delete collider; free(sphereVerticesNor);
	free(sphereIndices);
	delete collider;
	collider = NULL;
}
void Sphere::Draw(mat4 view)
{
	mat4 currentView = view;
	int triCount;
	sphereIndices = GetTriData(triCount);

	modelMat = mat4(1.0);
	modelMat = view;
	modelMat = translate(modelMat, vec3(positionC.x, positionC.y, positionC.z));
	modelMat = scale(modelMat, vec3(0.8, 0.8, 0.8));

	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, value_ptr(modelMat));  //send tp the shader
	glUniform1ui(objLoc, vao1);  //if (object == SPHERE)
	glBindVertexArray(vao1);
	glDrawElements(GL_TRIANGLE_STRIP, triCount, GL_UNSIGNED_INT, sphereIndices);  ///use index array to control triangle drawing
	//sphereCol->Draw();
}

void Sphere::Draw()
{
}
