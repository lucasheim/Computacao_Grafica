#ifndef MTL_H
#define MTL_H

#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <GL\glew.h>
#include "stb_image.h"

class Mtl {
public:
	glm::vec3 ambientLightColor;
	glm::vec3 difuseLightColor;
	glm::vec3 specularLightColor;
    int Ns;
    int d;
    void loadTexture(std::string path);
    int getTextureIndex();
private:
    int textId;
};

class MtlLib {
public:
    MtlLib(std::string directory, std::string mtlName);
    Mtl getMtlByName(std::string name);
private:
    std::map<std::string, Mtl> mtls;
};
  
#endif
