#version 330
precision highp float;

in vec2 v_position; // Входные координаты пикселя
uniform float uScale; // Параметр масштаба
layout(location = 0) out vec4 o_color; // Выходной цвет

void main() {
    int max_iterations = int(log(uScale + 1.0) * 100);
    float escape_radius = 2.0;

    // Преобразование координат для множества Мандельброта
    vec2 c = v_position * 1.5  / 1 - vec2(0.7, 0);
    vec2 z = vec2(0.0);

    int iterations;
    for (iterations = 0; iterations < max_iterations; iterations++) {
        // z = z^2 + c
        float x = z.x * z.x - z.y * z.y; // Реальная часть
        float y = 2.0 * z.x * z.y; // Мнимая часть
        
        z = vec2(x, y) + c;

        if (length(z) > escape_radius) {
            break;
        }
    }
    
    // Нормализация цвета на основе количества итераций
    float color = float(iterations) / float(max_iterations);

    // Генерация цвета с использованием синусоидальных функций для создания градиента
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
        col = vec3(0.0); // Цвет для точек, которые не выходят за пределы
    }

    o_color = vec4(clamp(col, 0.0, 1.0), 1.0);
}
