#version 330
in vec3 v_color;
in vec2 v_position;
layout(location = 0) out vec4 o_color;
void main() {
    float PI = 3.14;
    int countCrest = 3;
    int countWaves = 20;
    float color = int((v_position.y + 1) * countWaves + 2 * sin(v_position.x * countCrest * PI - PI)) % 2;
    vec3 p = vec3(color);
    o_color = vec4(p, 1.0);
}


