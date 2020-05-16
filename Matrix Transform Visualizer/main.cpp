#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include "Shader.h"
#include <GL/glew.h>          
#include <GLFW/glfw3.h>
glm::mat4 view;
glm::mat4 projection;
float r = 3.0;
const double width =1920;
const double height = 1080;
void OnScroll(GLFWwindow* window, double, double yoffset)
{
    r -= yoffset * 0.4;
    if (r > 5)
        r = 5;
    if (r < 0.5)
        r = 0.5;
}

void OnKeyboard(GLFWwindow*window, int key, int scancode, int actions, int mods)
{
    if (key == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

float hxy=0, hxz=0, hyx=0, hyz=0, hzx=0, hzy=0;

float theta = 17.0f;
float phi = 60.0f;

float alpha = 0.0f;
float beta = 0.0f;
float gamma = 0.0f;
bool firstMouse = true;
double lastX = width/2, lastY = height/2;
bool mouseClicked = false;
void OnMouse(GLFWwindow* window, double xpos, double ypos)
{
    if (mouseClicked)
    {
        if (xpos <= 2*width/3)
        {

            if (firstMouse) // эта переменная была проинициализирована значением true
            {
                lastX = xpos;
                lastY = ypos;
                firstMouse = false;
            }
            GLfloat xoffset = xpos - lastX;
            GLfloat yoffset = lastY - ypos;
            lastX = xpos;
            lastY = ypos;
            phi += xoffset;
            theta -= yoffset;
            if (phi > 89.0)
                phi = 89.0f;
            if (phi < -89.0)
                phi = -89.0f;

            if (theta > 89.0)
                theta = 89.0f;
            if (theta < -89.0)
                theta = -89.0f;
        }
    }
}
void OnMouseButtonClicked(GLFWwindow*window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        mouseClicked = true;
        glfwGetCursorPos(window, &lastX, &lastY);
    }
    else
    {
        mouseClicked = false;
    }
}
int main(int, char**)
{
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(width, height, "Matrix Transform Visualizer", nullptr, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); 
    glViewport(0, 0, width, height);
    bool err = glewInit() != GLEW_OK;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->AddFontFromFileTTF("RobotoMono-Regular.ttf", 18);
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


   projection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
   glm::vec3 position = glm::vec3(1.0f);
   glm::vec3 target = glm::vec3(0.0, 0.0f, -3.0f);
   glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
   view = glm::lookAt(position, target, up);

   float size = 1.0f;
   float angleY = 0.0f;
   float angleZ = 0.0f;

   glm::mat4 scaleTransform = glm::mat4(1.0f);
   glm::mat4 translateTransform = glm::mat4(1.0f);
   glm::mat4 rotateTransform = glm::mat4(1.0f);


   float cubeWidth = 1.0f;
   float cubeHeight = 1.0f;
   float cubeDepth = 1.0f;

   float positionX = 0.0f;
   float positionY = 0.0f;
   float positionZ = 0.0f;
   projection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);

   float fielOfView = 45.0f;
   float aspect = width/height;
   float far = 0.1f;
   float near = 100.0f;

   bool ortho = false;
     glfwSetScrollCallback(window, OnScroll);
   glfwSetCursorPosCallback(window, OnMouse);
   glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
   glfwSetKeyCallback(window, OnKeyboard);
   ImGuiWindowFlags window_flags = 0;
   window_flags |= ImGuiWindowFlags_NoMove;
   window_flags |= ImGuiWindowFlags_NoResize;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            static float f = 0.0f;
            static int counter = 0;
            ImGui::SetNextWindowPos(ImVec2(2*width/3, 0), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(2 * width / 3, height), ImGuiCond_FirstUseEver);
            ImGui::Begin("Matrix Transform Visualizer", nullptr, window_flags);                          // Create a window called "Hello, world!" and append into it.
            if (ImGui::CollapsingHeader("Perspective"))
            {

                ImGui::Text("Perspective matrix:");
                ImGui::Text("\
|x'|   | ctg(fov/2)/a  0           0            0  |   |x| \n\
|  |   |                                           |   | |       \n\
|y'|   | 0             ctg(fov/2)  0            0  |   |y| \n\
|  | = |                                           | * | |       \n\
|z'|   | 0             0           (f+n)/(f-n)  0  |   |z| \n\
|  |   |                                           |   | |       \n\
|1 |   | 0             0           -2fn/(f-n)   0  |   |1| \n\
                    ");
                ImGui::Separator();
                ImGui::BulletText("x, y, z old coordinates");
                ImGui::BulletText("x', y', z' new coordinates");
                ImGui::BulletText("a=aspect, n=near, f=far");
                ImGui::Separator();
                ImGui::SliderFloat("fov", &fielOfView, 0.f, 90.f);
                ImGui::SliderFloat("aspect", &aspect, 0.1, 3.0);
                ImGui::SliderFloat("near", &near, 0.1, 1000.0);
                ImGui::SliderFloat("far", &far, 0.1, 1000.0);
                if (ImGui::Button("Reset Perspective options"))
                {
                    fielOfView = 45.0f;
                    aspect = width / height;
                    near = 0.1f;
                    far = 100.0f;
                }
            }
            if (ImGui::CollapsingHeader("Size"))
            {
                ImGui::SliderFloat("height", &cubeHeight, 0.1, 3.0);
                ImGui::SliderFloat("width", &cubeWidth, 0.1, 3.0);
                ImGui::SliderFloat("depth", &cubeDepth, 0.1, 3.0);
                if (ImGui::Button("Reset size"))
                {
                    cubeWidth = cubeHeight = cubeDepth = 1.0f;
                }
            }
            if (ImGui::CollapsingHeader("Position"))
            {
                ImGui::SliderFloat("x", &positionX, -1.0, 1.0);
                ImGui::SliderFloat("y", &positionY, -1.0, 1.0);
                ImGui::SliderFloat("z", &positionZ, -1.0, 1.0);
                if (ImGui::Button("Reset position"))
                {
                    positionX = positionY = positionZ = 0.0f;
                }
            }
            if (ImGui::CollapsingHeader("Shears"))
            {
                ImGui::SliderFloat("hxy", &hxy, -90.0, 90.0);
                ImGui::SliderFloat("hxz", &hxz, -90.0, 90.0);
                ImGui::SliderFloat("hyx", &hyx, -90.0, 90.0);
                ImGui::SliderFloat("hyz", &hyz, -90.0, 90.0);
                ImGui::SliderFloat("hzx", &hzx, -90.0, 90.0);
                ImGui::SliderFloat("hzy", &hzy, -90.0, 90.0);
                if (ImGui::Button("Reset shears"))
                {
                    hxy = hxz = hyx = hyz = hzx = hzy = 0;
                }
            }
            if (ImGui::CollapsingHeader("Spherical coordinates for viewer"))
            {

                ImGui::SliderFloat("theta", &theta, -180.0, 180.0);
                ImGui::SliderFloat("phi", &phi, -180.0, 180.0);
                ImGui::SliderFloat("radius", &r, 0.5, 5.0);
                if (ImGui::Button("Reset spherical coordinates"))
                {

                    theta = 17.0f;
                    phi = 60.0f;
                    r = 3;
                }
            }

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);

            projection = glm::perspective(fielOfView, aspect, near, far);

        double PI = 3.1415;
        position.x = r*cos(glm::radians(theta)) * cos(glm::radians(phi));
        position.y = r*sin(glm::radians(theta));
        position.z = r*cos(glm::radians(theta)) * sin(glm::radians(phi));
        glm::vec3 target = glm::vec3(0.0, 0.0f, 0.0f);
        glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
        view = glm::lookAt(position, target, up);

        glBindVertexArray(axisVAO);
        axisShader->setMat4("model", glm::translate(glm::mat4(1.f), glm::vec3(0.0)) * glm::scale(glm::mat4(1.0), glm::vec3(size)));
        axisShader->setMat4("view", view);
        axisShader->setMat4("projection", projection);
        glDrawArrays(GL_LINES, 0, 6);
        glBindVertexArray(0);


        scaleTransform = glm::scale(glm::mat4(1.0), { cubeWidth, cubeHeight, cubeDepth });
        translateTransform = glm::translate(glm::mat4(1.0), { positionX, positionY, positionZ });
        glm::mat4 shears = glm::mat4(
            {
            {1, hxy*PI/180, hxz * PI / 180, 0},
            {hyx * PI / 180, 1, hyz * PI / 180, 0},
            {hzx * PI / 180, hzy * PI / 180, 1, 0},
            {0, 0, 0, 1}
            }
        );
        glBindVertexArray(cubeVAO);
        shader->setMat4("model", translateTransform*rotateTransform*scaleTransform* shears);
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
