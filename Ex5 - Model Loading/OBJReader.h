
class OBJReader {
public:
	MyMesh read(string const &path) {
		getDirectoryFromPath(path);
		loadOBJ(path);
		loadTextures();
		return mesh;
	}

private:
	string directory, materialFile;
	MyMesh mesh;
	Group group;

	void getDirectoryFromPath(string const &path) {
		directory = path.substr(0, path.find_last_of('/'));
	}

	void loadOBJ(string const &path) {
		ifstream file(path);
		while (!file.eof()) {
			string line, current;
			stringstream sline;
			getline(file, line);
			sline << line;
			sline >> current;
			if (current == "v") {
				insertVertex(sline);
			} else if (current == "vn") {
				insertNormal(sline);
			} else if (current == "vt") {
				insertTexture(sline);
			} else if (current == "g" || current == "usemtl") {
				insertGroup(sline);
			} else if (current == "f") {
				insertFaces(sline);
			} else if (current == "mtllib") {
				insertMaterialFile(sline);
			}
		}
	}

	void loadTextures() {
		ifstream file(directory + '/' + materialFile);
		string materialName;
		float ns;
		unsigned int id;
		glm::vec3 ka, kd, ks;
		while (!file.eof()) {
			string line, current;
			stringstream sline;
			getline(file, line);
			sline << line;
			sline >> current;
			if (current == "newmtl") {
				sline >> materialName;
			} else if (current == "map_Kd") {
				string path;
				sline >> path;
				id = loadTexturesFromFile(path, directory);
				insertInformationIntoMaterial(id, ka, kd, ks, ns, materialName);
			} else if (current == "Ka") {
				sline >> ka.x >> ka.y >> ka.z;
			} else if (current == "Kd") {
				sline >> kd.x >> kd.y >> kd.z;
			} else if (current == "Ks") {
				sline >> ks.x >> ks.y >> ks.z;
			} else if (current == "Ns") {
				sline >> ns;
			}
		}
	}

	void insertInformationIntoMaterial(unsigned int id, glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, float ns, string &materialName) {
		for (unsigned int i = 0; i < mesh.groups.size(); i++) {
			if (mesh.groups[i].material.name == materialName) {
				mesh.groups[i].material.id = id;
				mesh.groups[i].material.ka = ka;
				mesh.groups[i].material.kd = kd;
				mesh.groups[i].material.ks = ks;
				mesh.groups[i].material.ns = ns;
			}
		}
	}

	void insertMaterialFile(std::stringstream &sline) {
		sline >> materialFile;
	}

	void insertGroup(std::stringstream &sline) {
		string token;
		sline >> token;
		group.name = token;
		group.material.name = token;
		mesh.groups.push_back(group);
	}

	void insertFaces(std::stringstream &sline) {
		Face face;
		string token;
		sline >> token;
		insertFace(face, token);
		sline >> token;
		insertFace(face, token);
		sline >> token;
		insertFace(face, token);
		mesh.groups.back().faces.push_back(face);
	}

	void insertFace(Face &face, string token) {
		string current;
		stringstream stoken(token);
		getline(stoken, current, '/');
		face.vertices.push_back(std::stoi(current) - 1);
		getline(stoken, current, '/');
		face.textures.push_back(std::stoi(current) - 1);
		getline(stoken, current, '/');
		face.normals.push_back(std::stoi(current) - 1);
	}

	void insertVertex(std::stringstream &sline) {
		float x, y, z;
		sline >> x >> y >> z;
		glm::vec3 vertex(x, y, z);
		mesh.vertices.push_back(vertex);
	}

	void insertNormal(std::stringstream &sline) {
		float x, y, z;
		sline >> x >> y >> z;
		glm::vec3 vertex(x, y, z);
		mesh.normals.push_back(vertex);
	}

	void insertTexture(std::stringstream &sline) {
		float x, y;
		sline >> x >> y;
		glm::vec2 vertex(x, y);
		mesh.textures.push_back(vertex);
	}

	unsigned int loadTexturesFromFile(string &filename, const string &directory) {
		filename = directory + '/' + filename;

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

		if (data) {
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else {
			std::cout << "Texture failed to load at path: " << filename << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}
};