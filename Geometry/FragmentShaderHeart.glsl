#version 330
in vec3 v_color;
in vec2 v_position;
layout(location = 0) out vec4 o_color;
void main() {
    float r = 0.2;
    bool card = pow((pow((v_position.y - 0.5), 2) + pow(v_position.x, 2) + 2 * r * (v_position.y - 0.5)), 2) <= 4 * pow(r, 2) * (pow((v_position.y - 0.5), 2) + pow(v_position.x, 2));
    float color = float(card);
    vec3 p = vec3(1, 1 - color, 1 - color);
    o_color = vec4(p, 1.0);
}


