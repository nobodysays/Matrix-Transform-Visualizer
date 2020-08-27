#include "helpfile.h"
#include "Cube.h"
#include "Viewer.h"
#include "Axis.h"
void OnKeyboard(GLFWwindow* window, int key, int scancode, int actions, int mods);
void OnScroll(GLFWwindow* window, double, double yoffset);
void OnMouse(GLFWwindow* window, double xpos, double ypos);
void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods);
void PrintPerspectiveMatrixDescription();
void PrintScaleMatrixDescription();
void PrintTranslateMatrixDescription();
void PrintRotationMatrixDescription();
void PrintShearsMatrixDescription();
void PrintMatrixDescription();
void PrintSphericalCoordinatesDescription();
Cube* cube;
Viewer* viewer;
Axis* axis;
bool canRotate = true;

int main(int, char**)
{
#pragma region inits
    //initializing glew, glfw, imgui, opengl, callbacks
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow*  window = glfwCreateWindow(width, height, "Matrix Transform Visualizer", nullptr, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); 
    glViewport(0, 0, width, height);
    glewInit();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->AddFontFromFileTTF("RobotoMono-Regular.ttf", 18);
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
    glfwSetScrollCallback(window, OnScroll);
    glfwSetCursorPosCallback(window, OnMouse);
    glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
    glfwSetKeyCallback(window, OnKeyboard);
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
#pragma endregion
    //adding objects to scene
    viewer = new Viewer();
    cube = new Cube(viewer);
    axis = new Axis(viewer);

    bool fullScreen = false;
    bool showFps = false;
    
    const int sectors = 10;
    const int stacks =  10;

    const float pitchOffset = 2 * M_PI / sectors;
    const float yawOffset = 2 * M_PI / stacks;

    float vertices[3*sectors*stacks];

    float pitch = M_PI_2;
    float yaw = M_PI_2;
    for (size_t i = 0; i < sectors; i++)
    {
        for (size_t j = 0; j < stacks; j++)
        {
            vertices[stacks*i*3 + 3*j]   = sin(pitch) * cos(yaw);
            vertices[stacks*i*3 + 3*j+1] = sin(pitch)* sin(yaw);
            vertices[stacks*i*3 + 3*j+2] = cos(pitch);
            yaw += yawOffset;

        }
        pitch += pitchOffset;
    }
   /* position = { distanceToTarget * cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
                    distanceToTarget * sin(glm::radians(pitch)),
                    distanceToTarget * cos(glm::radians(pitch)) * sin(glm::radians(yaw)) };*/

    GLuint VAO;
    GLuint VBO;
     auto shader = new Shader("Dots.vert", "Dots.frag");
    glGenBuffers(1, &VAO);
    glGenBuffers(1, &VBO);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents(); //keyboaed, mouse, scroll events
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::SetNextWindowPos(ImVec2(2*width, 0), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(2 * width / 3, height), ImGuiCond_FirstUseEver);
            ImGui::Begin("Matrix Transform Visualizer", nullptr, window_flags);
            ImGui::Checkbox("Fullscreen", &fullScreen);
            ImGui::Checkbox("Show fps", &showFps);
            if (fullScreen)
                glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, width, height, 60);
            else
                glfwSetWindowMonitor(window,nullptr, 50, 50, width-50, height-50, 60);
            {
                ImGui::Text("Basis: ");
                float e1[3] = { cube->Basis[0].x, cube->Basis[0].y, cube->Basis[0].z };
                float e2[3] = { cube->Basis[1].x, cube->Basis[1].y, cube->Basis[1].z };
                float e3[3] = { cube->Basis[2].x, cube->Basis[2].y, cube->Basis[2].z };

                ImGui::DragFloat3("e1", e1, 0.1);
                ImGui::DragFloat3("e2", e2, 0.1);
                ImGui::DragFloat3("e3", e3, 0.1);
                cube->Basis = glm::mat3({ e1[0], e1[1],e1[2] }, { e2[0], e2[1],e2[2] }, { e3[0], e3[1],e3[2] });

                if (ImGui::Button("Reset basis"))
                    cube->ResetBasis();
            }
            if (ImGui::CollapsingHeader("Perspective"))
            {
                auto fieldOfView=viewer->Fov, aspect=viewer->Aspect, near=viewer->Near, far= viewer->Far;
                PrintPerspectiveMatrixDescription();
                ImGui::SliderFloat("fov", &fieldOfView, 0.f, 90.f);
                ImGui::SliderFloat("aspect", &aspect, 0.1, 3.0);
                ImGui::SliderFloat("near", &near, 0.1, 100.0);
                ImGui::SliderFloat("far", &far, 0.1, 500.0);
                viewer->Aspect = aspect;
                viewer->Fov = fieldOfView;
                viewer->Near = near;
                viewer->Far = far;
                if (ImGui::Button("Reset Perspective options"))
                    viewer->ResetProjection();
            }
            if (ImGui::CollapsingHeader("Rotation"))
            {
                PrintRotationMatrixDescription();
                ImGui::SliderFloat("phi", &cube->phi, -180, 180);
                ImGui::SliderFloat("theta", &cube->theta, -180, 180);
                ImGui::SliderFloat("gamma", &cube->gamma, -180, 180);
                if (ImGui::Button("Reset Rotation"))
                    cube->ResetRotation();
            }
            if (ImGui::CollapsingHeader("Size"))
            {
                PrintScaleMatrixDescription();
                auto cubeHeight = cube->Size.y, cubeWidth = cube->Size.x, cubeDepth = cube->Size.z;
                ImGui::SliderFloat("height", &cubeHeight, 0.1, 3.0);
                ImGui::SliderFloat("width", &cubeWidth, 0.1, 3.0);
                ImGui::SliderFloat("depth", &cubeDepth, 0.1, 3.0);
                cube->Size = {cubeWidth, cubeHeight, cubeDepth};

                if (ImGui::Button("Reset size"))
                    cube->ResetSize();
            }
            if (ImGui::CollapsingHeader("Translate"))
            {
                PrintTranslateMatrixDescription();
                auto positionX = cube->Position.x, positionY = cube->Position.y, positionZ = cube->Position.z;
                ImGui::SliderFloat("px", &positionX, -1.0, 1.0);
                ImGui::SliderFloat("py", &positionY, -1.0, 1.0);
                ImGui::SliderFloat("pz", &positionZ, -1.0, 1.0);
                cube->Position = { positionX, positionY, positionZ };
                if (ImGui::Button("Reset position"))
                    cube->ResetPosition();
            }
            if (ImGui::CollapsingHeader("Shears"))
            {
                PrintShearsMatrixDescription();
                ImGui::SliderFloat("hxy", &cube->shearsX.y, -90.0, 90.0);
                ImGui::SliderFloat("hxz", &cube->shearsX.z, -90.0, 90.0);
                ImGui::SliderFloat("hyx", &cube->shearsY.x, -90.0, 90.0);
                ImGui::SliderFloat("hyz", &cube->shearsY.z, -90.0, 90.0);
                ImGui::SliderFloat("hzx", &cube->shearsZ.x, -90.0, 90.0);
                ImGui::SliderFloat("hzy", &cube->shearsZ.y, -90.0, 90.0);
                if (ImGui::Button("Reset shears"))
                    cube->ResetShears();
            }
            if (ImGui::CollapsingHeader("Spherical coordinates for viewer"))
            {
                PrintSphericalCoordinatesDescription();
                auto theta = viewer->Pitch,
                      phi = viewer->Yaw,
                      radius = viewer->DistanceToTarget;

                ImGui::SliderFloat("theta", &theta, -180.0, 180.0);
                ImGui::SliderFloat("phi", &phi, -180.0, 180.0);
                ImGui::SliderFloat("radius", &radius, 0.5, 60.0);
                viewer->Pitch = theta;
                viewer->Yaw = phi;
                viewer->DistanceToTarget = radius;
                if (ImGui::Button("Reset spherical coordinates"))
                {
                    viewer->ResetSpherical();
                }
            }

            if(showFps)
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);

        
        glBindVertexArray(VAO);
        shader->setMat4("view", viewer->View);
        shader->setMat4("projection", viewer->Projection);
        glDrawArrays(GL_POINTS, 0, sizeof(vertices));
        glBindVertexArray(0);
        viewer->Update();
        axis->Update();
        cube->Update();

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

