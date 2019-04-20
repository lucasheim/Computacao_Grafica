
class MyMesh {
public:
	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	vector<glm::vec2> textures;
	vector<Group> groups;

	MyMesh() {
		setupMesh();
	}

	void draw(Shader shader) {
		for (Group group : groups) {
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, group.material.id);
			
			for (Face face : group.faces) {
				glBindVertexArray(vao.vao);
				glDrawElements(GL_TRIANGLES, face.vertices.size(), GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
			glActiveTexture(GL_TEXTURE0);
		}
	}

private:
	VAO vao;
	VBO vboNormals, vboVertices, vboTextures;
	
	void setupMesh() {
		vboVertices.bind(vertices);
		vboNormals.bind(normals);
		vboTextures.bind(textures);

		vao.bindVertices(vboVertices);
		vao.bindNormals(vboNormals);
		vao.bindTextures(vboTextures);

		glBindVertexArray(0);
	}
};