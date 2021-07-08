#include "GameEngine.h"


static enum object { FIELD, SKY_FRONT, HOVER, TRACK, FANS, SKY_LEFT, SKY_RIGHT, SKY_BACK, SKY_UP, CUBE}; // VAO ids.
static enum buffers { FIELD_VERTICES, SKY_FRONT_VERTICES, HOVER_VERTICES, TRACK_VERTICES, FANS_VERTICES, SKY_LEFT_VERTICES, SKY_RIGHT_VERTICES, SKY_BACK_VERTICES, SKY_UP_VERTICES, CUBE_VERTICES}; // VBO ids.

std::vector<GameObject*> GameEngine::objects;
int GameEngine::oldTimeSinceStart;
int GameEngine::newTimeSinceStart;

using namespace std;
using namespace glm;

struct Light
{
	vec4 ambCols;
	vec4 difCols;
	vec4 specCols;
	vec4 coords;
};

// Globals.
static Vertex fieldVertices[] =
{
	{vec4(250.0, 0.0, 250.0, 1.0), vec2(8.0, 0.0)},
	{vec4(250.0, 0.0, -250.0, 1.0), vec2(8.0, 8.0)},
	{vec4(-250.0, 0.0, 250.0, 1.0), vec2(0.0, 0.0)},
	{vec4(-250.0, 0.0, -250.0, 1.0), vec2(0.0, 8.0)}
};

static Vertex skyVerticesFront[4] =
{
	{vec4(200.0, 0.0, -200, 1.0), vec2(1.0, 0.0)},
	{vec4(200.0, 240.0, -200, 1.0), vec2(1.0, 1.0)},
	{vec4(-200.0, 0.0, -200, 1.0), vec2(0.0, 0.0)},
	{vec4(-200.0, 240.0, -200.0, 1.0), vec2(0.0, 1.0)}
};

static Vertex skyVerticesLeft[4] =
{
	{vec4(-200.0, 0.0, -200.0, 1.0), vec2(1.0, 0.0)},
	{vec4(-200.0, 240.0, -200.0, 1.0), vec2(1.0, 1.0)},
	{vec4(-200.0, 0.0, 200.0, 1.0), vec2(0.0, 0.0)},
	{vec4(-200.0, 240.0, 200.0, 1.0), vec2(0.0, 1.0)}
};

static Vertex skyVerticesRight[4] =
{
	{vec4(200.0, 0.0, 200.0, 1.0), vec2(1.0, 0.0)},
	{vec4(200.0, 240.0, 200.0, 1.0), vec2(1.0, 1.0)},
	{vec4(+200.0, 0.0, -200.0, 1.0), vec2(0.0, 0.0)},
	{vec4(+200.0, 240.0, -200.0, 1.0), vec2(0.0, 1.0)}
};
//////
static Vertex skyVerticesBack[4] =
{
	{vec4(-200.0, 0.0, 200.0, 1.0), vec2(1.0, 0.0)},
	{vec4(-200.0, 240.0, 200.0, 1.0), vec2(1.0, 1.0)},
	{vec4(200.0, 0.0, 200.0, 1.0), vec2(0.0, 0.0)},
	{vec4(200.0, 240.0, 200.0, 1.0), vec2(0.0, 1.0)}
};

static Vertex skyVerticesUp[4] =
{
	{vec4(-200.0, 240, 200.0, 1.0), vec2(1.0, 0.0)},
	{vec4(-200.0, 240.0, -200.0, 1.0), vec2(1.0, 1.0)},
	{vec4(200.0, 240.0, 200.0, 1.0), vec2(0.0, 0.0)},
	{vec4(200.0, 240.0, -200.0, 1.0), vec2(0.0, 1.0)}
};

static mat4 modelViewMat = mat4(1.0);
static mat4 projMat = mat4(1.0);
static mat3 normalMat = mat3(1.0);  ///create normal matrix

static const vec4 globAmb = vec4(0.2, 0.2, 0.2, 1.0);
// Front and back material properties.
static const Material sphereFandB =//yellow
{
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(0.0, 0.0, 0.0, 1.0),
	50.0f
};

static const Material cubeRed =
{
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(1.0, 0.0, 0.0, 1.0),
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(0.0, 0.0, 1.0, 1.0),
	50.0f
};

static const Material cubeBlue =
{
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(0.0, 0.0, 1.0, 1.0),
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(0.0, 0.0, 1.0, 1.0),
	50.0f
};

