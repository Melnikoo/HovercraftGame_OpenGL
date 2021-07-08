#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include "GameObject.h"
#include <iostream>
#include <vector>

#include "Sphere.h"
#include "particle.h"
#include "Player.h"
#include "RigidBody2D.h"
#include "SphereCollider.h"
#include "Obstacle.h"

#include <cmath>
#include <fstream>

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GLUT/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
//#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "shader.h"
#include "getbmp.h"
#include "vertex.h"
#include "objReader.h" //We need add header for the obj reader
struct Material
{
	glm::vec4 ambRefl;
	glm::vec4 difRefl;
	glm::vec4 specRefl;
	glm::vec4 emitCols;
	float shininess;
};

class GameEngine
{
private:
	
	static std::vector<GameObject*> objects;//stores all object derived from GameObject class
	static int oldTimeSinceStart;//For calculating deltas in update function
	static int newTimeSinceStart;
	static void UpdateGame();
	static void DrawGame();
	static void CleanupEngine();
	static void cubeColour(Material color);
	static void ResizeWindow(int w, int l);
public:

	
	static int idChecker;

	//static GameObject* target;
	//static glm::vec3 cameraTarget;
	static void Setup(void);
	void AddGameObject(GameObject*);
	void StartEngine();
	void InitEngine(int argc, char** argv, const char* windowTitle, int width, int height);
	GameEngine();
	~GameEngine();
};
