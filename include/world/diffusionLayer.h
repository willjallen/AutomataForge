#pragma once
#include <vector>
#include "Layer.h"

class DiffusionLayer : public Layer{
    public: 
        /**
         * @brief Construct a new Diffusion Layer object.
         * TODO: Different diffusion methods
         * 
         * @param diffusionRate 
         */
        DiffusionLayer(float diffusionRate);
        ~DiffusionLayer();

        void reset();
        void update();
        void render();
    private:
        float diffusionRate;

};