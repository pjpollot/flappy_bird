#include "application.hpp"

#include "SDL2/SDL_image.h"
#include <stdexcept>
#include <cmath>


bool Application::initialized = false;

Mix_Chunk* Application::jumpingSound = nullptr;


Application::Application(const std::string &appName, int width, int height, Bird* bird) {
    width = abs(width);
    height = abs(height);

    if (initialized)
        throw std::runtime_error("SDL already initialized. Cannot initialize twice.");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
        throwSDLError("Cannot load SDL2.");

    mWindow = SDL_CreateWindow(appName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (!mWindow)
        throwSDLError("Cannot create window.");

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!mRenderer)
        throwSDLError("Cannot create renderer.");

    
    int imgFlag = IMG_INIT_PNG;
    if (!(imgFlag && IMG_Init(imgFlag)))
        throwIMGError("Cannot initialize IMG.");

    if (bird) {
        mBird = bird;
        mBirdCreated = false;
    } else {
        unsigned hitboxSize = width / 40;
        if (width < height)
            hitboxSize = height / 40;

        mBird = new Bird("./images/bird.png", 1.6 * hitboxSize, hitboxSize, width/2, height/2);
        if (!mBird)
            throw std::runtime_error("Cannot allocate memory to instanciate bird.");

        mBird->setFlyingRange(0, height);
        mBirdCreated = true;
    }

    mBirdTexture = loadTextureFromBird(mRenderer, *mBird);
    if (!mBirdTexture)
        throwSDLError("Cannot load texture from bird.");

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        throwMixError("Cannot load Mix.");

    jumpingSound = Mix_LoadWAV("./sounds/sfx_wing.wav");
    if (!jumpingSound)
        throwMixError("Cannot load jumping sfx.");

    initialized = true;
}


Application::~Application() {
    if (mBirdCreated)
        delete mBird; // else we assume the user can access the bird object somewhere else
    SDL_DestroyTexture(mBirdTexture);

    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);

    SDL_Quit();
    initialized = false;
}


void Application::renderBird() {
    SDL_Rect box = {mBird->getX() - mBird->getWidth() / 2, mBird->getY() - mBird->getHeight() / 2, mBird->getWidth(), mBird->getHeight()};
    //SDL_RenderCopy(mRenderer, mBirdTexture, nullptr, &box);
    SDL_RenderCopyEx(mRenderer, mBirdTexture, nullptr, &box, mBird->getAngle(), nullptr, SDL_FLIP_NONE);
}


void Application::start() {
    SDL_Event event;
    bool run = true, jump = false;

    Uint32 t0 = SDL_GetTicks(), t1;
    float dt;

    while (run) {
        t1 = SDL_GetTicks();
        dt = (t1 - t0) * 1e-3;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                run = false;
                
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        if (!mBird->inCooldown())
                            playSound(jumpingSound);
                        mBird->jump();
                        break;

                    case SDLK_ESCAPE:
                        run = false;
                        break;

                    default:
                        break;
                }
            }
        }
        
        mBird->update(dt);
        render();

        t0 = t1;
        jump = false;
    }
}


void Application::render() {
    SDL_SetRenderDrawColor(mRenderer, 75, 127, 82, 255);
    SDL_RenderClear(mRenderer);

    renderBird();

    SDL_RenderPresent(mRenderer);
}


void Application::throwSDLError(const std::string &message) {
    const std::string errorMsg = message + "\nSDL error: " + SDL_GetError();
    throw std::runtime_error(errorMsg);
}


void Application::throwIMGError(const std::string &message) {
    const std::string errorMsg = message + "\nIMG error: " + IMG_GetError();
    throw std::runtime_error(errorMsg);
}

void Application::throwMixError(const std::string &message) {
    const std::string errorMsg = message + "\nMix error: " + Mix_GetError();
    throw std::runtime_error(errorMsg);
}


void Application::playSound(Mix_Chunk* chunk) {
    Mix_PlayChannel(-1, chunk, 0);
}


SDL_Texture* Application::loadTextureFromBird(SDL_Renderer* renderer, const Bird &bird) {
    SDL_Surface* loadedSurface = IMG_Load(bird.getImagePath().c_str());
    if (!loadedSurface) {
        std::string message = "Cannot load surface from path " + bird.getImagePath();
        throwIMGError(message);
    }

    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGBA(loadedSurface->format, 0, 0, 0, 0));

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    return texture;
}