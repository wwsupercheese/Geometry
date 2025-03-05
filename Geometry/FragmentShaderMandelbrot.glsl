#version 330
in vec3 v_color;
in vec2 v_position;
layout(location = 0) out vec4 o_color;
void main() {
    int max_iterations = 100;

    float escape_radius = 2.0;
    vec2 c = v_position * 1.5 - vec2(0.7, 0);
    vec2 z = vec2(0.0);

    int iterations = 0;
    for (int i = 0; i < max_iterations; i++) {
        // z = z^2 + c
        z = vec2(pow(z.x, 2) - pow(z.y, 2), 2 * z.x * z.y) + c;

        if (length(z) > escape_radius) {
            break;
        }
        iterations++;
    }
    
    float color = float(iterations) / float(max_iterations);

    vec3 col = vec3(
        0.5 + 0.5 * sin(color * 10.0),
        0.5 + 0.5 * sin(color * 15.0 + 1.0),
        0.5 + 0.5 * sin(color * 20.0 + 2.0)
    );
    o_color = vec4(col, 1.0);
}