static const Light light0 =
{
	vec4(0.0, 0.0, 0.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 0.0, 0.0)
};



float orient = 10; // For rotating the fans
int cameraMode = 1; // main camera by default
float cameraEffect;  //Will change depending on the hover's acceleration in update

static unsigned int
programId,
vertexShaderId,
fragmentShaderId,
modelViewMatLoc,
projMatLoc,
grassTexLoc,
skyTexLoc,
metalTexLoc,
trackTexLoc,
objectLoc,
buffer[10], ///add one more object
vao[10], ///add one more object
texture[4];






static BitMapFile* image[4]; // Local storage for bmp image data.

static float d = 0.0;
static float f = 0.0;// Distance parameter in gluLookAt().
static float t = 0.0;
static float xVal = 0, zVal = 0; // Co-ordinates of the ball.


static objReader hoverCraft;
static objReader track;
static objReader fans;


static Obstacle cube1(vec3(15,0,0));
static Obstacle cube2(vec3(-80, 0, 10));
static Obstacle cube3(vec3(-5, 0, -25));

static Player hover(glm::vec3(0, 0, 0), 10);

// Initialization routine.
void GameEngine::Setup(void)
{

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	// Create shader program executable.
	vertexShaderId = setShader((char*)"vertex", (char*)"vertexShader.glsl");
	fragmentShaderId = setShader((char*)"fragment", (char*)"fragmentShader.glsl");
	programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	glUseProgram(programId);

	//codes for OpenGL lighting
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.ambRefl"), 1, &sphereFandB.ambRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.difRefl"), 1, &sphereFandB.difRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.specRefl"), 1, &sphereFandB.specRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.emitCols"), 1, &sphereFandB.emitCols[0]);
	glUniform1f(glGetUniformLocation(programId, "sphereFandB.shininess"), sphereFandB.shininess);

	glUniform4fv(glGetUniformLocation(programId, "globAmb"), 1, &globAmb[0]);

	glUniform4fv(glGetUniformLocation(programId, "light0.ambCols"), 1, &light0.ambCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.difCols"), 1, &light0.difCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.specCols"), 1, &light0.specCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.coords"), 1, &light0.coords[0]);


	// Create VAOs and VBOs... 
	glGenVertexArrays(10, vao); ///add one more object
	glGenBuffers(10, buffer);  ///add one more object
	hoverCraft.LoadModel((char*)"./Models/HoverCraft.obj");
	track.LoadModel((char*)"./Models/FinalTrack.obj");
	fans.LoadModel((char*)"./Models/fans5.obj");

	cube1.LoadObject(programId, vao[CUBE], buffer[CUBE_VERTICES], CUBE);
	cube2.LoadObject(programId, vao[CUBE], buffer[CUBE_VERTICES], CUBE);
	cube3.LoadObject(programId, vao[CUBE], buffer[CUBE_VERTICES], CUBE);
	hover.LoadObject(programId, vao[HOVER], buffer[HOVER_VERTICES], texture[2], HOVER);
	

	// ---------------------------------------------------------------Bind FIELD
	glBindVertexArray(vao[FIELD]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[FIELD_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fieldVertices), fieldVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(fieldVertices[0]), 0);  //layout(location=0) in vec4 fieldCoords;
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(fieldVertices[0]), (void*)(sizeof(fieldVertices[0].coords)));  //layout(location=1) in vec2 fieldTexCoords;
	glEnableVertexAttribArray(1);

	// --------------------------------------------------------------Bind SKY
	glBindVertexArray(vao[SKY_FRONT]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[SKY_FRONT_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyVerticesFront), skyVerticesFront, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(skyVerticesFront[0]), 0);  //layout(location=2) in vec4 skyCoords;
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(skyVerticesFront[0]), (void*)(sizeof(skyVerticesFront[0].coords))); //layout(location=3) in vec2 skyTexCoords;
	glEnableVertexAttribArray(3);

	glBindVertexArray(vao[SKY_LEFT]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[SKY_LEFT_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyVerticesLeft), skyVerticesLeft, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(skyVerticesLeft[0]), 0);  //layout(location=2) in vec4 skyCoords;
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(skyVerticesLeft[0]), (void*)(sizeof(skyVerticesLeft[0].coords))); //layout(location=3) in vec2 skyTexCoords;
	glEnableVertexAttribArray(3);

	glBindVertexArray(vao[SKY_RIGHT]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[SKY_RIGHT_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyVerticesRight), skyVerticesRight, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(skyVerticesRight[0]), 0);  //layout(location=2) in vec4 skyCoords;
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(skyVerticesRight[0]), (void*)(sizeof(skyVerticesRight[0].coords))); //layout(location=3) in vec2 skyTexCoords;
	glEnableVertexAttribArray(3);

	glBindVertexArray(vao[SKY_BACK]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[SKY_BACK_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyVerticesBack), skyVerticesBack, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(skyVerticesBack[0]), 0);  //layout(location=2) in vec4 skyCoords;
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(skyVerticesBack[0]), (void*)(sizeof(skyVerticesBack[0].coords))); //layout(location=3) in vec2 skyTexCoords;
	glEnableVertexAttribArray(3);

	glBindVertexArray(vao[SKY_UP]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[SKY_UP_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyVerticesUp), skyVerticesUp, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(skyVerticesUp[0]), 0);  //layout(location=2) in vec4 skyCoords;
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(skyVerticesUp[0]), (void*)(sizeof(skyVerticesUp[0].coords))); //layout(location=3) in vec2 skyTexCoords;
	glEnableVertexAttribArray(3);


	//----------------------------------------------------------------Bind Racetrack

	glBindVertexArray(vao[TRACK]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[TRACK_VERTICES]);   // Bind vertex buffer 
	glBufferData(GL_ARRAY_BUFFER, sizeof(fVertex) * track.numVertices, track.v, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(fVertex), 0);  // coorindates////
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(fVertex), (void*)(sizeof(fVertex::coords)));  //texture coordinates
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(fVertex), (void*)(sizeof(fVertex::normals)));  //normals
	glEnableVertexAttribArray(5);

	//-----------------------------------------------------------------Bind FANS


	glBindVertexArray(vao[FANS]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[FANS_VERTICES]);   // Bind vertex buffer 
	glBufferData(GL_ARRAY_BUFFER, sizeof(fVertex) * fans.numVertices, fans.v, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(fVertex), 0);  // coorindates////
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(fVertex), (void*)(sizeof(fVertex::coords)));  //texture coordinates
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(fVertex), (void*)(sizeof(fVertex::normals)));  //normals
	glEnableVertexAttribArray(5);

	// Obtain projection matrix uniform location and set value.
	projMatLoc = glGetUniformLocation(programId, "projMat");   //uniform mat4 projMat;
	projMat = frustum(-5.0, 5.0, -5.0, 5.0, 3.0, 400.0);
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));

	// Obtain modelview matrix uniform and object uniform locations.
	modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");   //uniform mat4 modelViewMat;
	objectLoc = glGetUniformLocation(programId, "object");  //uniform uint object;

	// Load the images.
	image[0] = getbmp("./Textures/water.bmp");
	image[1] = getbmp("./Textures/sky3.bmp");
	image[2] = getbmp("./Textures/metalDots.bmp");
	image[3] = getbmp("./Textures/ground.bmp");
	// Create texture ids.	
	glGenTextures(4, texture);

	// ---------------------------------------WATER TEXTURE
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	grassTexLoc = glGetUniformLocation(programId, "grassTex");
	glUniform1i(grassTexLoc, 0);
		
	// --------------------------------------SKY TEXTURE
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[1]->sizeX, image[1]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[1]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	skyTexLoc = glGetUniformLocation(programId, "skyTex");
	glUniform1i(skyTexLoc, 1);

	// -------------------------------------HOVER TEXTURE

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[2]->sizeX, image[2]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[2]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	metalTexLoc = glGetUniformLocation(programId, "metalTex");
	glUniform1i(metalTexLoc, 2);


	//---------------------------------------TRACK TEXTURE

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[3]->sizeX, image[3]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[3]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	trackTexLoc = glGetUniformLocation(programId, "trackTex");
	glUniform1i(trackTexLoc, 3);


}