//user callbacks
void OnScroll(GLFWwindow* window, double, double yoffset)
{
    viewer->DistanceToTarget -= yoffset * 0.5;
}

void OnKeyboard(GLFWwindow* window, int key, int scancode, int actions, int mods)
{
    if (key == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

bool firstMouse = true;
double lastX = width / 2, lastY = height / 2;
bool mouseClicked = false;
void OnMouse(GLFWwindow* window, double xpos, double ypos)
{
    if (mouseClicked)
    {
        if(xpos <= 2*width/3)
        if (canRotate)
        {
            if (firstMouse) // эта переменная была проинициализирована значением true
            {
                lastX = xpos;
                lastY = ypos;
                firstMouse = false;
            }
            viewer->Yaw += xpos - lastX;
            viewer->Pitch -= lastY - ypos;
            lastX = xpos;
            lastY = ypos;
        }
    }
}

void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
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

void PrintPerspectiveMatrixDescription()
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
}

void PrintScaleMatrixDescription()
{
    ImGui::Text("Scale matrix:");
    ImGui::Text("\
|x'|   | width  0       0     |   |x| \n\
|  |   |                      |   | |       \n\
|y'| = | 0      height  0     | * |y| \n\
|  |   |                      |   | |       \n\
|z'|   | 0      0       depth |   |z| \n\
                    ");
    ImGui::Separator();
    ImGui::BulletText("x, y, z old coordinates");
    ImGui::BulletText("x', y', z' new coordinates");
    ImGui::Separator();

}

void PrintTranslateMatrixDescription()
{
    ImGui::Text("Translate matrix:");
    ImGui::Text("\
|x'|   | 1   0    0    px |   |x| \n\
|  |   |                  |   | |       \n\
|y'|   | 0   1    0    py |   |y| \n\
|  | = |                  | * | |       \n\
|z'|   | 0   0    1    pz |   |z| \n\
|  |   |                  |   | |       \n\
|1 |   | 0   0    0    1  |   |1| \n\
                    ");
    ImGui::Separator();
    ImGui::BulletText("x, y, z old coordinates");
    ImGui::BulletText("x', y', z' new coordinates");
    ImGui::Separator();
}

void PrintRotationMatrixDescription()
{
    ImGui::Text("Rotate matrices matrix:");
    ImGui::Separator();
    ImGui::Text("Rotating around Z axis (Rz)");
    ImGui::Text("\
|x'|   | cos(phi) -sin(phi) 0 |   |x| \n\
|  |   |                      |   | |       \n\
|y'| = | sin(phi) cos(phi)  0 | * |y| \n\
|  |   |                      |   | |       \n\
|z'|   | 0        0         1 |   |z| \n\
                    ");
    ImGui::Separator();
    ImGui::BulletText("x, y, z old coordinates");
    ImGui::BulletText("x', y', z' new coordinates");

    ImGui::Separator();
    ImGui::Text("Rotating around X axis (Rx)");
    ImGui::Text("\
|x'|   | 1  0           0           |   |x| \n\
|  |   |                            |   | |       \n\
|y'| = | 0  cos(theta)  -sin(theta) | * |y| \n\
|  |   |                            |   | |       \n\
|z'|   | 0  sin(theta)  cos(theta)  |   |z| \n\
                    ");
    ImGui::Separator();
    ImGui::BulletText("x, y, z old coordinates");
    ImGui::BulletText("x', y', z' new coordinates");

    ImGui::Separator();
    ImGui::Text("Rotating around Y axis (Ry)");
    ImGui::Text("\
|x'|   | cos(gamma)  0  -sin(gamma) |   |x| \n\
|  |   |                            |   | |       \n\
|y'| = | 0           1  0           | * |y| \n\
|  |   |                            |   | |       \n\
|z'|   | sin(gamma)  0  cos(gamma)  |   |z| \n\
                    ");
    ImGui::Separator();
    ImGui::BulletText("x, y, z old coordinates");
    ImGui::BulletText("x', y', z' new coordinates");

}

void PrintShearsMatrixDescription()
{}

void PrintMatrixDescription()
{}

void PrintSphericalCoordinatesDescription()
{}
