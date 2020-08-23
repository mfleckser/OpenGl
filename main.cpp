#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "player.h"
#include "block.h"
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <iostream>

using namespace std;

void render();
void update();
void resize(int width, int height);
void keydown(unsigned char key, int mouseX, int mouseY);
void keyup(unsigned char key, int mouseX, int mouseY);
void mouseControl(int x, int y);
void renderBitmapString(float x, float y, void *font,const char *string);

Mesh mesh;
Mesh ground;
Mesh skybox;
Mesh* testBlock;
Shader shader;
Texture crate;
Texture bricks;
Texture sky;
Block test;

Transform transform;
Player player(glm::vec3(0, 0, -5));

int last_time = 0;
int* last = &last_time;
int most = 0;

int main(int argc,char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1920, 1080);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("Open GL");
	glutFullScreen();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	glewExperimental = GL_TRUE;
	glewInit();
	
	//Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices
	
	Vertex vertices[4] = {
		Vertex(glm::vec3(-10,-2,-10), glm::vec2(0,0)),
		Vertex(glm::vec3(10,-2,-10), glm::vec2(10,0)),
		Vertex(glm::vec3(10,-2,10), glm::vec2(10,10)),
		Vertex(glm::vec3(-10,-2,10), glm::vec2(0,10))
	};
	
	unsigned int indices[6] = { 0, 1, 2, 0, 2, 3 };
	
	unsigned short int texIndicies[6] = { 0, 2, 1, 1, 1, 1 };
	
	mesh.Set("./res/models/box.obj");
	ground.Set(vertices, 4, indices, 6);
	skybox.Set("./res/models/skybox.obj");
	test.Set(glm::vec3(0, 0, 0), texIndicies);
	testBlock = test.GetMesh(1, 3);
	
	shader.Set("./res/shaders/basicShader");
	
	crate.Set("./res/images/crate_1.jpg");
	bricks.Set("./res/images/bricks.jpg");
	sky.Set("./res/images/skyimg.jpg");
	
	player.SetCam(player.GetPos(), 70, (float)glutGet(GLUT_WINDOW_WIDTH)/glutGet(GLUT_WINDOW_HEIGHT), 0.1, 100);
	
	
	glutDisplayFunc(render);
	glutIdleFunc(update);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
	glutPassiveMotionFunc(mouseControl);
	glutSetCursor(GLUT_CURSOR_NONE);
	//~ GLfloat c[3] = {0.8,0.8,0.8};
	//~ glClearColor(c[0], c[1], c[2], 1);
	
	glutMainLoop();
	
	return 0;
}

void drawCrate() {
	crate.Bind(0, shader.GetProgram());
	mesh.Draw();
}

void drawGround() {
	bricks.Bind(0, shader.GetProgram(), false, true);
	ground.Draw();
}

void drawSky() {
	sky.Bind(0, shader.GetProgram(), true);
	skybox.Draw();
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Camera& playerCam = player.GetCam();
	
	shader.Bind();
	shader.Update(playerCam);
	shader.UpdateLighting(playerCam);
	
	//drawCrate();
	drawGround();
	Camera temp = Camera(glm::vec3(0, 0, 0), 70, (float)glutGet(GLUT_WINDOW_WIDTH)/glutGet(GLUT_WINDOW_HEIGHT), 0.1, 100);
	temp.SetForward(playerCam.GetForward());
	temp.SetUp(playerCam.GetUp());
	shader.Update(temp);
	drawSky();
	sky.Bind(0, shader.GetProgram());
	shader.Update(playerCam);
	testBlock->Draw();
	
	glFinish();
	glutSwapBuffers();
} // end render

void update() {
	int current_time = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = current_time - *last;
	*last = current_time;
	//std::cout << deltaTime << std::endl;
	usleep((float)(30-deltaTime)*1000);
	
	player.update(deltaTime);
	
	glutPostRedisplay();
}

void resize(int width, int height) {
	player.GetCam().SetPerspective(70, (float)width/height, 0.1, 100);
}

void keydown(unsigned char key, int mouseX, int mouseY) {
	if(key == 'w' || key == 'W') {
		player.GetVel().z = 1;
	}
	if(key == 's' || key == 'S') {
		player.GetVel().z = -1;
	}
	if(key == 'a' || key == 'A') {
		player.GetVel().x = -1;
	}
	if(key == 'd' || key == 'D') {
		player.GetVel().x = 1;
	}
	if(key == 'q' || key == 'Q') {
		player.GetPos().y += 0.1;
	}
	if(key == 'z' || key == 'Z') {
		player.GetPos().y -= 0.1;
	}
	if(key == 27) {
		exit(0);
	}
}

void keyup(unsigned char key, int mouseX, int mouseY) {
	if(key == 'a' || key == 'A') {
		player.GetVel().x = 0;
	}
	if(key == 'd' || key == 'D') {
		player.GetVel().x = 0;
	}
	if(key == 'w' || key == 'W') {
		player.GetVel().z = 0;
	}
	if(key == 's' || key == 'S') {
		player.GetVel().z = 0;
	}
}
void mouseControl(int mouseX, int mouseY) {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	
	float xoff = ((float)mouseX - (float)(width/2));
	float yoff = ((float)mouseY - (float)(height/2));
	
	player.GetYaw() += xoff * 0.05;
	player.GetPitch() -= yoff * 0.05;
	
	if(player.GetPitch() > 89) {
		player.GetPitch() = 89;
	} else if(player.GetPitch() < -89) {
		player.GetPitch() = -89;
	}
	
	glm::vec3 direction;
	direction.x = cos(glm::radians(player.GetYaw())) * cos(glm::radians(player.GetPitch()));
	direction.y = sin(glm::radians(player.GetPitch()));
	direction.z = sin(glm::radians(player.GetYaw())) * cos(glm::radians(player.GetPitch()));
	player.GetLooking() = glm::normalize(direction);
	
	if(xoff != 0 && yoff != 0) {
		glutWarpPointer(width / 2,height / 2);
	}
}
