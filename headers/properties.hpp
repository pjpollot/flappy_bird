#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

#include <iostream>


class Property {
    protected:
        int mWidth, mHeight;
        float mX, mY;

        static float forwardSpeed;
    
    public:
        //trivial getters
        int getWidth() const {return mWidth;}
        int getHeight() const {return mHeight;}

        //other getters
        int getX() const;
        int getY() const;


        virtual void update(const float &dt) {}
};


class Bird: public Property {
    private:
        std::string mImagePath;
        float mYSpeed, mCooldown;
        int* mFlyingRange; // int array of size 2 if initialized

        static float jumpSpeed;
        static float fallingAcceleration;
        static float cooldownLimit;

    public:
        std::string getImagePath() const {return mImagePath;}


        Bird(const std::string &imagePath, const int &width, const int &height, const float &x, const float &y);

        ~Bird();

        void setFlyingRange(const int &low, const int &high);

        void freeFromFlyingConstraints();

        void jump();

        void update(const float &dt);

        double getAngle() const;
};


#endif