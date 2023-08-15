#include <Neural/Activation/Functions.hpp>

double Neural::sigmoid(double arg){
    return 1 / (1 + exp(-arg));
}

double Neural::stair_step(double arg){
    return arg > 0;
}

double Neural::tanh(double arg){
    return 2 * Neural::sigmoid(2 * arg) - 1;
}

double Neural::ReLu(double arg){
    return arg > 0 ? arg : 0;
}