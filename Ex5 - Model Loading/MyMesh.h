
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
			int textureLocation = shader.uniform("texture");
			glEnable(GL_TEXTURE_2D);
			glUniform1i(textureLocation, group.material.id);
			glBindTexture(GL_TEXTURE_2D, group.material.id);
			glDrawArrays(GL_TRIANGLES, 0, group.faces.size() * 3);
			glDisable(GL_TEXTURE_2D);
		}
	}

	void setup(Group group, Shader shader) {
		vector<GLfloat> finalVector;
		for (Face face : group.faces) {
			finalVector.push_back(vertices[face.vertices[0]].x);
			finalVector.push_back(vertices[face.vertices[1]].y);
			finalVector.push_back(vertices[face.vertices[2]].z);

			finalVector.push_back(textures[face.textures[0]].x);
			finalVector.push_back(textures[face.textures[1]].y);

			finalVector.push_back(normals[face.normals[0]].x);
			finalVector.push_back(normals[face.normals[1]].y);
			finalVector.push_back(normals[face.normals[2]].z);
		}
		group.setup(finalVector, shader);
	}
};