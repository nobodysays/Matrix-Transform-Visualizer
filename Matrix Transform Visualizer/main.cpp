#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include "Shader.h"
#include <GL/glew.h>          
#include <GLFW/glfw3.h>
glm::mat4 view;
glm::mat4 projection;
void OnScroll(GLFWwindow* window, double, double yoffset)
{

}

float theta = 35.0f;
float phi = 35.0f;

int main(int, char**)
{
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); 
    glViewport(0, 0, 800, 600);
    bool err = glewInit() != GLEW_OK;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");


    float vertices[] = {
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

    float axisVertices[] = {
        .0,.0, .0, 1.0, 0.0, 0.0,
        .0,.0, 2., 1.0, 0.0, 0.0,

        .0,.0,.0,  0.0, 1.0, 0.0,
        .0,2.0,.0, 0.0, 1.0, 0.0,

        .0,.0,.0,  0.0, 0.0, 1.0,
        2.0,0.0,.0,0.0, 0.0, 1.0
    };
    GLuint axisVAO;
    GLuint axisVBO;

    glGenBuffers(1, &axisVAO);
    glGenBuffers(1, &axisVBO);

    auto axisShader = new Shader("Axis.vert", "Axis.frag");
    glGenVertexArrays(1, &axisVAO);
    glGenBuffers(1, &axisVBO);

    glBindVertexArray(axisVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(1);


    GLuint cubeVAO;
    GLuint cubeVBO;

    glGenBuffers(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

   auto shader = new Shader("Cube.vert", "Cube.frag");
   glGenVertexArrays(1, &cubeVAO);
   glGenBuffers(1, &cubeVBO);

   glBindVertexArray(cubeVAO);
   glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(0);

   glm::mat4 model(0.0);
   model = glm::rotate(model, -55.0f, glm::vec3(1.0f, 0.0f, 0.0f));


   projection = glm::perspective(45.0f, (float)800 / (float)600, 0.1f, 100.0f);
   glm::vec3 position = glm::vec3(1.0f, 2.0, 1.0f);
   glm::vec3 target = glm::vec3(0.0, 0.0f, -3.0f);
   glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
   view = glm::lookAt(position, target, up);

   float size = 1.0f;
   float angleY = 0.0f;
   float angleZ = 0.0f;

   glm::mat4 scaleTransform = glm::mat4(1.0f);
   glm::mat4 translateTransform = glm::mat4(1.0f);
   glm::mat4 rotateTransform = glm::mat4(1.0f);


   float width = 1.0f;
   float height = 1.0f;
   float depth = 1.0f;

   float positionX = 0.0f;
   float positionY = 0.0f;
   float positionZ = 0.0f;
   projection = glm::perspective(45.0f, (float)800 / (float)600, 0.1f, 100.0f);

   float fielOfView = 45.0f;
   float aspect = 800.f/600.f;
   float far = 0.1f;
   float near = 100.0f;

   bool ortho = false;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glfwSetScrollCallback(window, OnScroll);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Matrix Transform Visualizer");                          // Create a window called "Hello, world!" and append into it.

            //ImGui::Checkbox("Orthographic projection", &ortho);

            ImGui::Text("Perspective projection options");
            ImGui::SliderFloat("fov", &fielOfView, 0.f,90.f);
            ImGui::SliderFloat("aspect", &aspect, 0.1, 3.0);
            ImGui::SliderFloat("near", &near, 0.1, 1000.0);
            ImGui::SliderFloat("far", &far, 0.1, 1000.0);
            if (ImGui::Button("Reset Perspective options"))
            {
                fielOfView = 45.0f;
                aspect = 800.f / 600.f;
                near = 0.1f;
                far = 100.0f;
            }


            ImGui::Text("Size");               // Display some text (you can use a format strings too)
            ImGui::SliderFloat("height", &height, 0.1, 3.0);
            ImGui::SliderFloat("width",  &width,  0.1, 3.0);
            ImGui::SliderFloat("depth",  &depth,  0.1, 3.0);
            if (ImGui::Button("Reset size"))
            {
                width = height = depth = 1.0f;
            }
            ImGui::Text("Position");               // Display some text (you can use a format strings too)
            ImGui::SliderFloat("x", &positionX, -1.0, 1.0);
            ImGui::SliderFloat("y", &positionY, -1.0, 1.0);
            ImGui::SliderFloat("z", &positionZ, -1.0, 1.0);
            if (ImGui::Button("Reset position"))
            {
                positionX = positionY = positionZ = 0.0f;
            }

            ImGui::Text("Spherical coordinates for viewer");               // Display some text (you can use a format strings too)
            ImGui::SliderFloat("theta", &theta, -180.0, 180.0);
            ImGui::SliderFloat("phi", &phi, -180.0, 180.0);
            if (ImGui::Button("Reset spherical coordinates"))
            {
                theta = phi = 35.0f;
            }

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);

        if (ortho)
        {
            //projection = glm::ortho(-100.0f, -100.f, -100.f, 100.f);
        }
        else
        {
            projection = glm::perspective(fielOfView, aspect, near, far);
        }

        double r = 3.0;
        double PI = 3.1415;
        glm::vec3 position = { r * sin(theta * PI/180.) * cos(phi * PI / 180.),r * sin(theta * PI / 180.) * sin(phi * PI / 180.) , r * cos(theta * PI / 180.) };
        glm::vec3 target = glm::vec3(0.0, 0.0f, 0.0f);
        glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
        view = glm::lookAt(position, target, up);

        glBindVertexArray(axisVAO);
        axisShader->setMat4("model", glm::translate(glm::mat4(1.f), glm::vec3(0.0)) * glm::scale(glm::mat4(1.0), glm::vec3(size)));
        axisShader->setMat4("view", view);
        axisShader->setMat4("projection", projection);
        glDrawArrays(GL_LINES, 0, 6);
        glBindVertexArray(0);


        scaleTransform = glm::scale(glm::mat4(1.0), { width, height, depth });
        translateTransform = glm::translate(glm::mat4(1.0), { positionX, positionY, positionZ });

        glBindVertexArray(cubeVAO);
        shader->setMat4("model", translateTransform*rotateTransform*scaleTransform);
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
        glDrawArrays(GL_LINES, 0, 48);
        glBindVertexArray(0);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
