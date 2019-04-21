class VBO {
public: 
	GLuint vbo = 0;

	VBO() {
		glGenBuffers(1, &vbo);
	}
	
	void bind(vector<glm::vec3> points) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);
	}
	
	void bind(vector<glm::vec2> points) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec2), points.data(), GL_STATIC_DRAW);
	}

	void bind(vector<GLfloat> points) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * points.size(), &points[0], GL_STATIC_DRAW);
	}
};