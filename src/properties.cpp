#include "properties.hpp"

#include <cmath>


Bird::Bird(const std::string &imagePath, const int &width, const int &height, const float &x, const float &y) {
    mImagePath = imagePath;
    mWidth = abs(width);
    mHeight = abs(height);
    mX = x;
    mY = y;
    mYSpeed = 0.;
    mCooldown = cooldownLimit;
}


Bird::~Bird() {
    freeFromFlyingConstraints();
}


void Bird::jump() {
    if (mCooldown >= cooldownLimit) {
        mYSpeed = jumpSpeed;
        mCooldown = 0.;
    }
}


double Bird::getAngle() const {
    return 180 * atan(mYSpeed / Property::forwardSpeed) / M_PI;
}


void Bird::setFlyingRange(const int &low, const int &high) {
    if (!mFlyingRange) {
        mFlyingRange = new int[2];
        if (!mFlyingRange)
            throw std::runtime_error("Cannot allocate two integers for setting the flying range of the bird.");
    }

    mFlyingRange[0] = low;
    mFlyingRange[1] = high;
}


void Bird::freeFromFlyingConstraints() {
    if (mFlyingRange) {
        delete[] mFlyingRange;
        mFlyingRange = nullptr;
    }
}


void Bird::update(const float &dt) {
    mY += mYSpeed * dt;

    if (mFlyingRange &&  mY + mHeight/2 > mFlyingRange[1]) {
        mY = mFlyingRange[1] - mHeight/2;
        mYSpeed = 0.;
    } else if (mFlyingRange && mY - mHeight/2 < mFlyingRange[0]) {
        mY = mHeight/2;
        mYSpeed = 0.;
    } else 
        mYSpeed += fallingAcceleration * dt;

    mCooldown += dt;
}


int Property::getX() const {
    return (int) mX;
}


int Property::getY() const {
    return (int) mY;
}