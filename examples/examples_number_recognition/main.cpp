#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <cmath>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <Neural/Perceptron.hpp>
#include <Neural/Activation/Functions.h>

using namespace sf;
using namespace std;

#define SCALE 20

#define POS 10

double sigmoid(double arg){
    return 1.0 / (1.0 + exp(-arg));
}

std::vector<double> ImgToNeurons(const Image& img) {
    std::vector<double> pixels;

    for (int i = 0; i < img.getSize().y; i++) {
        for (int j = 0; j < img.getSize().x; j++) {
            pixels.push_back((double) (img.getPixel(j, i).r + img.getPixel(j, i).g + img.getPixel(j, i).b) / (255 * 3));
        }
    }

    return pixels;
}

Image NeuronsToImage(const std::vector<double>& neurons){
    std::vector<double> buffer = neurons;

    int size = sqrt(neurons.size());

    Image img;

    img.create(size, size, Color::Black);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            img.setPixel(j, i, Color(255 * buffer.front(), 255 * buffer.front(), 255 * buffer.front()));
            swap(buffer.front(), buffer.back());
            buffer.pop_back();
        }
    }

    return img;

}
// Graphics
int painter(Neural::Perceptron* network) {
    Image img;
    Texture texture;
    Sprite spriteCanvas;

    RenderTexture canvas;
    canvas.create(28, 28, true);
    canvas.clear(Color::Black);

    CircleShape circle(1);
    circle.setFillColor(Color(255, 255, 255, 40));

    img.create(28, 28, Color::Black);
    texture.loadFromImage(img);
    spriteCanvas.setTexture(texture);
    spriteCanvas.scale(SCALE, SCALE);
    spriteCanvas.setPosition(POS, POS);

    int x = 1000, y = 20;

    RectangleShape shape;
    shape.setSize(Vector2f(128, 32));
    shape.setFillColor(Color::Green);
    Font font;
    font.loadFromFile("../build/font.ttf");
    Text text("0: ", font, 32);
    text.setFillColor(Color(0, 0, 0, 100));

    RenderWindow window(VideoMode(1200, 600), "Neuro");

    Neural::Neurons output = network->forwardFeed();

    Event event;

    Mouse mouse;

    window.clear(Color(255, 255, 255, 180));
    window.draw(spriteCanvas);
    window.display();

    window.pollEvent(event);

    bool mouseTrigger = false;

    while (window.isOpen()) {

        window.pollEvent(event);

        if (!event.type)
            window.close();


        if (spriteCanvas.getGlobalBounds().contains(mouse.getPosition(window).x, mouse.getPosition(window).y)
            && mouse.isButtonPressed(Mouse::Button::Left)) {

            mouseTrigger = true;

            circle.setPosition((mouse.getPosition(window).x - POS) / SCALE,
                               (mouse.getPosition(window).y - POS) / SCALE);
            circle.setFillColor(Color(255, 255, 255, 64));

            canvas.draw(circle);
            canvas.display();

            texture = canvas.getTexture();
            spriteCanvas.setTexture(texture);
            img = texture.copyToImage();

            output = network->forwardFeed(ImgToNeurons(img));


            window.clear(Color(255, 255, 255, 180));
            window.draw(spriteCanvas);

            int bias = 600;

            for (int i = 1; i < network->size() - 1; ++i) {
                Texture l;
                l.loadFromImage(NeuronsToImage(network->getLayer(i)));
                Sprite ll;
                ll.setTexture(l);
                ll.setScale(10, 10);
                ll.setPosition(i * bias + 10, 10);
                bias -= 200;
                window.draw(ll);
            }

            for (int i = 0; i < output.size(); ++i) {
                text.setString(std::to_string(i) + ":");
                text.setPosition(x, y + (i * 48));
                shape.setPosition(x + 32, y + (i * 48) + 5);
                shape.setScale(output[i], 1.0);
                shape.setFillColor(Color(255 * (1 - output[i]), 255 * output[i], 0));
                window.draw(text);
                window.draw(shape);
            }

            window.display();

        } else if (!mouse.isButtonPressed(Mouse::Button::Left) && mouseTrigger) {
            mouseTrigger = false;
        } else if (spriteCanvas.getGlobalBounds().contains(mouse.getPosition(window).x, mouse.getPosition(window).y)
                   && mouse.isButtonPressed(Mouse::Button::Right)) {

            circle.setPosition((mouse.getPosition(window).x - POS) / SCALE,
                               (mouse.getPosition(window).y - POS) / SCALE);
            circle.setFillColor(Color::Black);

            canvas.draw(circle);
            canvas.display();

            texture = canvas.getTexture();
            spriteCanvas.setTexture(texture);
            img = texture.copyToImage();

            output = network->forwardFeed(ImgToNeurons(img));

            window.clear(Color(255, 255, 255, 40));
            window.draw(spriteCanvas);

            int bias = 600;

            for (int i = 1; i < network->size() - 1; ++i) {
                Texture l;
                l.loadFromImage(NeuronsToImage(network->getLayer(i)));
                Sprite ll;
                ll.setTexture(l);
                ll.setScale(10, 10);
                ll.setPosition(i * bias + 10, 10);
                bias -= 200;
                window.draw(ll);
            }

            for (int i = 0; i < output.size(); ++i) {
                text.setString(std::to_string(i) + ":");
                text.setPosition(x, y + (i * 48));
                shape.setPosition(x + 32, y + (i * 48) + 5);
                shape.setScale(output[i], 1.0);
                shape.setFillColor(Color(255 * (1 - output[i]), 255 * output[i], 0));
                window.draw(text);
                window.draw(shape);
            }
            window.display();
        } else if (!spriteCanvas.getGlobalBounds().contains(mouse.getPosition(window).x, mouse.getPosition(window).y)
                   && mouse.isButtonPressed(Mouse::Button::Left)) {

            canvas.clear(Color::Black);

            texture = canvas.getTexture();
            spriteCanvas.setTexture(texture);
            img = texture.copyToImage();

            output = network->forwardFeed(ImgToNeurons(img));

            window.clear(Color(255, 255, 255, 180));
            window.draw(spriteCanvas);

            int bias = 600;

            for (int i = 1; i < network->size() - 1; ++i) {
                Texture l;
                l.loadFromImage(NeuronsToImage(network->getLayer(i)));
                Sprite ll;
                ll.setTexture(l);
                ll.setScale(10, 10);
                ll.setPosition(i * bias + 10, 10);
                bias -= 200;
                window.draw(ll);
            }

            for (int i = 0; i < output.size(); ++i) {
                text.setString(std::to_string(i) + ":");
                text.setPosition(x, y + (i * 48));
                shape.setPosition(x + 32, y + (i * 48) + 5);
                shape.setScale(output[i], 1.0);
                shape.setFillColor(Color(255 * (1 - output[i]), 255 * output[i], 0));
                window.draw(text);
                window.draw(shape);
            }
            window.display();
        }

    }
}

