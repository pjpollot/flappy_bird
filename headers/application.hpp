#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "properties.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

#include <iostream>
#include <vector>
#include <map>


class Application {
    private:
        int mWidth, mHeight;
        unsigned mScore;

        SDL_Window* mWindow;
        SDL_Renderer* mRenderer;

        Bird* mBird;
        SDL_Texture* mBirdTexture;

        std::vector<Pipe> mPipes;
        float mInterDistance;


        void renderBird();

        void renderPipes();

        void render();

        void updatePipes(const float &dt);


        static std::map<std::string, Mix_Chunk*> sounds;

        static bool initialized;

        static SDL_Texture* loadTextureFromBird(SDL_Renderer* renderer, const Bird &bird);

        static void throwSDLError(const std::string &message);

        static void throwIMGError(const std::string &message);

        static void throwMixError(const std::string &message);

        static void playSound(const std::string &soundName);

        void birdStatusCheck(bool &sameX, bool &collision);

    public:
        Application(const std::string &appName, int width, int height, const unsigned &nPipes = 3);

        ~Application();

        void start();


        unsigned getScore() const {return mScore;}
};


#endif