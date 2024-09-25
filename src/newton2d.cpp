#include"newton2d.h"

Polynomial get_partial_derivative(Polynomial p, char var){
    Polynomial result;
    for(auto it = p.begin(); it != p.end(); it++){
        if(it->first == var){
            for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++){
                result[var].push_back({it2->coef * it2->power, it2->power - 1});
            }
        }
    }
    return result;
}

PolynomialMatrix2d get_jacobian_matrix(Polynomial f, Polynomial g){
    PolynomialMatrix2d jacobian;
    jacobian(0, 0) = get_partial_derivative(f, 'x');
    jacobian(0, 1) = get_partial_derivative(f, 'y');
    jacobian(1, 0) = get_partial_derivative(g, 'x');
    jacobian(1, 1) = get_partial_derivative(g, 'y');
    return jacobian;
}

double get_func_value(Polynomial p, Vector2d point){
    double result = 0;
    for(auto it = p.begin(); it != p.end(); it++){
        if(it->first == 'c'){
            result += it->second[0].coef;
            continue;
        }
        for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
            result += it2->coef * std::pow(point[it->first - 'x'], it2->power);
    }
    return result;
}

Matrix2d get_jacobian_value(PolynomialMatrix2d jacobian, Vector2d point){
    Matrix2d result;
    for(int i = 0; i < 2; i++)
        for(int j = 0; j < 2; j++)
            result(i, j) = get_func_value(jacobian(i, j), point);
        
    return result;
}

std::vector<Vector2d> newton2d(Polynomial f, Polynomial g, Vector2d startPoint){
    std::vector<Vector2d> result;
    result.push_back(startPoint);
    Vector2d nowPoint, lastPoint;
    PolynomialMatrix2d jacobian = get_jacobian_matrix(f, g);
    nowPoint = startPoint;
    for(;;){
        lastPoint = nowPoint;
        Matrix2d jacobianValue = get_jacobian_value(jacobian, nowPoint);
        Vector2d fValue = {get_func_value(f, nowPoint), get_func_value(g, nowPoint)};
        nowPoint = lastPoint - jacobianValue.inverse() * fValue;
        double error = std::sqrt((nowPoint[0] - lastPoint[0]) * (nowPoint[0] - lastPoint[0]) + 
                        (nowPoint[1] - lastPoint[1]) * (nowPoint[1] - lastPoint[1]));
        if(error < ESP)
            break;
        result.push_back(nowPoint);
        double x = nowPoint[0], y = nowPoint[1];
        if(x != x || y != y){
            break;
        }
    }
    return result;
}