class MyMesh {
public:
	vector<glm::vec3*>* vertices;
	vector<glm::vec3*>* normals;
	vector<glm::vec2*>* textures;
	ModelData modelData;
	vector<Group*>* groups;
	string materialFile;
	
	MyMesh(glm::vec3* initialPosition) {
		vertices = new vector<glm::vec3*>();
		normals = new vector<glm::vec3*>();
		textures = new vector<glm::vec2*>();
		groups = new vector<Group*>();
		modelData.rotation = 0.0f;
		modelData.scale = 1.0f;
		modelData.translate = initialPosition;
	}

	MyMesh(vector<glm::vec3*>* vertices, vector<glm::vec3*>* normals, vector<glm::vec2*>* textures, vector<Group*>* groups, glm::vec3* initialPosition) {
		this->vertices = vertices;
		this->normals = normals;
		this->textures = textures;
		this->groups = groups;
		modelData.rotation = 0.0f;
		modelData.scale = 1.0f;
		modelData.translate = initialPosition;
	}
	
	void draw(Shader shader) {
		for (vector<Group*>::iterator group = groups->begin(); group != groups->end(); ++group) {
			glBindVertexArray((*group)->vao.vao);
			glActiveTexture(GL_TEXTURE0 + (*group)->material->id);
			int textureLocation = shader.uniform("texture_diffuse1");
			glUniform1i(textureLocation, (*group)->material->id);
			glBindTexture(GL_TEXTURE_2D, (*group)->material->id);
			glDrawArrays(GL_TRIANGLES, 0, (*group)->faces->size() * 3);
		}
	}

	void setup(Shader shader) {
		for (vector<Group*>::iterator group = groups->begin(); group != groups->end(); ++group) {
			vector<GLfloat> finalVector;
			for (vector<Face*>::iterator face = (*group)->faces->begin(); face != (*group)->faces->end(); ++face) {
				for (
					int vIndex = 0, tIndex = 0, nIndex = 0;
					vIndex < (*face)->vertices.size();
					vIndex++, tIndex++, nIndex++
					) {
					auto index = (*face)->vertices[vIndex];
					finalVector.push_back(vertices->at(index)->x);
					finalVector.push_back(vertices->at(index)->y);
					finalVector.push_back(vertices->at(index)->z);

					index = (*face)->textures[tIndex];
					finalVector.push_back(textures->at(index)->x);
					finalVector.push_back(textures->at(index)->y);

					index = (*face)->normals[nIndex];
					finalVector.push_back(normals->at(index)->x);
					finalVector.push_back(normals->at(index)->y);
					finalVector.push_back(normals->at(index)->z);
				}
			}
			(*group)->setup(finalVector, shader);
		}
	}
};