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


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, reshape);
    std::cout << "GLFW version: " << glfwGetVersionString() << std::endl;

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    auto vao = set_vao();
    auto func_shader = setGLSLshaders("shader/vert.glsl", "shader/frag.glsl");
    while(!glfwWindowShouldClose(window)){


        // 清除颜色缓冲
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // 使用着色器程序
        glUseProgram(func_shader);

        // 绑定 VAO 并绘制
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // 交换缓冲区和轮询事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}