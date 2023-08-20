#pragma once
#include <vector>
#include "layer.h"

class World{

    public:
        World();
        void update();

    private:
        std::vector<Layer<class T>> layers; 

};