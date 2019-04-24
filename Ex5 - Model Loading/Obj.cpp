#include "Obj.h" 

bool isEol(char c) {
    return (c == '\n') ||
           (c == '\r');
}

namespace std {
    template<>
    struct equal_to<GabrielVertex> {
        bool operator() (const GabrielVertex& a, const GabrielVertex& b) const {
            bool equals = a.normal == b.normal && a.position == b.position && a.texture == b.texture;
            return equals;
        }
    };
    
    template<>
    struct hash<GabrielVertex> {
        std::size_t operator() (GabrielVertex const& s) const noexcept {
            using std::size_t;
            using std::hash;
            using glm::vec3;
            using glm::vec4;
            
            std::size_t const h1( std::hash<glm::vec3>{}(s.normal));
            std::size_t const h2( std::hash<glm::vec4>{}(s.position));
            std::size_t const h3( std::hash<glm::vec2>{}(s.texture));
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}

struct VertexComparison {
    explicit VertexComparison(GabrielVertex i) {
        this->vertex = i;
    }
    inline bool operator() (const GabrielVertex& obj) const  {
        return std::equal_to<GabrielVertex>{}(this->vertex, obj);
    }
private:
    GabrielVertex vertex;
};

Obj::Obj(const GLchar* objFilePath, const GLchar* vertexShaderLocation, const GLchar* fragmentShaderLocation) {
    this->shader = new Shader(vertexShaderLocation, fragmentShaderLocation);
    std::ifstream objFile(objFilePath);
    
    // ensure ifstream objects can throw exceptions:
    if (!objFile) {
        std::cout << "CAN'T FIND THE OBJ FILE";
    }
    std::vector<glm::dvec4> vertexs;
    std::vector<glm::vec3> vertexNormals;
    std::vector<glm::vec3> texCoords;
    std::map<std::string, std::vector<GabrielFace> > groups;
    std::vector<std::vector<GabrielFace> *> faces;
    faces.push_back(&groups["default"]);
    std::vector<std::string> currentGroupNames;
    std::map<std::string,Mtl> mtls;
    
    std::string line;
    std::string identifier;
    try {
        while(std::getline(objFile, line)) {
            std::stringstream lineStream(line);
            lineStream >> identifier;
            if (identifier == "#") {
                continue;
            } else if (identifier == "v") {
                glm::dvec4 v;
                if (!(lineStream >> v.x >> v.y >> v.z >> v.w)) {
                    lineStream >> v.x >> v.y >> v.z;
                    v.w = 1.0f;
                }
                vertexs.push_back(v);
            } else if (identifier == "vn") {
                glm::vec3 vn;
                lineStream >> vn.x >> vn.y >> vn.z;
                vertexNormals.push_back(vn);
            } else if (identifier == "vt") {
                glm::vec3 vt;
                if (!(lineStream >> vt.x >> vt.y >> vt.z)) {
                    if (!(lineStream >> vt.x >> vt.y)) {
                        lineStream >> vt.x;
                    }
                }
                texCoords.push_back(vt);
            } else if (identifier == "g") {
                std::string groupName;
                faces = {};
                currentGroupNames = {};
                while(lineStream >> groupName){
                    currentGroupNames.push_back(groupName);
                    faces.push_back(&groups[groupName]);
                }
            } else if (identifier == "mtllib") {
                std::string mtlname;
                lineStream >> mtlname;
				std::string filePath = objFilePath;
				std::string directory = filePath.substr(0, filePath.find_last_of('/'));
                this->mtllib = new MtlLib(directory, mtlname);
            } else if (identifier == "usemtl") {
                std::string name;
                lineStream >> name;
                for (auto groupName: currentGroupNames) {
                   mtls[groupName] = this->mtllib->getMtlByName(name);
                }
            } else if (identifier == "f") {
                std::string verticeIndex;
                GabrielFace face = {};
                int i = -1;
                while (lineStream >> verticeIndex) {
                    i++;
                    std::stringstream vertStream(verticeIndex);
                    std::string v;
                
                    std::getline(vertStream, v, '/');
                    face.vertice[i] = std::stoi(v) - 1 ;
                    if (vertStream.eof()) {
                        continue;
                    }
                    std::getline(vertStream, v, '/');
                    if (v.size() > 0) {
                        face.vt[i] = std::stoi(v) - 1;
                    }
                    vertStream >> face.vn[i];
                    face.vn[i]--;
                    
                }
                for (auto group = faces.begin(); group != faces.end(); ++group) {
                    (*(*group)).push_back(face);
                }
            }
        }
        
        // close file handlers
        objFile.close();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::OBJ::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    
    if (vertexNormals.size() == 0) {
        vertexNormals.push_back(glm::vec3(0));
    }
    
    if (texCoords.size() == 0) {
        texCoords.push_back(glm::vec3(0));
    }
    
    std::unordered_set<GabrielVertex> unorderedVert;
    //create the object to be used by opengl
    for (auto group: groups) {
        std::vector<unsigned int> indexes;
        for (auto face: group.second) { // warning, sometimes the order is not preserved, idk why
            for (int i = 0; i < 3; i++) {
                GabrielVertex vert;
                vert.position = vertexs.at(face.vertice[i]);
                vert.normal = vertexNormals.at(face.vn[i]);
                vert.texture = texCoords.at(face.vt[i]);
                unsigned int index;
                bool canInsert;
                std::tie(std::ignore, canInsert) = unorderedVert.insert(vert);
                if (canInsert) {
                    this->vertexes.push_back(vert);
                    index = this->vertexes.size() - 1;
                } else {
                    auto existentVertex = std::find_if(this->vertexes.begin(), this->vertexes.end(), VertexComparison(vert));
                    if (existentVertex == this->vertexes.end()) {
                        std::cout << "FATAL ERROR, COULDNT FIND THE VERTEX ALREADY ON THE LIST";
                    }
                    index = std::distance(this->vertexes.begin(), existentVertex);
                    assert(std::equal_to<GabrielVertex>{}(this->vertexes.at(index), vert));
                }
                indexes.push_back(index);
            }
        }
        GabrielGroup currentGroup;
        currentGroup.indices = indexes;
        currentGroup.mtl = mtls[group.first];
        
        this->groups.push_back(currentGroup);
    }
}

void Obj::createBuffers() {
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    float* location = glm::value_ptr(this->vertexes.front().position);
    glBufferData(GL_ARRAY_BUFFER, this->vertexes.size() * sizeof(GabrielVertex), location, GL_STATIC_DRAW);
    
     for (auto group = this->groups.begin(); group != this->groups.end(); ++group) {
        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
    
    
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
        //position
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GabrielVertex), (void*)0);
        glEnableVertexAttribArray(0);
        
        //normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GabrielVertex), (void*)(4 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
         //texture
         glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GabrielVertex), (void*)(7 * sizeof(float)));
         glEnableVertexAttribArray(2);
    
