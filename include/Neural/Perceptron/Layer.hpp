#ifndef LAYER_H
#define LAYER_H

#include <vector>

namespace Neural {

    typedef std::vector<double> Neurons;

    struct Layer {

        int size;

        Neurons neurons;

        std::vector<Neurons> weights;

        Neurons biases;

        Layer(int size, int nextSize);

        Layer(const Layer &copy);

    };

};

#endif //LAYER_H
