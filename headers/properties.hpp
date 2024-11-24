#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

#include <iostream>


class Property {
    protected:
        int mWidth, mHeight;
        float mX, mY;

        static float forwardSpeed;
    
    public:
        float getExactX() const {return mX;}
        float getExactY() const {return mY;}
        int getWidth() const {return mWidth;}
        int getHeight() const {return mHeight;}
        int getX() const {return (int) mX;}
        int getY() const {return (int) mY;}

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

        float getAngle() const;

        bool inCooldown() const;
};


class Pipe: public Property {
    public:
        float mMaxHeight, mThickness;
        
        static float minHeightRatio;

    public: 
        Pipe(const float &x, const float &thickness, const float &maxHeight);

        void update(const float &dt);

        void sample(const float &x);

        void positionCheck(const Property &property, bool &sameX, bool &collision) const;

        bool outOfScreen() const;


        float getExactThickness() const {return mThickness;}
        int getThickness() const {return (int) mThickness;}
};


#endif