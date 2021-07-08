#include "GameEngine.h"

//GameObject* cube1 = new Cube(glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), 1.0f, false);
//GameObject* cube2 = new Cube(glm::vec3(0, -5, 0), glm::vec3(0, 0.5, 0.5), 10.f,  false);
//GameObject* cone1 = new Cone(glm::vec3(-1, -2, 2), glm::vec3(140, 65, 200));
//GameObject* sphere1 = new Sphere(glm::vec3(3, -3, 0), glm::vec3(0.5, 0.5, 0.5), 2);
//GameObject* particle1 = new Particle(glm::vec3(0, 0, 0), 0.01f);
//GameObject* hover = new Player(glm::vec3(0, 0, 0), 10.0f);
//GameObject* rigid1 = new RigidBody2D(glm::vec3(0, 0, 0), 2.0f, 1.0f, 1.0f);
//GameObject* cube1 = new Obstacle(glm::vec3(5, 0, 0));


GameEngine engine;
// Main routine.
int main(int argc, char** argv)
{
	//engine.AddGameObject(hover);
	//engine.AddGameObject(cube1);
	engine.InitEngine(argc, argv, "WorldOfHovercraft", 500, 500);
	//engine.AddGameObject(rigid1);
	//engine.AddGameObject(cube1);
	//engine.AddGameObject(cube2);
	//engine.AddGameObject(cone1);
	//engine.AddGameObject(sphere1);
	//engine.AddGameObject(particle1);
	//engine.AddGameObject(player1);

	engine.Setup();

	engine.StartEngine();

	return 0;

}