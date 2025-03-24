#version 330
in vec3 v_color;
in vec3 v_position;
layout(location = 0) out vec4 o_color;
void main() {
    float r = 0.2;
    bool cardXY = pow((pow((v_position.y - 0.35), 2) + pow(v_position.x, 2) + 2 * r * (v_position.y - 0.35)), 2) <= 4 * pow(r, 2) * (pow((v_position.y - 0.35), 2) + pow(v_position.x, 2));
    bool cardXZ = pow((pow((v_position.z - 0.35), 2) + pow(v_position.x, 2) + 2 * r * (v_position.z - 0.35)), 2) <= 4 * pow(r, 2) * (pow((v_position.z - 0.35), 2) + pow(v_position.x, 2));
    bool cardYZ = pow((pow((v_position.z - 0.35), 2) + pow(v_position.y, 2) + 2 * r * (v_position.z - 0.35)), 2) <= 4 * pow(r, 2) * (pow((v_position.z - 0.35), 2) + pow(v_position.y, 2));
    
    
    float color = float(cardXY || cardXZ || cardYZ);
    vec3 p = vec3(1, 1 - color, 1 - color);
    o_color = vec4((1 - color) * v_color + color * p, 1.0);
}


