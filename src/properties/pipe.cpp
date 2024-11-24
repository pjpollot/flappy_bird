#include "properties.hpp"

#include <cmath>
#include <random>


Pipe::Pipe(const float &x, const int &width, const float &maxHeight) {
    mWidth = abs(width);
    mMaxHeight = abs(maxHeight);
    sample(x);
}


void Pipe::sample(const float &x) {
    mX = x;

    float u = (float) std::rand() / RAND_MAX;
    if (u < minHeightRatio)
        u = minHeightRatio;
    
    mHeight = u * mMaxHeight;
    mY = mHeight/2;
}


void Pipe::positionCheck(const Property &property, bool &sameX, bool &collision) const {
    sameX = false;
    collision = false;

    float left = mX - mWidth/2,
    right = mX + mWidth/2;
    if (property.getExactX() + property.getWidth()/2 >= left && property.getExactX() - property.getWidth()/2 <= right) {
        sameX = true;


        float up = mY - mHeight/2,
        down = mY + mHeight/2;
        if (property.getExactY() + property.getHeight()/2 >= up && property.getExactY() - property.getHeight()/2 <= down)
            collision = true;
    }
}


void Pipe::update(const float &dt) {
    mX += -dt * forwardSpeed;
}


bool Pipe::outOfScreen() const {
    return mX + mWidth/2 < 0;
}