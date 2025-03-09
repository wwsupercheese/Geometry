#version 330
precision highp float;

in vec2 v_position;
uniform float uScale;
layout(location = 0) out vec4 o_color;

void main() {
    int max_iterations = int(log(uScale + 1.0) * 100);
    float escape_radius = 2.0;

    vec2 c = v_position * 1.5  / 1 - vec2(0.7, 0);
    vec2 z = vec2(0.0);

    int iterations;
    for (iterations = 0; iterations < max_iterations; iterations++) {
        // z = z^2 + c
        float x = z.x * z.x - z.y * z.y; // real
        float y = 2.0 * z.x * z.y; // image
        
        z = vec2(x, y) + c;

        if (length(z) > escape_radius) {
            break;
        }
    }
    
    float color = float(iterations) / float(max_iterations);

    vec3 col;
    if (iterations < max_iterations) {
        float smoothFactor = float(iterations) + 1.0 - log(log(length(z))) / log(2.0);
        color = smoothFactor / float(max_iterations);
        
        col = vec3(
            0.5 + 0.5 * sin(color * 10.0),
            0.5 + 0.5 * sin(color * 15.0 + 1.0),
            0.5 + 0.5 * sin(color * 20.0 + 2.0)
        );
    } else {
        col = vec3(0.0);
    }

    o_color = vec4(col, 1.0);
}
