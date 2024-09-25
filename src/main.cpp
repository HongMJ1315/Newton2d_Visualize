#include<iostream>
#include<algorithm>
#include<vector>
#include<utility>
#include<glm/glm.hpp>
#include<opencv4/opencv2/opencv.hpp>
#include<iomanip>
#include<fstream>

#include"newton2d.h"
#include"visualize.h"
#include"glsl.h"
#include "GLinclude.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 400");

    static char x_input[64] = "0.001";
    static char y_input[64] = "1";
    static char speed_input[64] = "1";
// /*
    std::vector<glm::vec2> allPointList[9][7];
    for(int i = -4; i < 5; i++){
        for(int j = -3; j < 4; j++){
            Vector2d start = {i, j};
            // if(i == 0) start = {0.0001, j};
            auto newtonResult = newton2d(f, g, start);
            for(auto k:newtonResult){
                // std::cout << std::fixed << std::setprecision(7) << "( " << k[0] << ", " << k[1]  << " )"<< std::endl;        
                allPointList[i + 4][j + 3].push_back({k[0], k[1]});
            }
        }
    }
// */
    int showPoint = 1;

    int nxt = 0;

    float speed = 0.5;
    // get now time
    float lastTime = glfwGetTime();
    std::vector<glm::vec2> pointList;
    std::string res;

    bool runAll = false;
    bool running = true;
    bool showPointStatus = false;

    while(!glfwWindowShouldClose(window)){
        float nowTime = glfwGetTime();      
        int pointListI = nxt / 7, pointListJ = nxt % 7;
        if(nowTime - lastTime > speed && running){
            if(showPoint < pointList.size()) {
                showPoint++;
            }
            else if(runAll){
                if(nxt < 62){
                    nxt++, showPoint = 1;
                    pointListI = nxt / 7, pointListJ = nxt % 7;
                    pointList = allPointList[pointListI][pointListJ];
                }
                else{
                    nxt= 0;
                    runAll = false;
                }
            }  
            lastTime = nowTime;

        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Input Window"); 

            ImGui::InputText("X", x_input, IM_ARRAYSIZE(x_input));
            ImGui::InputText("Y", y_input, IM_ARRAYSIZE(y_input));
            ImGui::InputText("Speed", speed_input, IM_ARRAYSIZE(speed_input));

            float x = 0, y = 0;

            if (ImGui::Button("Run")) {
                runAll = false;
                std::string x_value(x_input);
                std::string y_value(y_input);

                x = std::stod(x_value), y = std::stof(y_value);
                showPoint = 1;
                lastTime = nowTime;
                Vector2d start = {x, y};
                auto newtonResult = newton2d(f, g, start);
                pointList.clear();
                for(auto i:newtonResult)
                    pointList.push_back({i[0], i[1]});
            }
            ImGui::SameLine();

            if (ImGui::Button("Set Speed")) {
                std::string s = std::string(speed_input);
                if(s.size() == 0) speed = speed;
                else
                    speed = std::stod(std::string(speed_input));
            }

            ImGui::SameLine();

            if(ImGui::Button("Run All")){
                for(int j = -3; j < 4; j++){
                    allPointList[0 + 4][j + 3].clear();
                    Vector2d start = {0.0, j};
                    auto newtonResult = newton2d(f, g, start);
                    for(auto k:newtonResult){
                        allPointList[0 + 4][j + 3].push_back({k[0], k[1]});
                    }
                }
                nxt = 0;
                showPoint = 1;
                lastTime = nowTime;
                pointList = allPointList[0][0];
                runAll = true;
            }
            
            if(ImGui::Button("Run All whitout divergence")){
                for(int j = -3; j < 4; j++){
                    allPointList[0 + 4][j + 3].clear();
                    Vector2d start = {0.0001, j};
                    auto newtonResult = newton2d(f, g, start);
                    for(auto k:newtonResult){
                        allPointList[0 + 4][j + 3].push_back({k[0], k[1]});
                    }
                }
                nxt = 0;
                showPoint = 1;
                lastTime = nowTime;
                pointList = allPointList[0][0];
                runAll = true;
            }
            
            if(ImGui::Button("Stop")){
                running = false;
            }

            ImGui::SameLine();

            if(ImGui::Button("Continue")){
                running = true;
            }

            ImGui::SameLine();

            if(ImGui::Button("Save Image")){
                unsigned char *buffer = new unsigned char[3 * width * height];
                glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer);
                cv::Mat img(height, width, CV_8UC3, buffer);
                cv::flip(img, img, 0);
                cv::cvtColor(img, img, cv::COLOR_RGB2BGR);
                cv::imwrite("screenshot.jpg", img);
                delete[] buffer;

            }

            if(ImGui::Button("Show Point Status")){
                showPointStatus = !showPointStatus;
            }

            ImGui::Text("(%lf, %lf) => step: %d", (pointList.size() == 0 ? 0 : pointList[0].x), (pointList.size() == 0 ? 0 : pointList[0].y), (int)pointList.size());
            for(int i = 0; i < pointList.size(); i++){
                if(pointList[i].x != pointList[i].x || pointList[i].y != pointList[i].y){
                    ImGui::Text("Divergence at step %d", i);
                    break;
                }
                ImGui::Text("(%lf, %lf)", pointList[i].x, pointList[i].y);

            }

            ImGui::End();
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(func_shader);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glUseProgram(point_shader);

        glBindVertexArray(pointVao);

        if(showPointStatus){
            // Show Point Status
            // if start point leads to divergence show red point
            // if start point leads to convergence show green point
            for(int i = -4; i < 5; i++){
                for(int j = -3; j < 4; j++){
                    std::vector<Vertex> vertices = update_point_vbo(pointVbo, allPointList[i + 4][j + 3]); 
                    glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
                    glPointSize(8.0f);
                    glDrawArrays(GL_POINTS, 0, vertices.size());
                }
            }
        }

        if(pointList.size() != 0){
            std::vector<Vertex> vertices = update_vbo(pointVbo, pointList, showPoint); 

            glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

            glPointSize(8.0f);
            glDrawArrays(GL_POINTS, 0, vertices.size());

            glBindVertexArray(0);
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}