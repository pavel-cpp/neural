#include <Neural/Perceptron/Perceptron.hpp>

using namespace Neural;

double Perceptron::derivative(const double &x) {
    return activate(x) * (1.f - activate(x));
}

double Perceptron::addition(int first, int second) {
    double result(0);
    for(int i(0); i < layers[first].size; i++)
        result += layers[first].neurons[i] * layers[first].weights[i][second];
    return result + layers[first + 1 < layers.size() ? first + 1 : first].biases[second];
}

Perceptron::Perceptron(const std::vector<unsigned>& layer, double (*function)(double)) : activate(function), learningRate(0.1){
    for (int i = 0; i < layer.size(); ++i) {
        if(i + 1 == layer.size())
            layers.emplace_back(layer[i], 1);
        else
            layers.emplace_back(layer[i], layer[i + 1]);
    }

    srand(time(NULL));

    for (int l = 0; l < layers.size(); ++l) {
        for (int i = 0; i < layers[l].size; ++i) {
            for (int j = 0; j < layers[l].weights[i].size(); ++j) {
                layers[l].weights[i][j] = ((rand() % 10000 + 1.f) / 10000.f) * 4.f - 2.f; // TODO: Добавить функции рандома
            }
        }
    }
}

Neurons Perceptron::forwardFeed() {
    for (int l = 1; l < layers.size(); ++l) {
        for (int i = 0; i < layers[l].size; ++i) {
            layers[l].neurons[i] = addition(l - 1, i);
            layers[l].neurons[i] = activate(layers[l].neurons[i]);
        }
    }

    return layers.back().neurons;
}

Neurons Perceptron::forwardFeed(const std::vector<double>& input) {

    layers.front().neurons = std::move(input);

    for (int l = 1; l < layers.size(); ++l) {
        for (int i = 0; i < layers[l].size; ++i) {
            layers[l].neurons[i] = addition(l - 1, i);
            layers[l].neurons[i] = activate(layers[l].neurons[i]);
        }
    }

    return layers.back().neurons;
}

void Perceptron::setInput(const Neurons& input) {
    if(layers.front().neurons.size() != input.size()) throw std::runtime_error("Input error!");
    layers.front().neurons = std::move(input);
}

void Perceptron::backPropagation(const Neurons &target) {
    std::vector<double> error(layers[layers.size() - 1].size, 0);
    for (int i = 0; i < layers[layers.size() - 1].size; i++) {
        error[i] = target[i] - layers[layers.size() - 1].neurons[i];
    }

    for (int k = layers.size() - 2; k >= 0; k--) {
        std::vector<double> errorNext(layers[k].size, 0);
        std::vector<double> gradient(layers[k + 1].size, 0);

        for (int i = 0; i < layers[k + 1].size; i++) {
            gradient[i] = error[i] * derivative(layers[k + 1].neurons[i]);
            gradient[i] *= learningRate;
        }

        std::vector<std::vector<double>> delta(layers[k + 1].size, std::vector<double>(layers[k].size, 0));

        for (int i = 0; i < layers[k + 1].size; i++) {
            for (int j = 0; j < layers[k].size; j++) {
                delta[i][j] = gradient[i] * layers[k].neurons[j];
            }
        }

        for (int i = 0; i < layers[k].size; i++) {
            errorNext[i] = 0;
            for (int j = 0; j < layers[k + 1].size; j++) {
                errorNext[i] += layers[k].weights[i][j] * error[j];
            }
        }

        error.assign(layers[k].size, 0);

        error = errorNext;

        std::vector<std::vector<double>> weightsNew = layers[k].weights;
        for (int i = 0; i < layers[k + 1].size; i++) {
            for (int j = 0; j < layers[k].size; j++) {
                weightsNew[j][i] = layers[k].weights[j][i] + delta[i][j];
            }
        }

        layers[k].weights = weightsNew;

        for (int i = 0; i < layers[k + 1].size; i++) {
            layers[k + 1].biases[i] += gradient[i];
        }
    }
}

Perceptron::~Perceptron() = default;

bool Perceptron::saveToFile(const std::string &path) {
    using namespace std;
    ofstream gens(path);

    if(!gens.is_open()) return false;

    gens.precision(16);
    for (int i = 0; i < layers.size(); ++i) {

        if(i + 1 < layers.size())
            gens << layers[i].size << " " << layers[i + 1].size << " ";
        else
            gens << layers[i].size << " 1 ";

        for (int j = 0; j < layers[i].weights.size(); ++j) {
            for (int k = 0; k < layers[i].weights[j].size(); ++k) {
                gens << layers[i].weights[j][k] << " ";
            }
        }

        for (int j = 0; j < layers[i].biases.size(); ++j) {
            gens << layers[i].biases[j] << " ";
        }

    }
    return true;
}

bool Perceptron::loadFromFile(const std::string &path) {
    using namespace std;
    ifstream gens(path);

    layers.clear();

    if(!gens.is_open()) return false;

    gens.precision(16);

    int layerCount = 0;

    while (!gens.eof()) {

        unsigned current, next;

        gens >> current >> next;

        layers.emplace_back(current, next);

        for (int j = 0; j < layers[layerCount].weights.size(); ++j) {
            for (int k = 0; k < layers[layerCount].weights[j].size(); ++k) {

                gens >> layers[layerCount].weights[j][k];

            }
        }

        for (int j = 0; j < layers[layerCount].biases.size(); ++j) {
            gens >> layers[layerCount].biases[j];
        }

        layerCount++;

    }

    layers.pop_back();

    return true;
}

Neurons Perceptron::getLayer(const size_t& arg) {
    if(arg >= layers.size()) throw std::runtime_error("Layer error!");
    return layers[arg].neurons;
}

size_t Perceptron::size() {
    return layers.size();
}

std::vector<Layer> Perceptron::getSource() {
    return layers;
}

bool Perceptron::setSource(const std::vector<Layer> &layers) {
    this->layers = layers;
    return false;
}

Neurons Perceptron::operator[](const size_t& arg) {
    if(arg >= layers.size()) throw std::runtime_error("Layer error!");
    return layers[arg].neurons;
}

void Perceptron::setLearningRate(const double &arg) {
    learningRate = arg;
}

Perceptron::Perceptron(const Perceptron &copy) = default;

