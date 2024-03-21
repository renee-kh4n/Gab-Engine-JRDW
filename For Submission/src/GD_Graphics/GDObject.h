#include "GDutil.h"
#include "GDShader.h"
#include "GDTexture.h"

#include "tiny_obj_loader.h"

class GDObject {
public:
    glm::vec3 pos = glm::vec3(0, 0, 0);
    glm::vec3 scale = glm::vec3(1);
    glm::vec3 rot = glm::vec3(0);

    GDShader* shader;
    GDTexture* texturediff;
    GDTexture* texturenormal;

    GLuint VAO, VBO;
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    std::vector<GLfloat> fullVertexData;

    GDObject(std::string path, GDShader* shader, GDTexture* texturediff, GDTexture* texturenormal, glm::vec3 pos = glm::vec3(0,0,0), glm::vec3 scale = glm::vec3(1, 1, 1), glm::vec3 rot = glm::vec3(0, 0, 0));
};