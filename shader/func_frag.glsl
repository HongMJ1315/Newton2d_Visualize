// func_frag.glsl

#version 410 core

out vec4 FragColor;

in vec2 TexCoord;

void main(){
    double x = double(TexCoord.x) * 5.0; // x ∈ [-4, 4]
    double y = double(TexCoord.y) * 4.0; // y ∈ [-3, 3]

    double value1 = (x * x) / 9.0 + (y * y) / 4.0 - 1.0;

    double value2 = x * x - y - 4.0;

    double thickness = 0.05;

    //smoothstep 插植 
    double alpha1 = 1.0 - smoothstep(0.0, thickness, abs(value1)); // value1 = 0 為在線上
    double alpha2 = 1.0 - smoothstep(0.0, thickness, abs(value2));

    vec3 color1 = vec3(1.0, 0.0, 0.0);
    vec3 color2 = vec3(0.0, 1.0, 0.0);
    vec3 overlapColor = vec3(0.0, 0.0, 1.0);

    vec3 color;
    double alpha;

    vec3 gridColor = vec3(0.5, 0.5, 0.5);

    double gridThickness = 0.01;

    // x, y 被四捨五入回整數點
    double xGridDist = abs(x - round(x));
    double yGridDist = abs(y - round(y));

    double xGridAlpha = 1.0 - smoothstep(0.0, gridThickness, xGridDist);
    double yGridAlpha = 1.0 - smoothstep(0.0, gridThickness, yGridDist);

    double gridAlpha = max(xGridAlpha, yGridAlpha);

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
    else if (gridAlpha > 0.0){
        color = gridColor;
        alpha = gridAlpha;
    }
    else{
        discard; 
    }

    FragColor = vec4(color, float(alpha));
}
