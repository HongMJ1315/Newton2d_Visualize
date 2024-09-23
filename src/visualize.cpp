#include"visualize.h"
#include<iostream>
int set_vao(){

    // 设置顶点数据（覆盖整个屏幕的四边形）
    float vertices[] = {
        // 位置坐标
        -1.0f, -1.0f, // 左下角
         1.0f, -1.0f, // 右下角
         1.0f,  1.0f, // 右上角
        -1.0f,  1.0f  // 左上角
    };

    unsigned int indices[] = {
        0, 1, 2, // 第一个三角形
        2, 3, 0  // 第二个三角形
    };

    unsigned int VAO, VBO, EBO;

    // 生成 VAO、VBO、EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // 绑定 VAO
    glBindVertexArray(VAO);

    // 绑定并设置 VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 绑定并设置 EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 设置顶点属性指针
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    std::cout << "vao: " << VAO << std::endl;
    return VAO;
}