#pragma once
#include "helpfile.h"
#include "Viewer.h"
class Axis
{
public:

    float vertices[36] = {
        .0,.0, .0, 1.0, 0.0, 0.0,
        .0,.0, 2., 1.0, 0.0, 0.0,

        .0,.0,.0,  0.0, 1.0, 0.0,
        .0,2.0,.0, 0.0, 1.0, 0.0,

        .0,.0,.0,  0.0, 0.0, 1.0,
        2.0,0.0,.0,0.0, 0.0, 1.0
    };
    GLuint VAO = 0;
    Shader* shader = nullptr;
    Viewer* viewer = nullptr;
    glm::vec3 position = glm::vec3(0.0);
    Axis(Viewer* viewer) :viewer(viewer)
    {
        GLuint VBO = 0;
        glGenBuffers(1, &VAO);
        glGenBuffers(1, &VBO);

        shader = new Shader("Axis.vert", "Axis.frag");
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 3));
        glEnableVertexAttribArray(1);
    }
    void Update()
    {
        glBindVertexArray(VAO);
        shader->Use();
        shader->setMat4("model", glm::mat4(1.0f));
        shader->setMat4("view", viewer->View);
        shader->setMat4("projection", viewer->Projection);
        glDrawArrays(GL_LINES, 0, 6);
        glBindVertexArray(0);
    }
};
