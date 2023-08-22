#pragma once

class Layer {

    public:
        virtual ~Layer() {};

        virtual void reset() = 0;
        virtual void update() = 0;
        virtual void render() = 0;

    protected:
        int width, height;

};

