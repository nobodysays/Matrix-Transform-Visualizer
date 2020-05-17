#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <GL/glew.h>          
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Shader.h"

const double width = 1920;
const double height = 1080;
float ClampFloat(float value, float min, float max)
{
    if (value < min)
        value = min;
    if (value > max)
        value = max;
    return value;
}
glm::vec2 ClampVec2(glm::vec2 value, float min, float max)
{
    value.x = ClampFloat(value.x, min, max);
    value.y = ClampFloat(value.y, min, max);
    return value;
}
glm::vec3 ClampVec3(glm::vec3 value, float min, float max)
{
    value.x = ClampFloat(value.x, min, max);
    value.y = ClampFloat(value.y, min, max);
    value.z = ClampFloat(value.z, min, max);
    return value;
}
