#pragma once
#include "SphereCollider.h"
#include "GameObject.h"
#include "vertex.h"
#include <vector>
#include "GameEngine.h"
#include "GameObject.h"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

using namespace glm;
using namespace std;

class collisionManager;

class Sphere : public GameObject
{
private:

	int sizeX;
	int sizeY;
	//Shere creation
	//REFERENCE: 'FieldAndSkyFilteredShaderized.sln'. From Computer Graphics Through OpenGL by Samanta Guha
	VertexWtihNormal* sphereVerticesNor;  //121
	unsigned int* sphereIndices; //660
	unsigned int modelMatLoc, objLoc, texLoc;
	glm::mat4 modelMat;
	int stacks; //10
	int slices; //10
	unsigned int vao1;
	void CreateSpherewithNormal();
public:
	SphereCollider* collider = NULL;

	int idd;
	//static int counter = 0;

	void loadObject(unsigned int programId, unsigned int vao1, unsigned int buffer1, unsigned int buffer2);

	static int idChecker;

	vec3 positionC;
	vec3 directionC;

	float radius;

	Sphere();
	Sphere(vec3 pos, int i);
	~Sphere();

	void Draw(mat4 view);
	void Draw();
	void update(int deltaTime);
	void updatePos();

	VertexWtihNormal* GetVerData(int&);
	unsigned int* GetTriData(int&);
};
