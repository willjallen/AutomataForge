#pragma once
#include <vector>
#include "Layer.h"
// #include "cuda_runtime.h"

// __global__ void update();

class DiffusionLayer : public Layer<float>{
    public: 
        DiffusionLayer(float diffusion_rate) 
            : diffusion_rate(diffusion_rate) {};
    private:
        float diffusion_rate;

};