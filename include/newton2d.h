#ifndef NEWTON2D_H
#define NEWTON2D_H

#include<Eigen/Dense>
#include<Eigen/Core>
#include<map>
#include<iostream>
#include<iomanip>
#define ESP 1e-6

struct term{
    double coef;
    int power;
};

typedef Eigen::Vector2d Vector2d;
typedef std::map<char, std::vector<term>> Polynomial;
typedef Eigen::Matrix<Polynomial, 2, 2> PolynomialMatrix2d;
typedef Eigen::Matrix2d Matrix2d;

std::ostream& operator<<(std::ostream&, Polynomial);
Polynomial get_partial_derivative(Polynomial, char);
PolynomialMatrix2d get_jacobian_matrix(Polynomial, Polynomial);
double get_func_value(Polynomial, Vector2d);
Matrix2d get_jacobian_value(PolynomialMatrix2d, Vector2d);
std::vector<Vector2d> newton2d(Polynomial, Polynomial, Vector2d);
#endif