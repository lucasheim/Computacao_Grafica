#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "GLFWWrapper.h"
#include "GLEWWrapper.h"
#include "VBO.h"
#include "VAO.h"
#include "Material.h"
#include "Face.h"
#include "Group.h"
#include "MyMesh.h"
#include "Inserter.h"
#include "OBJReader.h"
#include "Model.h"

#define VERTEX_SHADER "Shaders/model_loading.vs"
#define FRAGMENT_SHADER "Shaders/model_loading.fs"
#define OBJ_BANANA "banana/banana.obj"
#define OBJ_NANOSUIT "nanosuit/nanosuit.obj"
#define OBJ_STATUE "LibertStatue/LibertStatue.obj"
#define OBJ_TABLE "table/table.obj"

void onResize(GLFWwindow* window, int width, int height);
void onScroll(GLFWwindow* window, double xpos, double ypos);
void onZoom(GLFWwindow* window, double xoffset, double yoffset);
void onKeyPress();

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float translation = -1.0f;

GLFWWrapper glfwWrapper;
GLEWWrapper glewWrapper;

vector<Model> objects;
int selectedObject = -1;

int main() {
	glfwWrapper.initialize()
		.setFramebufferSizeCallback(onResize)
		.setCursorPositionCallback(onScroll)
		.setScrollCallback(onZoom);
	glewWrapper.initialize();

	Shader ourShader(VERTEX_SHADER, FRAGMENT_SHADER);
	//OBJReader reader;
	//MyMesh mesh = reader.read(OBJ_MODEL, ourShader);
	//mesh.setup(ourShader);
	Model banana(OBJ_BANANA);
	objects.push_back(banana);
	Model nanosuit(OBJ_NANOSUIT);
	objects.push_back(nanosuit);
	Model statue(OBJ_STATUE);
	objects.push_back(statue);
	Model table(OBJ_TABLE);
	objects.push_back(table);

	while (!glfwWrapper.windowShouldClose()) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		onKeyPress();
		glewWrapper.clear();
		ourShader.useProgram();

		for (int i = 0; i < objects.size(); i++) {
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.getViewMatrix();
			glUniformMatrix4fv(glGetUniformLocation(ourShader.program, "projection"), 1, GL_FALSE, &projection[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(ourShader.program, "view"), 1, GL_FALSE, &view[0][0]);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(objects[i].modelData.xTranslate, objects[i].modelData.yTranslate, objects[i].modelData.zTranslate));
			model = glm::rotate(model, glm::radians(objects[i].modelData.rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(objects[i].modelData.scale, objects[i].modelData.scale, objects[i].modelData.scale));
			glUniformMatrix4fv(glGetUniformLocation(ourShader.program, "model"), 1, GL_FALSE, &model[0][0]);
			//mesh.draw(ourShader);
			objects[i].draw(ourShader);
		}

		glfwWrapper.swapBuffers();
		glfwPollEvents();
	}

	return 0;
}

void onKeyPress() {
	if (glfwWrapper.onKeyPress(ESCAPE))
		glfwWrapper.terminate();

	if (glfwWrapper.onKeyPress(W))
		camera.processKeyboard(FORWARD, deltaTime);
	if (glfwWrapper.onKeyPress(S))
		camera.processKeyboard(BACKWARD, deltaTime);
	if (glfwWrapper.onKeyPress(A))
		camera.processKeyboard(LEFT, deltaTime);
	if (glfwWrapper.onKeyPress(D))
		camera.processKeyboard(RIGHT, deltaTime);
	if (glfwWrapper.onKeyPress(ZERO))
		selectedObject = 0;
	if (glfwWrapper.onKeyPress(ONE))
		selectedObject = 1;
	if (glfwWrapper.onKeyPress(TWO))
		selectedObject = 2;
	if (glfwWrapper.onKeyPress(THREE))
		selectedObject = 3;
	if (glfwWrapper.onKeyPress(FOUR))
		selectedObject = 4;
	if (glfwWrapper.onKeyPress(FIVE))
		selectedObject = 5;
	if (glfwWrapper.onKeyPress(SIX))
		selectedObject = 6;
	if (glfwWrapper.onKeyPress(SEVEN))
		selectedObject = 7;
	if (glfwWrapper.onKeyPress(EIGHT))
		selectedObject = 8;
	if (glfwWrapper.onKeyPress(NINE))
		selectedObject = 9;

	if (glfwWrapper.onKeyPress(R))
		objects[selectedObject].modelData.rotation += 0.05f;
	if (glfwWrapper.onKeyPress(T))
		objects[selectedObject].modelData.rotation -= 0.05f;
	if (glfwWrapper.onKeyPress(F))
		objects[selectedObject].modelData.scale += 0.005f;
	if (glfwWrapper.onKeyPress(G))
		if (objects[selectedObject].modelData.scale > 0) {
			objects[selectedObject].modelData.scale -= 0.005f;
		}
	if (glfwWrapper.onKeyPress(V))
		objects[selectedObject].modelData.xTranslate += 0.05f;
	if (glfwWrapper.onKeyPress(B))
		objects[selectedObject].modelData.xTranslate -= 0.05f;
}

void onResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void onScroll(GLFWwindow* window, double xPosition, double yPosition) {
	if (firstMouse) {
		lastX = xPosition;
		lastY = yPosition;
		firstMouse = false;
	}

	float xOffset = xPosition - lastX;
	float yOffset = lastY - yPosition; 

	lastX = xPosition;
	lastY = yPosition;

	camera.processMouseScroll(xOffset, yOffset);
}

void onZoom(GLFWwindow* window, double xoffset, double yoffset) {
	camera.processMouseZoom(yoffset);
}