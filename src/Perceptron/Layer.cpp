#include "../../include/Neural/Perceptron/Layer.h"

using namespace Neural;

Layer::Layer(int size, int nextSize) : size(size) {
    neurons.assign(size, 0);
    biases.assign(size, 0);
    weights.assign(size, std::vector<double>(nextSize, 0));
}

Layer::Layer(const Layer &copy) : size(copy.size), neurons(copy.neurons), biases(copy.biases), weights(copy.weights) {}