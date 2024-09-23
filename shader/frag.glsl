// frag.glsl

#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

void main()
{
    float x = TexCoord.x * 4.0; // x ∈ [-4, 4]
    float y = TexCoord.y * 3.0; // y ∈ [-3, 3]

    float value1 = (x * x) / 9.0 + (y * y) / 4.0 - 1.0;

    float value2 = x * x - y - 4.0;

    float thickness = 0.03;

    float alpha1 = 1.0 - smoothstep(0.0, thickness, abs(value1));
    float alpha2 = 1.0 - smoothstep(0.0, thickness, abs(value2));

    vec3 color1 = vec3(1.0, 0.0, 0.0); 
    vec3 color2 = vec3(0.0, 1.0, 0.0); 
    vec3 overlapColor = vec3(0.0, 0.0, 1.0); 

    vec3 color;
    float alpha;

    if (alpha1 > 0.0 && alpha2 > 0.0){
        color = overlapColor;
        alpha = max(alpha1, alpha2);
    }
    else if (alpha1 > 0.0){
        color = color1;
        alpha = alpha1;
    }
    else if (alpha2 > 0.0){
        color = color2;
        alpha = alpha2;
    }
    else{
        discard; 
    }

    FragColor = vec4(color, alpha);
}
