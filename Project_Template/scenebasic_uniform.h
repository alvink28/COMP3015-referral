#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"
#include "helper/random.h"
#include "helper/grid.h"
#include "helper/plane.h"
#include "helper/objmesh.h"
#include "helper/teapot.h"

#include <glm/glm.hpp>

class SceneBasic_Uniform : public Scene
{
private:

    GLSLProgram prog;

    GLuint quad;

    float angle;

    Plane plane;
    std::unique_ptr<ObjMesh> mesh;
    Teapot teapot;

    float tPrev, lightAngle, lightRotationSpeed;
    glm::vec4 lightPos;

    void setMatrices(GLSLProgram&);
    void compile();
    void drawScene();
    void drawFloor();
    void drawSpot(const glm::vec3& pos, float rough, int metal, const glm::vec3& color);

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
