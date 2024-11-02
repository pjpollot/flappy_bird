#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "properties.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <iostream>


class Application {
    private:
        SDL_Window* mWindow;
        SDL_Renderer* mRenderer;

        Bird* mBird;
        bool mBirdCreated;
        SDL_Texture* mBirdTexture;

        void renderBird();

        void render();

        static Mix_Chunk* jumpingSound;

        static bool initialized;

        static SDL_Texture* loadTextureFromBird(SDL_Renderer* renderer, const Bird &bird);

        static void throwSDLError(const std::string &message);

        static void throwIMGError(const std::string &message);

        static void throwMixError(const std::string &message);

        static void playSound(Mix_Chunk* chunk);

    public:
        Application(const std::string &appName, int width, int height, Bird* bird = nullptr);

        ~Application();

        void start();
};


#endif