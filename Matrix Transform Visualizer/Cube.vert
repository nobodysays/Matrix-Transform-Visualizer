#version 410 core
layout (location = 0) in vec3 position;

uniform mat4 view;
uniform mat4 projection;

uniform mat4 translateTransform;
uniform mat4 scaleTransform;
uniform mat4 rotationTransform;
uniform mat4 shearsTransform;
void main(){
    mat4 model = rotationTransform*translateTransform*scaleTransform*shearsTransform;
	gl_Position = projection*view*model*vec4(position, 1);
}