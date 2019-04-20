class VAO {
public:
	GLuint vao = 0;

	VAO() {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
	}

	void bindTextures(VBO textures) {
		glBindBuffer(GL_ARRAY_BUFFER, textures.vbo);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	}

	void bindNormals(VBO normals) {
		glBindBuffer(GL_ARRAY_BUFFER, normals.vbo);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}

	void bindVertices(VBO positions) {
		glBindBuffer(GL_ARRAY_BUFFER, positions.vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}

	void bind(int start, int size, int stride) {
		glEnableVertexAttribArray(start);
		glVertexAttribPointer(start, size, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), NULL);
	}
};