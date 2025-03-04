#version 330
in vec3 v_color;
in vec2 v_position;
layout(location = 0) out vec4 o_color;
void main() {
    int countRings = 20;
    float rad = length(v_position);
    float color = 1 - int(rad * (countRings + 1)) % 2;
    vec3 p = vec3(color);
    o_color = vec4(p, 1.0);
}


