
class MyMesh {
public:
	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	vector<glm::vec2> textures;
	vector<Group> groups;

	void draw(Shader shader) {
		for (Group group : groups) {
			//This will fucking kill performance as we build finalVector each time we draw
			this->setup(group, shader);
			glBindVertexArray(group.vao.vao);
			glActiveTexture(GL_TEXTURE0 + group.material.id);
			int textureLocation = shader.uniform("texture");
			glUniform1i(textureLocation, group.material.id);
			glBindTexture(GL_TEXTURE_2D, group.material.id);
			glDrawArrays(GL_TRIANGLES, 0, group.faces.size() * 3);
		}
	}

	void setup(Group group, Shader shader) {
		vector<GLfloat> finalVector;
		for (Face face : group.faces) {
			for (
				int vIndex = 0, tIndex = 0, nIndex = 0;
				vIndex < face.vertices.size();
				vIndex++, tIndex++, nIndex++
				) {
				auto index = face.vertices[vIndex];
				finalVector.push_back(vertices[index].x);
				finalVector.push_back(vertices[index].y);
				finalVector.push_back(vertices[index].z);

				index = face.textures[tIndex];
				finalVector.push_back(textures[index].x);
				finalVector.push_back(textures[index].y);

				index = face.normals[nIndex];
				finalVector.push_back(normals[index].x);
				finalVector.push_back(normals[index].y);
				finalVector.push_back(normals[index].z);
			}
		}
		group.setup(finalVector, shader);
	}
};