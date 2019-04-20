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
#include "OBJReader.h"

#define VERTEX_SHADER "Shaders/model_loading.vs"
#define FRAGMENT_SHADER "Shaders/model_loading.fs"
#define OBJ_MODEL "nanosuit/nanosuit.obj"

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

GLFWWrapper glfwWrapper;
GLEWWrapper glewWrapper;

int main() {
	glfwWrapper.initialize()
		.setFramebufferSizeCallback(onResize)
		.setCursorPositionCallback(onScroll)
		.setScrollCallback(onZoom);
	glewWrapper.initialize();

	Shader ourShader(VERTEX_SHADER, FRAGMENT_SHADER);
	OBJReader reader;
	MyMesh mesh = reader.read(OBJ_MODEL);
	mesh.setup(ourShader);

	while (!glfwWrapper.windowShouldClose()) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		onKeyPress();
		glewWrapper.clear();
		ourShader.useProgram();

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(ourShader.program, "projection"), 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(ourShader.program, "view"), 1, GL_FALSE, &view[0][0]);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); 
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	
		glUniformMatrix4fv(glGetUniformLocation(ourShader.program, "model"), 1, GL_FALSE, &model[0][0]);
		//ourModel.draw(ourShader);
		mesh.draw(ourShader);

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
	if (glfwWrapper.onKeyPress(D))
		camera.processKeyboard(LEFT, deltaTime);
	if (glfwWrapper.onKeyPress(A))
		camera.processKeyboard(RIGHT, deltaTime);
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