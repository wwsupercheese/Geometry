#version 330
in vec3 v_color;
in vec2 v_position;
layout(location = 0) out vec4 o_color;
void main() {
    int countRings = 20;
    float rad = length(v_position);
    float color = 1 - int(rad * (countRings + 1)) % 2;
    vec3 col = vec3(
        0.5 + 0.5 * sin(color * 10.0),
        0.5 + 0.5 * sin(color * 15.0 + 1.0),
        0.5 + 0.5 * sin(color * 20.0 + 2.0)
    );
    o_color = vec4(col, 1.0);
}
