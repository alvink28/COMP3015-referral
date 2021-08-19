#include <sstream>

#include "scenebasic_uniform.h"
#include "helper/noisetex.h"

#include <iostream>
using std::cerr;
using std::endl;

//#include "helper/glutils.h"
#include <glm/gtc/matrix_transform.hpp>
using glm::mat3;
using glm::mat4;
using glm::vec3;

SceneBasic_Uniform::SceneBasic_Uniform() : plane(20, 20, 1, 1), teapot(5, glm::mat4(1.0f)), tPrev(0.0f), lightPos(5.0f, 5.0f, 5.0f, 1.0f)
{
    mesh = ObjMesh::load("media/trophy.obj");
}

void SceneBasic_Uniform::initScene()
{
    compile();
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f); 
    glEnable(GL_DEPTH_TEST);

    view = glm::lookAt(glm::vec3(10.0f, 4.0f, 7.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    projection = glm::perspective(glm::radians(50.0f), (float)width / height, 0.5f, 100.0f);

    lightAngle = 0.0f;
    lightRotationSpeed = 1.5f;

    prog.setUniform("Light[0].L", glm::vec3(1.0f));
    prog.setUniform("Light[0].Position", view * lightPos);
    prog.setUniform("Light[1].L", glm::vec3(1.0f));
    prog.setUniform("Light[1].Position", glm::vec4(0, 0.15f, -1.0f, 0));
    prog.setUniform("Light[2].L", glm::vec3(1.0f));
    prog.setUniform("Light[2].Position", view * glm::vec4(-7, 3, 7, 1));

    //noise
    GLfloat verts[] = {-1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                       -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f };

    GLfloat tc[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                    0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};

    // Set up the buffers 
    unsigned int handle[2]; 
    glGenBuffers(2, handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, GL_STATIC_DRAW);

    // Set up the vertex array object 
    glGenVertexArrays(1, &quad); 
    glBindVertexArray(quad);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0))); 
    glEnableVertexAttribArray(0); // Vertex position

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0))); 
    glEnableVertexAttribArray(2); // Texture coordinates

    glBindVertexArray(0); 
    prog.setUniform("NoiseTex" , 0);
    GLuint noiseTex = NoiseTex::generate2DTex(6.0f); 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, noiseTex);
}


void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();

	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
    float deltaT = t - tPrev;

    if (tPrev == 0.0f) deltaT = 0.0f;
    tPrev = t; 
    
    if (animating()) {
        lightAngle = glm::mod(lightAngle + deltaT * lightRotationSpeed, glm::two_pi<float>());
        lightPos.x = glm::cos(lightAngle) * 7.0f;
        lightPos.y = 3.0f;
        lightPos.z = glm::sin(lightAngle) * 7.0f;
    }
}

void SceneBasic_Uniform::render()
{
    //view = mat4(1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    prog.setUniform("Light[0].Position", view * lightPos); 
    drawScene();
    glFinish();
}

void SceneBasic_Uniform::setMatrices(GLSLProgram& prog)
{
    glm::mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", glm::mat3(mv)); 
    prog.setUniform("MVP", projection * mv);
}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(60.0f), (float)width / height, 0.3f, 100.0f);
}

void SceneBasic_Uniform::drawScene() {
    drawFloor();

    // Draw pigs with varying roughness
    int numPigs = 9;
    glm::vec3 cowBaseColor(0.1f, 0.33f, 0.97f);
    for (int i = 0; i < numPigs; i++) {
        float cowX = i * (10.0f / (numPigs - 1)) - 5.0f;
        float rough = (i + 1) * (1.0f / numPigs);
        drawSpot(glm::vec3(cowX, 0, 0), rough, 0, cowBaseColor);
    }

    // Draw metal pigs
    float metalRough = 0.43f;
    //Gold
    drawSpot(glm::vec3(-3.0f, 0.0f, 3.0f), metalRough, 1, glm::vec3(1, 0.71f, 0.29f));
    //Copper
    drawSpot(glm::vec3(-1.5f, 0.0f, 3.0f), metalRough, 1, glm::vec3(0.95f, 0.64f, 0.54f));
    //Aluminum
    drawSpot(glm::vec3(-0.0f, 0.0f, 3.0f), metalRough, 1, glm::vec3(0.91f, 0.92f, 0.92f));
    //Titanium
    drawSpot(glm::vec3(1.5f, 0.0f, 3.0f), metalRough, 1, glm::vec3(0.542f, 0.497f, 0.449f));
    //Silver
    drawSpot(glm::vec3(3.0f, 0.0f, 3.0f), metalRough, 1, glm::vec3(0.95f, 0.93f, 0.88f));
}

void SceneBasic_Uniform::drawFloor() {
    model = glm::mat4(1.0f);
    prog.setUniform("Material.Rough", 0.9f);
    prog.setUniform("Material.Metal", 0);
    prog.setUniform("Material.Color", glm::vec3(0.2f));
    model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f));
    setMatrices(prog); 
    plane.render();

    glBindVertexArray(quad);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SceneBasic_Uniform::drawSpot(const glm::vec3& pos, float rough, int metal, const glm::vec3& color) {
    model = glm::mat4(1.0f);
    prog.setUniform("Material.Rough", rough); 
    prog.setUniform("Material.Metal", metal); 
    prog.setUniform("Material.Color", color); 
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-90.0f, 1.0f, 0.0f));

    setMatrices(prog); 
    mesh->render();
}
