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

    std::vector<glm::vec2> pointList[9][7];
    for(int i = -4; i < 5; i++){
        for(int j = -3; j < 4; j++){
            Vector2d start = {i, j};
            // if(i == 0) start = {0.0001, j};
            auto newtonResult = newton2d(f, g, start);
            std::cout<< "(" << i << ", " << j << ") => " << newtonResult.size() << " steps" << std::endl; 
            for(auto k:newtonResult){
                std::cout << std::fixed << std::setprecision(7) << "( " << k[0] << ", " << k[1]  << " )"<< std::endl;        
                pointList[i + 4][j + 3].push_back({k[0], k[1]});
            }
            std::cout << "====================" << std::endl;
        }
    }

    int showPoint = 1;

    int nxt = 0;

    // get now time
    float lastTime = glfwGetTime();
    while(!glfwWindowShouldClose(window)){
        float nowTime = glfwGetTime();

        int pointListI = nxt / 7, pointListJ = nxt % 7;
        if(nowTime - lastTime > 0.05){
            if(showPoint < pointList[pointListI][pointListJ].size()) {
                showPoint++;
                
            }
            else{
                if(nxt < 62){
                    nxt++, showPoint = 1;
                    pointListI = nxt / 7, pointListJ = nxt % 7;
                }
                else{
                    nxt= 0;
                    for(int i = -4; i < 5; i++){
                        for(int j = -3; j < 4; j++){
                            pointList[i + 4][j + 3].clear();
                            Vector2d start = {i, j};
                            if(i == 0) start = {0.0001, j};
                            auto newtonResult = newton2d(f, g, start);
                            std::cout<< "(" << i << ", " << j << ") => " << newtonResult.size() << " steps" << std::endl; 
                            for(auto k:newtonResult){
                                pointList[i + 4][j + 3].push_back({k[0], k[1]});
                            }
                            std::cout << "====================" << std::endl;
                        }
                    }

                }
            }
            lastTime = nowTime;
        }
        

        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(func_shader);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glUseProgram(point_shader);

        glBindVertexArray(pointVao);

        // for(int i = 0; i < 63; i++){
        //     int pointListI = i / 7, pointListJ = i % 7;

        //     std::vector<Vertex> vertices = update_vbo(pointVbo, pointList[pointListI][pointListJ], -1); 
        //     glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

        //     glPointSize(8.0f);
        //     glDrawArrays(GL_POINTS, 0, vertices.size());
        // }
        std::vector<Vertex> vertices = update_vbo(pointVbo, pointList[pointListI][pointListJ], showPoint); 

        glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

        glPointSize(8.0f);
        glDrawArrays(GL_POINTS, 0, vertices.size());

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}