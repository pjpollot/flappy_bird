#include "properties.hpp"

#include <cmath>
#include <random>


Pipe::Pipe(const float &x, const float &thickness, const float &maxHeight) {
    mThickness = abs(thickness);
    mMaxHeight = abs(maxHeight);
    sample(x);
}


void Pipe::sample(const float &x) {
    mX = x;

    float u = (float) std::rand() / RAND_MAX;
    if (u < minHeightRatio)
        u = minHeightRatio;
    mY = u * mMaxHeight;
}


void Pipe::positionCheck(const Property &property, bool &sameX, bool &collision) const {
    sameX = false;
    collision = false;

    if (property.getExactX() >= mX && property.getExactX() <= mX + mThickness) {
        sameX = true;

        if (property.getExactY() <= mY)
            collision = true;
    }
}


void Pipe::update(const float &dt) {
    mX += -dt * forwardSpeed;
}


bool Pipe::outOfScreen() const {
    return mX + mThickness < 0;
}