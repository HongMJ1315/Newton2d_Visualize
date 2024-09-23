#include<iostream>
#include<algorithm>
#include<vector>
#include<utility>
#include<glm/glm.hpp>

#include<iomanip>
#include"newton2d.h"

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


/*
[∂f/∂x, ∂f/∂y]
[∂g/∂x, ∂g/∂y]
*/


int main(){
    /*
    Vector2d nowPoint, lastPoint;
    PolynomialMatrix2d jacobian = get_jacobian_matrix(f, g);
    nowPoint = {20, 20};
    std::vector<Vector2d> res = newton2d(f, g, nowPoint);
    for(auto i:res){
        std::cout << std::fixed << std::setprecision(6) << i[0] << " " << i[1] << std::endl;
    }
*/

}