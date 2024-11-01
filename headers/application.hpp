#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "properties.hpp"

#include "SDL2/SDL.h"
#include <iostream>


class Application {
    private:
        SDL_Window* mWindow;
        SDL_Renderer* mRenderer;

        Bird* mBird;
        bool mBirdCreated;
        SDL_Texture* mBirdTexture;

        //unsigned mWidth, mHeight;

        static bool initialized;

        static SDL_Texture* loadTextureFromBird(SDL_Renderer* renderer, const Bird &bird);

        static void throwSDLError(const std::string &message);

        static void throwIMGError(const std::string &message);

        void renderBird();

        void render();

    public:
        Application(const std::string &appName, int width, int height, Bird* bird = nullptr);

        ~Application();

        void start();
};


#endif