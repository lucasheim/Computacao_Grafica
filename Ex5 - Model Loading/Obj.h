#ifndef OBJ_H
#define OBJ_H

#include "Mtl.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include "Shader.h"
#include <glm/gtx/hash.hpp>
#include <unordered_set>
#include <cstring>

struct GabrielFace {
    int vertice[3];
    int vt[3];
    int vn[3];
};

struct GabrielVertex {
    glm::vec4 position;
    glm::vec3 normal;
    glm::vec2 texture;
};

struct Drawable {
    int vao;
    int groupSize;
    int textureId;
    glm::vec3 ambientLightColor;
    glm::vec3 difuseLightColor;
    glm::vec3 specularLightColor;
};

struct GabrielGroup {
    std::vector<unsigned int> indices;
    Mtl mtl;
};

class Obj {
public:
    Obj(const GLchar* objFilePath, const GLchar* vertexShaderLocation, const GLchar* fragmentShaderLocation);
    ~Obj();
    void draw(glm::mat4 perspective, glm::mat4 model, glm::mat4 view);
    void draw(glm::mat4 perspective, glm::mat4 model, glm::mat4 view, glm::vec3 cameraPos);
    void setLightColor(glm::vec3 lightColor);
    void setObjColor(glm::vec3 objColor);
    void setLightPosition(glm::vec3 lightPosition);
    void createBuffers();
    bool isLamp = false;
private:
    std::vector<GabrielVertex> vertexes;
    std::vector<GabrielGroup> groups;
    Shader* shader;
    glm::vec3 lightColor = glm::vec3(1.0f);
    glm::vec3 objColor;
    glm::vec3 lightPosition;
    std::vector<Drawable> drawables;
    MtlLib* mtllib;
};

#endif
