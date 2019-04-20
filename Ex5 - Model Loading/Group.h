class Group {
public:
	vector<Face> faces;
	Material material;
	string name;
	VAO vao;
	VBO vbo;

	void setup(vector<GLfloat> finalVector, Shader shader) {
		unsigned int texture;
		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0 + material.id);
		glBindTexture(GL_TEXTURE_2D, material.id);

		setTextureIntoShader(shader);
		setMaterialIntoShader(shader);
		
		glBindVertexArray(vao.vao);

		vbo.bind(finalVector);
		vao.bind(0, 3, 0);
		vao.bind(1, 2, 3);
		vao.bind(2, 3, 5);

		glBindVertexArray(0);
	}

private:
	void setTextureIntoShader(Shader &shader) {
		int textureLocation = shader.uniform("texture");
		glUniform1i(textureLocation, material.id);
	}

	void setMaterialIntoShader(Shader &shader) {
		glm::vec3 Ka = material.ka;
		int KaLocation = shader.uniform("Ka");
		if (Ka.r == 0 && Ka.g == 0 && Ka.b == 0)
			glUniform3f(KaLocation, 1.0, 1.0, 1.0);
		else
			glUniform3f(KaLocation, Ka.r, Ka.g, Ka.b);

		glm::vec3 Kd = material.kd;
		int KdLocation = shader.uniform("Kd");
		if (Kd.r == 0 && Kd.g == 0 && Kd.b == 0)
			glUniform3f(KdLocation, 1.0, 0.5, 0.0);
		else
			glUniform3f(KdLocation, Kd.r, Kd.g, Kd.b);

		glm::vec3 Ks = material.ks;
		int KsLocation = shader.uniform("Ks");
		if (Ks.r == 0 && Ks.g == 0 && Ks.b == 0)
			glUniform3f(KsLocation, 1.0, 1.0, 1.0);
		else
			glUniform3f(KsLocation, Ks.r, Ks.g, Ks.b);

		float Ns = material.ns;
		int NsLocation = shader.uniform("Ns");
		if (Ns == 0)
			glUniform1f(NsLocation, 100);
		else
			glUniform1f(NsLocation, Ns);
	}
};
