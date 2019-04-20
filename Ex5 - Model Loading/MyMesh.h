
class MyMesh {
public:
	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	vector<glm::vec2> textures;
	vector<Group> groups;

	void draw(Shader shader) {
		for (Group group : groups) {
			glBindVertexArray(group.vao.vao);
			int textureLocation = shader.uniform("texture");
			glEnable(GL_TEXTURE_2D);
			glUniform1i(textureLocation, group.material.id);
			glBindTexture(GL_TEXTURE_2D, group.material.id);
			glDrawArrays(GL_TRIANGLES, 0, group.faces.size() * 3);
			glDisable(GL_TEXTURE_2D);
		}
	}

	void setup(Shader shader) {
		for (Group group : groups) {
			vector<GLfloat> finalVector;
			for (Face face : group.faces) {
				for (
					int vertex = 0, normal = 0, texture = 0;
					vertex < face.vertices.size();
					vertex++, normal++, texture++
				) {
					finalVector.push_back(vertices[vertex].x);
					finalVector.push_back(vertices[vertex].y);
					finalVector.push_back(vertices[vertex].z);

					finalVector.push_back(textures[texture].x);
					finalVector.push_back(textures[texture].y);

					finalVector.push_back(normals[normal].x);
					finalVector.push_back(normals[normal].y);
					finalVector.push_back(normals[normal].z);
				}
			}
			group.setup(finalVector, shader);
		}
	}
};