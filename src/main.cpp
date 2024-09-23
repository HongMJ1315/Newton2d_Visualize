#include<iostream>
#include<algorithm>
#include<vector>
#include<utility>
#include<glm/glm.hpp>

#include<iomanip>

#include<fstream>
#include"newton2d.h"
#include"visualize.h"
#include"glsl.h"
#include "GLinclude.h"

// f(x, y) = 1/9 * x^2 + 1/4 * y^2 - 1
// g(x, y) = x^2 - y - 4
// key: 'x', 'y', 'c'（c for constant）
// value: vector of terms
Polynomial f = {
    {'x', {{1.0/9, 2}}},
    {'y', {{1.0/4, 2}}},
    {'c', {{-1, 0}}}
};
Polynomial g = {
    {'x', {{1, 2}}},
    {'y', {{-1, 1}}},
    {'c', {{-4, 0}}}
};
// Polynomial f = {
//     {'x', {{1.0, 2}}},
//     {'y', {{-5, 1}}},
//     {'c', {{-6, 0}}}
// };
// Polynomial g = {
//     {'x', {{3, 1}}},
//     {'y', {{-1, 2}}},
//     {'c', {{-8, 0}}}
// };


int width = 800;
int height = 600;

void reshape(GLFWwindow *window, int w, int h){
    width = w;
    height = h;
}
int main(int argc, char **argv){
    glutInit(&argc, argv);
    if(!glfwInit()){
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(width, height, "Hw1", nullptr, nullptr);
    if(!window){
        glfwTerminate();
        return -1;
    }


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, reshape);
    std::cout << "GLFW version: " << glfwGetVersionString() << std::endl;

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "GLSL version: " << glslVersion << std::endl;
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;
    
    auto vao = set_vao();
    auto pointVAOVBO = set_point_vao();
    unsigned int pointVao = pointVAOVBO.first;
    unsigned int pointVbo = pointVAOVBO.second;
    auto func_shader = setGLSLshaders("shader/func_vert.glsl", "shader/func_frag.glsl");
    auto point_shader = setGLSLshaders("shader/point_vert.glsl", "shader/point_frag.glsl");

    Vector2d start = {1,4};
    auto newtonReslut = newton2d(f, g, start);
    std::vector<glm::vec2> points, points2;

    for(auto i : newtonReslut){
        std::cout << i[0] << " " << i[1] << std::endl;
        points.push_back({i[0], i[1]}); 
    }

    start = {-4, 1};
    auto newtonReslut2 = newton2d(f, g, start);

    for(auto i : newtonReslut2){
        std::cout << i[0] << " " << i[1] << std::endl;
        points2.push_back({i[0], i[1]}); 
    }


    int showPoint = 1;


    // get now time
    float lastTime = glfwGetTime();
    while(!glfwWindowShouldClose(window)){
        float nowTime = glfwGetTime();
        // if(nowTime - lastTime > 2){
        //     if(show_point < points.size()) 
        //         show_point++;
        //     lastTime = nowTime;
        // }
        

        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(func_shader);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glUseProgram(point_shader);

        glBindVertexArray(pointVao);

        std::vector<Vertex> vertices = update_vbo(pointVbo, points, -1); 
        glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

        glPointSize(8.0f);
        glDrawArrays(GL_POINTS, 0, vertices.size());

        std::vector<Vertex> vertices2 = update_vbo(pointVbo, points2, -1); 
        glDrawArrays(GL_LINE_STRIP, 0, vertices2.size());

        glPointSize(8.0f);
        glDrawArrays(GL_POINTS, 0, vertices2.size());

        
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}