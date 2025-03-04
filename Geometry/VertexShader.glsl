#version 330
layout(location = 0) in vec2 a_position;
layout(location = 1) in vec3 a_color;
out vec3 v_color;
out vec2 v_position;
void main() {
    v_color = a_color;
    v_position = a_position;
    gl_Position = vec4(a_position, 0.0, 1.0);
}