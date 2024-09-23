#include"visualize.h"
#include<iostream>
int set_vao(){

    float vertices[] = {
        -1.0f, -1.0f, 
         1.0f, -1.0f, 
         1.0f,  1.0f, 
        -1.0f,  1.0f  
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0 
    };

    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    std::cout << "vao: " << VAO << std::endl;
    return VAO;
}
std::pair<unsigned int, unsigned int> set_point_vao(){
    unsigned int lineVAO, lineVBO;
    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);

    // 绑定 VAO
    glBindVertexArray(lineVAO);

    // 绑定 VBO
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);

    // 设置顶点属性指针
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    // 解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return {lineVAO, lineVBO};

}

std::vector<Vertex> update_vbo(unsigned int vbo, std::vector<glm::vec2> points, int showPoint = -1){
    if(showPoint == -1) showPoint = points.size();
    std::vector<Vertex> vertices;
    for (size_t i = 0; i < std::min((int)points.size(), showPoint); ++i){
        Vertex vertex;
        vertex.position = glm::vec2(points[i]);
        if (i == 0){
            vertex.color = glm::vec3(1.0f, 0.0f, 0.0f);
        }
        else if (i == points.size() - 1){
            vertex.color = glm::vec3(0.0f, 0.0f, 1.0f);
        }
        else{
            vertex.color = glm::vec3(1.0f, 1.0f, 1.0f);
        }

        vertices.push_back(vertex);
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return vertices;
}