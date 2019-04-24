#include "Mtl.h"


MtlLib::MtlLib(std::string directory, std::string mtlName) {
    std::ifstream mtlFile(directory + "/" + mtlName);
    std::string line;
    
    Mtl* currentMtl = NULL;
    try {
        while(std::getline(mtlFile, line)) {
            std::stringstream lineStream(line);
            std::string identifier;
            lineStream >> identifier;
            if (identifier == "newmtl") {
                std::string mtlName;
                lineStream >> mtlName;
                currentMtl = &this->mtls[mtlName];
            } else if (identifier == "Ka") {
                glm::dvec3 ka;
                lineStream >> ka.r >> ka.g >> ka.b;
                currentMtl->ambientLightColor = ka;
            } else if (identifier == "Kd") {
                glm::dvec3 kd;
                lineStream >> kd.r >> kd.g >> kd.b;
                currentMtl->difuseLightColor = kd;
            } else if (identifier == "Ks") {
                glm::dvec3 ks;
                lineStream >> ks.r >> ks.g >> ks.b;
                currentMtl->specularLightColor = ks;
            } else if (identifier == "d") {
                lineStream >> currentMtl->d;
            } else if (identifier == "map_Kd") {
                std::string textureFileName;
                lineStream >> textureFileName;
                currentMtl->loadTexture(directory + "/" + textureFileName);
            }
        }
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::MTL::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
}

Mtl MtlLib::getMtlByName(std::string name) {
    return this->mtls[name];
}

void Mtl::loadTexture(std::string path) {
	int width, height, nrComponents;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    this->textId = textureId;
	stbi_image_free(data);
}

int Mtl::getTextureIndex() {
    return this->textId;
}
