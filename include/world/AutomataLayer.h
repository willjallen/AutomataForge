#pragma once
#include <vector>
#include <string>
#include "Layer.h"

class AutomataLayer : public Layer{
    public: 
        /**
         * @brief Construct a new Automata Layer object. Initializes a GL texture object the layer will write to.
         * 
         * @param width Width of the layer
         * @param height Height of the layer
         * @param numStates Number of states each cell can be
         * @param rewriteRule TODO: The update rule for the automata
         */
        AutomataLayer(unsigned int width, unsigned int height, unsigned int numStates, std::string rewriteRule);
        ~AutomataLayer();
        void reset();
        void update();
        void render();
    private:
        int numStates;
        std::string rewriteRule;

};