#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "player.h"
//#include "block.h"
#include "chunk.h"
#include <X11/Xlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <vector>
#include <iostream>

using namespace std;

void render();
void update();
void resize(int width, int height);
void keydown(unsigned char key, int mouseX, int mouseY);
void keyup(unsigned char key, int mouseX, int mouseY);
void mouseControl(int x, int y);

Mesh skybox;
Shader shader;
Texture sky;
Texture block_textures;
Chunk test;

Transform transform;
Player player(glm::vec3(0, 0, -5));

int last_time = 0;
int* last = &last_time;

Display* disp = XOpenDisplay(NULL);
Screen* scrn = DefaultScreenOfDisplay(disp);
int height = scrn->height;
int width = scrn->width;

#define WIDTH width
#define HEIGHT height

int texWidth = 5;
int texHeight = 5;



int main(int argc,char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	//glutInitWindowSize(width, height);
	glutInitWindowSize(640, 480);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("OpenGl");
	//glutFullScreen();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	glewExperimental = GL_TRUE;
	glewInit();

	
	skybox.Set("./res/models/skybox.obj");
	test.SetMesh();
	
	shader.Set("./res/shaders/basicShader");
	sky.Set("./res/images/skyimg.jpg");
	block_textures.Set("./res/images/BlockTextures.jpg");
	
	player.SetCam(player.GetPos(), 70, (float)glutGet(GLUT_WINDOW_WIDTH)/glutGet(GLUT_WINDOW_HEIGHT), 0.1, 100);
	
	
	glutDisplayFunc(render);
	glutIdleFunc(update);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
	glutPassiveMotionFunc(mouseControl);
	glutSetCursor(GLUT_CURSOR_NONE);
	
	glutMainLoop();
	
	return 0;
}

void drawSky(Camera& playerCam) {
	Camera temp = Camera(glm::vec3(0, 0, 0), 70, (float)glutGet(GLUT_WINDOW_WIDTH)/glutGet(GLUT_WINDOW_HEIGHT), 0.1, 100);
	temp.SetForward(playerCam.GetForward());
	temp.SetUp(playerCam.GetUp());
	shader.Update(temp);
	sky.Bind(0, shader.GetProgram(), true);
	skybox.Draw();
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Camera& playerCam = player.GetCam();
	
	shader.Bind();
	shader.Update(playerCam);
	shader.UpdateLighting(playerCam);
	
	drawSky(playerCam);
	block_textures.Bind(0, shader.GetProgram());
	shader.Update(playerCam);
	test.GetMesh()->Draw();
	
	glFinish();
	glutSwapBuffers();
} // end render

void update() {
	int current_time = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = current_time - *last;
	*last = current_time;
	
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
	if(key == ' ') { // space
		player.GetPos().y += 0.5;
	}
	if(key == 'z' || key == 'Z') {
		player.GetPos().y -= 0.5;
	}
	if(key == 27) { // escape
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