        GLuint EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        unsigned int * loc = &group->indices.front();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, group->indices.size() * 3 * sizeof(unsigned int), loc, GL_STATIC_DRAW);
    
        glBindVertexArray(0);
         Drawable drawable;
         drawable.vao = VAO;
         drawable.groupSize = group->indices.size();
         drawable.textureId = group->mtl.getTextureIndex();
         this->drawables.push_back(drawable);
    }
}

void Obj::draw(glm::mat4 perspective, glm::mat4 model, glm::mat4 view) {
    shader->useProgram();
    
    shader->setMat4("perspective", perspective);
    shader->setMat4("view", view);
    shader->setMat4("model", model);
    

    for (auto drawable : this->drawables){
        glBindVertexArray(drawable.vao);
        glBindTexture(GL_TEXTURE_2D, drawable.textureId);
        glDrawElements(GL_TRIANGLES, drawable.groupSize * 3, GL_UNSIGNED_INT, 0);
    }
}

void Obj::draw(glm::mat4 perspective, glm::mat4 model, glm::mat4 view, glm::vec3 cameraPos) {
    shader->useProgram();
    shader->setVec3("cameraPosition", cameraPos);
    this->draw(perspective, model, view);
}

void Obj::setObjColor(glm::vec3 objColor) {
    this->objColor = objColor;
}

void Obj::setLightColor(glm::vec3 lightColor) {
    this->lightColor = lightColor;
}

void Obj::setLightPosition(glm::vec3 lightPosition) {
    this->lightPosition = lightPosition;
}

Obj::~Obj() {
    delete shader;
} 
