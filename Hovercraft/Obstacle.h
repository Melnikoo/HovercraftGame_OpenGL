#pragma once
#pragma comment(lib, "glew32.lib") 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm\gtc\type_ptr.hpp> ///FOR MAT4 VALUE PTR USAGE
#include <vector> ///FOR SHADER COMPILE TEST
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/gtx/rotate_vector.hpp"
#include <glm\gtx/string_cast.hpp>
#include "objReader.h"
#include <map>
#include "SphereCollider.h"
#include "GameObject.h"



//This class loads cubes that move on the track. They have a collider
class Obstacle : public GameObject
{
private:

	objReader obst;
	unsigned int modelMatLoc, objLoc, texLoc;
	unsigned int vao1, objName;
	glm::mat4 modelMat;
public:

	bool increase = true;
	int count = 50;
	glm::vec3 pos;
	SphereCollider* collider;
	Obstacle(glm::vec3 position);
	~Obstacle();
	void LoadObject(unsigned int programId, unsigned int vao1, unsigned int buffer1, unsigned int objName);
		
	void Draw(glm::mat4 view);

	void Update(float deltaTime);

	void Draw();
};
