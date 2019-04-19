#include <GLFW/glfw3.h>

#define WINDOW_TITLE "Hello Triangle!"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

enum Key {
	ESCAPE = GLFW_KEY_ESCAPE,
	W = GLFW_KEY_W,
	S = GLFW_KEY_S,
	A = GLFW_KEY_A,
	D = GLFW_KEY_D
};

class GLFWWrapper {
public:
	GLFWWrapper initialize() {
		initializeGLFW();
		setWindowHints();
		createWindow();
		setEnableVariables();
		return *this;
	}

	GLFWWrapper setFramebufferSizeCallback(GLFWframebuffersizefun callback) {
		glfwSetFramebufferSizeCallback(window, callback);
		return *this;
	}

	GLFWWrapper setCursorPositionCallback(GLFWcursorposfun callback) {
		glfwSetCursorPosCallback(window, callback);
		return *this;
	}

	GLFWWrapper setScrollCallback(GLFWscrollfun callback) {
		glfwSetScrollCallback(window, callback);
		return *this;
	}

	int windowShouldClose() {
		return glfwWindowShouldClose(window);
	}

	bool onKeyPress(int key) {
		return glfwGetKey(window, key) == GLFW_PRESS;
	}

	void swapBuffers() {
		glfwSwapBuffers(window);
	}

	void terminate() {
		glfwSetWindowShouldClose(window, true);
		glfwTerminate();
	}

private:
	GLFWwindow *window;

	void setWindowHints() {
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
	}

	void createWindow() {
		window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
		glfwMakeContextCurrent(window);
	}

	void initializeGLFW() {
		if (!glfwInit()) {
			std::cout << "GLFW init failed! \n";
		}
	}

	void setEnableVariables() {
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
};