void GameEngine::UpdateGame()
{
	//cameraTarget = target->GetPosition();
	oldTimeSinceStart = newTimeSinceStart;
	newTimeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (newTimeSinceStart - oldTimeSinceStart);
	deltaTime /= 1000.f;
	hover.Update(deltaTime);//Updates hover's orientation, velocity and position 
	orient += length(hover.velocity) * 10;
	

	/*for (int i = 0; i < objects.size(); i++)
	{
		for (int j = 0; j < objects.size(); j++)
		{
			if (i == j)
				continue;
			else if ((*objects[i]->GetCollider()).CollideCheck((*objects[j]->GetCollider())))//checks if the collision happens
			{
				std::cout << "Collision";
				//colDatas.push_back(new CollisionData{objects[i],objects[j]});
			}
		}
	}*/

	if (hover.GetCollider()->CollideCheck(*cube1.GetCollider()))//check if the player collides with this obstacle
	{
		std::cout << "coillision" << endl;
		hover.collides(cube1.GetCollider());
	}
	else if (hover.GetCollider()->CollideCheck(*cube2.GetCollider()))
	{
		std::cout << "coillision" << endl;
		hover.collides(cube2.GetCollider());
	}
	else if (hover.GetCollider()->CollideCheck(*cube3.GetCollider()))
	{
		std::cout << "coillision" << endl;
		hover.collides(cube3.GetCollider());
	}
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(deltaTime);
	}
	

	cube1.Update(deltaTime);//Animating Cubes
	cube2.Update(deltaTime);
	cube3.Update(deltaTime);
	
	cameraEffect = length(hover.velocity) * 15;//With more speed the distance from eye to camera target increases
	glutPostRedisplay();
}

