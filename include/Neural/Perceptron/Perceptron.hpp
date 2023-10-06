#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <cmath>
#include <fstream>

#include "Layer.hpp"

namespace Neural {

    class Perceptron {
    protected:

        double (*activate)(double);

        double learningRate;

        double derivative(const double &x);

        double addition(int first, int second);

        std::vector<Layer> layers;


    public:

        virtual ~Perceptron();

        explicit Perceptron(const std::vector<unsigned> &layer, double (*function)(double));

        Perceptron(const Perceptron &copy);

        bool saveToFile(const std::string &path);

        bool loadFromFile(const std::string &path);

        void setLearningRate(const double &arg);

        void setInput(const Neurons& input);

        Neurons getLayer(const size_t &arg);

        Neurons operator[](const size_t &arg);

        [[nodiscard]] const std::vector<Layer>& getSource() const noexcept;

        void setSource(const std::vector<Layer> &layers);

        size_t size() const noexcept;

        Neurons forwardFeed();

        Neurons forwardFeed(const Neurons& input);

        void backPropagation(const Neurons &target);

    };

};

#endif //NETWORK_H
