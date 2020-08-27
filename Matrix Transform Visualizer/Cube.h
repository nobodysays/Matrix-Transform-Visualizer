#pragma once
#include "helpfile.h"
#include "Viewer.h"
#include "Object.h"
class Cube : public Object
{
private:
    float vertices[72] = {
        //front
       -.5, .5, .5,
       -.5, -.5, .5,

       -.5, -.5, .5,
       .5, -.5, .5,

       .5, -.5, .5,
       .5, .5, .5,

       .5, .5, .5,
       -.5, .5, .5,
       //back
       -.5, .5, -.5,
       -.5, -.5, -.5,

       -.5, -.5, -.5,
       .5, -.5, -.5,

       .5, -.5, -.5,
       .5, .5, -.5,

       .5, .5, -.5,
       -.5, .5, -.5,

       //left

       -.5, .5,-.5,
       -.5, .5,.5,

       -.5, -.5,.5,
       -.5, -.5,-.5,

       //right

       .5, .5,-.5,
       .5, .5,.5,

       .5, -.5,.5,
       .5, -.5,-.5,
    };
    glm::mat4 basis = glm::mat4(1.0);
    Viewer* viewer=nullptr;
public:
    float phi = 0.0f;
    float theta = 0.0f;
    float gamma = 0.0f;
    float b = .5;
    glm::vec3 shearsX = glm::vec3(.0f);
    glm::vec3 shearsY = glm::vec3(.0f);
    glm::vec3 shearsZ = glm::vec3(.0f);
#pragma region Properties Funcs
    const glm::vec3& GetPosition() { return position; }
    void SetPosition(const glm::vec3& value) { position = ClampVec3(value, -5, 5); }

    const glm::vec3& GetSize() { return size; }
    void SetSize(const glm::vec3& value) { size = ClampVec3(value, 0.1, 10); }

    const glm::mat4& GetBasis() { return basis; }
    void SetBasis(const glm::mat4& value) { basis = value; }
#pragma endregion
#pragma region Properties
    __declspec(property(get = GetPosition, put = SetPosition)) glm::vec3 Position;
    __declspec(property(get = GetSize, put = SetSize)) glm::vec3 Size;
    __declspec(property(get = GetBasis, put = SetBasis)) glm::mat4 Basis;
#pragma endregion
    void ResetBasis() { basis = glm::mat4(1.0f); }
    void ResetPosition() { position = glm::vec3(0.0); }
    void ResetSize() { size = glm::vec3(1.0f); }
    void ResetShears() { shearsX = shearsY = shearsZ = glm::vec3(.0f); }
    Cube(Viewer* viewer)
    {
        this->viewer = viewer;
        GLuint VBO;
        glGenBuffers(1, &VAO);
        glGenBuffers(1, &VBO);

        shader = new Shader("Cube.vert", "Cube.frag");
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    }
    void Update() override
    {
        glBindVertexArray(VAO);
        glm::mat3 rotateAroundZ = {
            {cos(glm::radians(phi)), -sin(glm::radians(phi)),0},
            {sin(glm::radians(phi)), cos(glm::radians(phi)), 0},
            {0, 0, 1}
        };
        glm::mat3 rotateAroundX = {
            {1, 0, 0},
            {0, cos(glm::radians(theta)), -sin(glm::radians(theta))},
            {0, sin(glm::radians(theta)), cos(glm::radians(theta))},
        };
        glm::mat3 rotateAroundY = {
            {cos(glm::radians(gamma)), 0, -sin(glm::radians(gamma))},
            {0,1,0},
            {sin(glm::radians(gamma)), 0, cos(glm::radians(gamma))}
        };
        shader->setMat4("translateTransform", glm::translate(basis, position));
        shader->setMat4("scaleTransform", glm::scale(basis, size));
        shader->setMat4("rotationTransform", rotateAroundZ*rotateAroundY* rotateAroundX);
        shader->setMat4("shearsTransform", glm::mat4(
            {
            {1,   glm::radians(ClampFloat(shearsX.y, -180, 180)), glm::radians(ClampFloat(shearsX.z, -180, 180)), 0},
            {glm::radians(ClampFloat(shearsY.x, -180, 180)), 1,   glm::radians(ClampFloat(shearsY.z, -180, 180)), 0},
            {glm::radians(ClampFloat(shearsZ.x, -180, 180)), glm::radians(ClampFloat(shearsZ.y, -180, 180)), 1,   0},
            {0,   0,   0,   1}
            }));
        shader->setMat4("view", viewer->View);
        shader->setMat4("projection", viewer->Projection);
        glDrawArrays(GL_LINES, 0, 48);
        glBindVertexArray(0);
    }

    void ResetRotation()
    {
        phi = theta = gamma = 0;
    }
};
