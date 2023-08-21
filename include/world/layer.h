#pragma once
#include <vector>

template<class T>
class Layer {

    public:
        virtual void reset();
        virtual void update();

    protected:
        int width, height;
        std::vector<std::vector<T>> grid;
};

