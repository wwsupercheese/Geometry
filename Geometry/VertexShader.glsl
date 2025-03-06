#version 330
layout(location = 0) in vec2 a_position;
layout(location = 1) in vec3 a_color;
uniform vec2 uOffset;
uniform float uScale;
out vec3 v_color;
out vec2 v_position;

void main() {
    vec2 scaledPos = a_position * uScale; // NEW

    gl_Position = vec4((a_position * uScale) + vec2(uOffset.x, uOffset.y), 0.0, 1.0);
    v_color = a_color;
    v_position = a_position;
}