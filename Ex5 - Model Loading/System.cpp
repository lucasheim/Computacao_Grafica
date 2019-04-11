#include "System.h"

float lastX = 400;
float lastY = 300;
float yaw = -90.0f;
float pitch = 0;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
bool firstMouse = true;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
};

class Mesh {
	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
		void Draw(Shader shader);
	private:
		unsigned int VAO, VBO, EBO;
		void setupMesh();

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		setupMesh();
	}

	void setupMesh() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}

	void Draw(Shader shader)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
			// retrieve texture number (the N in diffuse_textureN)
			std::string number;
			std::string name = textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);

			shader.setFloat(("material." + name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);

		// draw mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
};

class Model
{
public:
	/*  Functions   */
	Model(char *path)
	{
		loadModel(path);
	}
	void Draw(Shader shader);
private:
	/*  Model Data  */
	std::vector<Mesh> meshes;
	std::string directory;
	/*  Functions   */
	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
		std::string typeName);

	void Draw(Shader shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
			meshes[i].Draw(shader);
	}


};


System::~System()
{
}

void System::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
}

int System::GLFWInit()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(WIDTH, HEIGHT, "", nullptr, nullptr);

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (window == nullptr) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed no init GLEW." << std::endl;
		return EXIT_FAILURE;
	}

	glfwSetCursorPosCallback(window, this->mouseCallback);

	glViewport(0, 0, screenWidth, screenHeight);

	return EXIT_SUCCESS;
}

int System::OpenGLSetup()
{
	glEnable(GL_BLEND);	// Enables blending ( glBlendFunc )
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return EXIT_SUCCESS;
}


int System::SystemSetup()
{
	coreShader = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag");
	coreShader.use();

	return EXIT_SUCCESS;
}

void System::Run()
{
	coreShader.use();

	GLfloat vertices[] =
	{
		// Positions         // Colors
		 0.0f,  0.5f, -0.5f,	  1.0f, 0.0f, 0.0f, // Top			 (RED)
		 0.5f, -0.5f,  0.0f,	  0.0f, 1.0f, 0.0f, // Bottom middle (GREEN)
		-0.5f, -0.5f,  0.0f,	  0.0f, 0.0f, 1.0f, // Bottom left	 (BLUE)

		  0.0f,   0.5f, -0.5f,    1.0f, 0.0f, 0.0f, // Top			 (RED)
		  0.0f,  -0.5f, -1.0f,    0.0f, 0.0f, 1.0f, // Back			 (BLUE)
		  0.5f,  -0.5f,  0.0f,    0.0f, 1.0f, 0.0f, // Bottom middle (GREEN)

		  0.0f,   0.5f, -0.5f,    1.0f, 0.0f, 0.0f, // Top			 (RED)
		 -0.5f,  -0.5f,  0.0f,    0.0f, 0.0f, 1.0f, // Bottom left	 (BLUE)
		  0.0f,  -0.5f, -1.0f,    0.0f, 1.0f, 0.0f, // Back			 (BLUE)

	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO


	float cameraSpeed;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;


	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		cameraSpeed = 2.5f * deltaTime;
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

		int modelLoc, viewLoc, projLoc;
		modelLoc = glGetUniformLocation(coreShader.ID, "model");
		viewLoc = glGetUniformLocation(coreShader.ID, "view");
		projLoc = glGetUniformLocation(coreShader.ID, "proj");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(perspective));
		
		this->ShowFPS(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			cameraPos += cameraSpeed * cameraFront;
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			cameraPos -= cameraSpeed * cameraFront;
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		coreShader.use();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 9);
		glBindVertexArray(0);


		glfwSwapBuffers(window);
	}


}

void System::ShowFPS(GLFWwindow* window)
{
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime(); // returns number of seconds since GLFW started, as double float

	elapsedSeconds = currentSeconds - previousSeconds;

	// Limit text updates to 4 times per second
	if (elapsedSeconds > 0.25)
	{
		previousSeconds = currentSeconds;
		double fps = (double)frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		// The C++ way of setting the window title
		std::ostringstream outs;
		outs.precision(3);	// decimal places
		outs << std::fixed
			<< "Ex. 1" << "    "
			<< "FPS: " << fps << "    ";
		glfwSetWindowTitle(window, outs.str().c_str());

		// Reset for next average.
		frameCount = 0;
	}

	frameCount++;
}

void System::Finish()
{
	glfwTerminate();
}