int max(vector<double> neurons) {
    int iter = 0;
    for (int i = 0; i < neurons.size(); ++i) {
        if (neurons[i] > neurons[iter])
            iter = i;
    }
    return iter;
}

namespace fs = std::filesystem;

double test(Neural::Perceptron network) {

    vector<string> files;

    std::string path = "./build/test/";
    for (const auto &entry: fs::directory_iterator(path)) {
        stringstream ss;
        ss << entry.path();
        files.push_back(ss.str().substr(1,ss.str().length() - 2));
        cout << files.back() << endl;
    }

    Image img;

    img.create(28, 28, Color::Black);

    img.loadFromFile(files[0]);

    double result = 0;

    for (int i = 0; i < files.size(); ++i) {
        img.loadFromFile(files[i]);
        Neural::Neurons l = network.forwardFeed(ImgToNeurons(img));
        if (max(l) == (int) (files[i][10] - '0'))
            result++;
    }

    return result / 10000.0;

}

Neural::Perceptron lerner(const Neural::Perceptron& old) {

    vector<string> files;

    std::string path = "./build/train/";
    for (const auto &entry: fs::directory_iterator(path)) {
        stringstream ss;
        ss << entry.path();
        files.push_back(ss.str().substr(1,ss.str().length() - 2));
    }

    Image img;

    img.create(28, 28, Color::Black);

    img.loadFromFile(files[0]);

    Neural::Perceptron network = old;

    int EPOCH = 6000;
    int packSize = 100;

    srand(time(NULL));

    ofstream graph("graph.csv");

    for (int i = 0; i < EPOCH; ++i) {
        size_t rightA = 0;
        double errorSum = 0;
        for (int j = 0; j < packSize; j++) {
            int pack = ((int) rand() + rand()) % 60000;

            //cout << "EPOCH: " << i + 1 << " Package: " << j << " : " << pack << " -> " << files[pack][10] << endl;

            img.loadFromFile(files[pack]);
            std::vector<double> targets(10, 0);
            targets[files[pack][10] - '0'] = 1;

            Neural::Neurons l = network.forwardFeed(ImgToNeurons(img));

            network.backPropagation(targets);

            if(max(l) == (files[pack][10] - '0')) rightA++;

            for(int j = 0; j < 10; j++) {
                errorSum += (targets[j] - l[j]) * (targets[j] - l[j]);
            }

        }
        graph << i + 1 << ", " << rightA << ", " << errorSum << endl;
        cout << "Epoch: " << i + 1 << " | Right: " << rightA << " | Error: " << errorSum << endl;
    }

    cout << "Accuracy: " << test(network) * 100 << "%" << endl;

    return network;

}

int main(int argc, char *argv[]) {

    Image img;
    img.create(28, 28, Color::Black);

    // Creating Perceptron model
    Neural::Perceptron network({784, 256, 128, 10}, Neural::sigmoid);

    network.setLearningRate(0.01);

    // loading finished neural network
    if(!network.loadFromFile("../build/gen/gen.txt")) {
        cout << "Load Error!" << endl;
    }
//    if need to test finished neural network
//    cout << "Accuracy: " << test(network) * 100 << "%" << endl;

    // window
    painter(&network);

    // Learning algorithm
    //network = lerner(network);

    // Saving neural network
    /*if(!network.saveToFile("./build/gen/gen.txt")) {
        cout << "Save Error!" << endl;
    }*/

}