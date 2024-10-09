#version 410 core

out vec4 FragColor;
in vec2 TexCoord;

void main() {
    float x = float(TexCoord.x) * 5.0; // 将 TexCoord 转换为 float 类型
    float y = float(TexCoord.y) * 4.0;

    // 定义方程
    // float value1 = (x * x) / 9.0 + (y * y) / 4.0 - 1.0;

    // float value2 = x * x - y - 4.0;

    float value1 = (x * x * x) / 9.0 + (y * y * y) / 10.0 + (y * y) / 5.0;

    float value2 = x * x * x * x + x * x * x - x * x * 10 - 8 * x - y;
 

    // 使用 fwidth 动态调整粗细
    float thickness = 1;
    float gradient1 = fwidth(value1);
    float gradient2 = fwidth(value2);

    float alpha1 = 1.0 - smoothstep(0.0, thickness * gradient1, abs(value1));
    float alpha2 = 1.0 - smoothstep(0.0, thickness * gradient2, abs(value2));

    // 定义颜色
    vec3 color1 = vec3(1.0, 0.0, 0.0);  // 红色
    vec3 color2 = vec3(0.0, 1.0, 0.0);  // 绿色
    vec3 overlapColor = vec3(0.0, 0.0, 1.0); // 蓝色（重叠部分）

    vec3 color;
    float alpha;

    // 网格线颜色和粗细
    vec3 gridColor = vec3(0.5, 0.5, 0.5); // 灰色
    float gridThickness = 0.01;

    // 计算距离最近整数的距离，用于网格线
    float xGridDist = abs(x - round(x));
    float yGridDist = abs(y - round(y));

    // 设定坐标轴线更粗
    if ((abs(round(x) - x) < 0.03 && round(x) == 0) || (abs(round(y) - y) < 0.03 && round(y) == 0)) {
        gridThickness = 0.03;
        gridColor = vec3(1, 1, 1); // 白色
    }

    float xGridAlpha = 1.0 - smoothstep(0.0, gridThickness, xGridDist);
    float yGridAlpha = 1.0 - smoothstep(0.0, gridThickness, yGridDist);
    float gridAlpha = max(xGridAlpha, yGridAlpha);

    // 颜色和透明度选择
    if (alpha1 > 0.0 && alpha2 > 0.0) {
        color = overlapColor;
        alpha = max(alpha1, alpha2);
    } else if (alpha1 > 0.0) {
        color = color1;
        alpha = alpha1;
    } else if (alpha2 > 0.0) {
        color = color2;
        alpha = alpha2;
    } else if (gridAlpha > 0.0) {
        color = gridColor;
        alpha = gridAlpha;
    } else {
        discard; 
    }

    FragColor = vec4(color, alpha);
}
