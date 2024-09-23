#ifndef VISUALIZE_H
#define VISUALIZE_H

#include"newton2d.h"
#include"glsl.h"

struct Vertex {
    glm::vec2 position;
    glm::vec3 color;
};


int set_vao();

std::pair<unsigned int, unsigned int> set_point_vao();

std::vector<Vertex> update_vbo(unsigned int, std::vector<glm::vec2>, int );
#endif