void GameEngine::DrawGame()
{	

	glm::mat4 currentView;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//press numButtons
	if(cameraMode == 1)
		currentView = glm::lookAt((hover.pos + hover.direction * 15.0f) + vec3(0 + cameraEffect, 10 + cameraEffect, 0 + cameraEffect), glm::vec3(0.0f, 10.0f, 0.0f) + hover.pos, glm::vec3(0.0f, 1.0f, 0.0f));//main camera
	else if (cameraMode == 2)
		currentView = glm::lookAt(glm::vec3(0.0f + f, 30.0f + d, 15.0f + t), glm::vec3(0.0f, 10.0f, 0.0f)+hover.pos, glm::vec3(0.0f, 1.0f, 0.0f));
	else if (cameraMode == 3)
		currentView = glm::lookAt(glm::vec3(hover.pos.x + 10.0f + cameraEffect, 5.0f + d + cameraEffect, 15.0f + hover.pos.z + cameraEffect), glm::vec3(0.0f, 10.0f, 0.0f) + hover.pos, glm::vec3(0.0f, 1.0f, 0.0f));
	else if (cameraMode == 4)
		currentView = glm::lookAt(glm::vec3(0.0f + cameraEffect, 10.0f + cameraEffect, 15.0f + cameraEffect) + hover.pos, glm::vec3(0.0f, 10.0f, 0.0f) + hover.pos, glm::vec3(0.0f, 1.0f, 0.0f));
	// Calculate and update modelview matrix.
	modelViewMat = mat4(1.0);
	modelViewMat = currentView;
	
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

	// -------------------------------------------------------FIELD
	glUniform1ui(objectLoc, FIELD);  //if (object == FIELD)
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBindVertexArray(vao[FIELD]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//Draw Sphere

	// -------------------------------------------------------SKY
	modelViewMat = mat4(1.0);
	modelViewMat = currentView;
	modelViewMat = translate(modelViewMat, vec3(0, 0, 0));
	modelViewMat = translate(modelViewMat, hover.pos);
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	glUniform1ui(objectLoc, SKY_FRONT);  //if (object == SKY_FRONT)
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBindVertexArray(vao[SKY_FRONT]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	glUniform1ui(objectLoc, SKY_BACK);  //if (object == SKY_BACK)
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBindVertexArray(vao[SKY_BACK]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	glUniform1ui(objectLoc, SKY_LEFT);  //if (object == SKY_LEFT)
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBindVertexArray(vao[SKY_LEFT]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	glUniform1ui(objectLoc, SKY_RIGHT);  //if (object == SKY_RIGHT)
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBindVertexArray(vao[SKY_RIGHT]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	glUniform1ui(objectLoc, SKY_UP);  //if (object == SKY_UP)
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBindVertexArray(vao[SKY_UP]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// ------------------------------------------------------RACETRACK

	modelViewMat = mat4(1.0);
	modelViewMat = currentView;
	modelViewMat = translate(modelViewMat, vec3(-22.0, 0.0, 0.0));
	vec3 scale1 = glm::vec3(1.5f, 1.5f, 1.5f);
	modelViewMat = glm::scale(modelViewMat, scale1);
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));  //send to the shader
	glUniform1ui(objectLoc, TRACK);  //if (object == TRACK)
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBindVertexArray(vao[TRACK]);
	glDrawArrays(GL_TRIANGLES, 0, track.numVertices);


	// -------------------------------------------------------DRAW FANS

	
	modelViewMat = mat4(1.0);
	modelViewMat = currentView;
	modelViewMat = translate(modelViewMat, hover.pos);
	modelViewMat = rotate(modelViewMat, radians(hover.rotationAngle), vec3(0, 1, 0));//totating in Y direction around origin

	modelViewMat = translate(modelViewMat, vec3(0, 7.0f, 4.0f));
	
	modelViewMat = rotate(modelViewMat, radians((float) 90), vec3(0, 0, 1));
	modelViewMat = rotate(modelViewMat, radians((float) 90), vec3(0, 1, 0));
	modelViewMat = rotate(modelViewMat, radians(orient), vec3(1, 0, 0));//rotating fan around its own centre

	
	//modelViewMat = translate(modelViewMat, vec3(0.0, 0.0, 0.0))
	//modelViewMat = rotate(modelViewMat, radians(hover.rotationAngle), vec3(0, 0, 1));
	vec3 scale2 = glm::vec3(0.2f, 0.2f, 0.2f);
	modelViewMat = glm::scale(modelViewMat, scale2);

	
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));  //send to the shader
	glUniform1ui(objectLoc, FANS);  //if (object == FANS)
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	modelViewMat = mat4(1.0);
	glBindVertexArray(vao[FANS]);
	glDrawArrays(GL_TRIANGLES, 0, track.numVertices);

	//Drawing hover and 3 cubes from the class objects

	hover.Draw(currentView);
	cubeColour(sphereFandB);
	cube1.Draw(currentView);
	cubeColour(cubeBlue);
	cube2.Draw(currentView);
	cubeColour(cubeRed);
	cube3.Draw(currentView);

	glutSwapBuffers();
}

void GameEngine::CleanupEngine()
{
	for (int i = 0; i < objects.size(); ++i)
	{
		delete objects[i];
	}

	/*for (int i = 0; i < colDatas.size(); ++i)
	{
		delete colDatas[i];
	}
	*/
}


void GameEngine::InitEngine(int argc, char** argv, const char* windowTitle, int width, int height)
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(windowTitle);
	glutDisplayFunc(DrawGame);
	glutReshapeFunc(ResizeWindow);
	glutIdleFunc(UpdateGame);

	glutKeyboardFunc([](unsigned char key, int x, int y)
		{
			GameObject::keys[key] = true;
			//If we press escape, quit

			switch (key) {
			
			case '1':
				cameraMode = 1;
				break;
			case '2':
				cameraMode = 2;
				break;
			case '3':
				cameraMode = 3;
			case '4':
				cameraMode = 4;
				break;
			case 'c':
				d += 1;
				break;       // and exits the switch
			case 'x':
				d -= 1;
				break;
			case 'b':
				f += 1;
			case 'v':
				f -= 1;
				break;
			case 'm':
				t += 1;
				break;
			case 'n':
				t -= 1;
				break;


			}
			if (key == 27)
			{
				CleanupEngine(); //changed to include the GameEngine function - this function will have to be static!
				exit(0);
			}
		}
	);
	glutKeyboardUpFunc([](unsigned char key, int x, int y)
		{
			GameObject::keys[key] = false;

			
		}
	);
	glutSpecialFunc([](int key, int x, int y)
		{
			GameObject::specialKeys[key] = true;
			if (key == 27)
			{
				CleanupEngine(); //changed to include the GameEngine function - this function will have to be static!
				exit(0);
			}
		}
	);
	glutSpecialUpFunc([](int key, int x, int y)
		{
			GameObject::specialKeys[key] = false;
		}
	);


	glewExperimental = GL_TRUE;
	glewInit();


}

GameEngine::GameEngine()
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::ResizeWindow(int w, int l)
{
	glViewport(0, 0, w, l);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)w / (float)l, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

void GameEngine::AddGameObject(GameObject* object)
{
	objects.push_back(object);
}

void GameEngine::StartEngine(void)
{
	std::cout << "Press escape to exit the game." << std::endl;
	glutMainLoop();
}
//Changes current material
void GameEngine::cubeColour(Material color) {
	//codes for OpenGL lighting SPHERE MATS
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.ambRefl"), 1, &color.ambRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.difRefl"), 1, &color.difRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.specRefl"), 1, &color.specRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.emitCols"), 1, &color.emitCols[0]);
	glUniform1f(glGetUniformLocation(programId, "sphereFandB.shininess"), color.shininess);
}