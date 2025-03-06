#version 330
precision highp float;
out vec4 o_color;
in vec2 v_position;
uniform float uScale; // Параметр масштаба

void main() {

    vec2 c = vec2(-0.7, 0.27015);
    vec2 z = (v_position * 2.0);
    float escape_radius = 2.0;
    int max_iterations = int(log(uScale + 1.0) * 150);
    int iterations;
    
    for (iterations = 0; iterations < max_iterations; iterations++) {
        float x = z.x * z.x - z.y * z.y + c.x; // Реальная часть
        float y = 2.0 * z.x * z.y + c.y; // Мнимая часть
        
        z = vec2(x, y);
        
        if (length(z) > escape_radius) {
            break;
        }
    }
    
    // Нормализация цвета на основе количества итераций
    float color = float(iterations) / float(max_iterations);
    vec3 col = vec3(
        0.5 + 0.5 * sin(color * 10.0),
        0.5 + 0.5 * sin(color * 15.0 + 1.0),
        0.5 + 0.5 * sin(color * 20.0 + 2.0)
    );
    o_color = vec4(vec3(col), 1.0); // Цвет на основе количества итераций
}
