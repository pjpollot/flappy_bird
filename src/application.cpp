#include "application.hpp"

#include "SDL2/SDL_image.h"
#include "unistd.h"

#include <stdexcept>
#include <cmath>
#include <limits>


bool Application::initialized = false;
std::map<std::string, Mix_Chunk*> Application::sounds;

static std::map<std::string, std::string> soundLinks = {
    {"wing", "./sounds/sfx_wing.wav"}, 
    {"hit", "./sounds/sfx_hit.wav"},
    {"point", "./sounds/sfx_point.wav"},
};


Application::Application(const std::string &appName, int width, int height, const unsigned &nPipes) {
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

    // Create bird
    unsigned hitboxSize = width / 40;
    if (width < height)
        hitboxSize = height / 40;

    mBird = new Bird("./images/bird.png", 1.6 * hitboxSize, hitboxSize, width/2, height/2);

    mBird->setFlyingRange(0, height);

    mBirdTexture = loadTextureFromBird(mRenderer, *mBird);
    if (!mBirdTexture)
        throwSDLError("Cannot load texture from bird.");

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        throwMixError("Cannot load Mix.");
    
    for (auto &[key, path]: soundLinks) {
        sounds[key] = Mix_LoadWAV(path.c_str());
        if (!sounds[key]) 
            throwMixError("Cannot load sfx from path: " + path);
    }

    // create pipes
    float thickness = width / (4 * nPipes), 
    maxHeight = 0.8 * height;

    mInterDistance = (width - nPipes * thickness) / (nPipes + 1);
    float x = width + mInterDistance;

    for (unsigned i = 0; i < nPipes; i++) {
        Pipe pipe(x, thickness, maxHeight);
        mPipes.push_back(pipe);
        x += thickness + mInterDistance;
    }

    mWidth = width;
    mHeight = height;
    mScore = 0;

    initialized = true;
}


Application::~Application() {
    delete mBird;
    
    SDL_DestroyTexture(mBirdTexture);

    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);

    SDL_Quit();
    initialized = false;
}


void Application::renderBird() {
    SDL_Rect box = {mBird->getX() - mBird->getWidth() / 2, mBird->getY() - mBird->getHeight() / 2, mBird->getWidth(), mBird->getHeight()};
    SDL_RenderCopyEx(mRenderer, mBirdTexture, nullptr, &box, mBird->getAngle(), nullptr, SDL_FLIP_NONE);
}


void Application::renderPipes() {
    SDL_Rect box;
    SDL_SetRenderDrawColor(mRenderer, 146, 20, 12, 255);

    for (auto pipe: mPipes) {
        box.x = pipe.getX() - pipe.getWidth()/2;
        box.y = pipe.getY() - pipe.getHeight()/2;
        box.w = pipe.getWidth();
        box.h = pipe.getHeight();

        SDL_RenderFillRect(mRenderer, &box);
    }
}


void Application::start() {
    SDL_Event event;
    bool run = true, jump = false;

    Uint32 t0 = SDL_GetTicks(), t1;
    float dt;

    bool underPipe = false, 
    currentlyUnderPipe = false, 
    collision = false;

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
                            playSound("wing");
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
        updatePipes(dt);

        birdStatusCheck(currentlyUnderPipe, collision);
        if (collision) {
            playSound("hit");
            sleep(1);
            run = false;
        } else if (underPipe && !currentlyUnderPipe) {
            playSound("point");
            mScore++;
        }
            
        underPipe = currentlyUnderPipe;

        render();

        t0 = t1;
        jump = false;
    }
}


void Application::render() {
    SDL_SetRenderDrawColor(mRenderer, 75, 127, 82, 255);
    SDL_RenderClear(mRenderer);

    renderBird();
    renderPipes();

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


void Application::playSound(const std::string &soundName) {
    auto entry = sounds.find(soundName);
    if (entry == sounds.end())
        throw std::runtime_error("Sound effect '" + soundName + "' does not exist.");

    Mix_PlayChannel(-1, entry->second, 0);
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


void Application::updatePipes(const float &dt) {
    std::vector<unsigned> outOfScreenPipeIndexes;
    
    for (unsigned i = 0; i < mPipes.size(); i++) {
        mPipes[i].update(dt);

        if (mPipes[i].outOfScreen())
            outOfScreenPipeIndexes.push_back(i);
    }

    float x = mWidth;
    for (auto index: outOfScreenPipeIndexes) {
        mPipes[index].sample(x + mPipes[index].getWidth()/2);
        x += mPipes[index].getWidth()/2 + mInterDistance;
    }

}


void Application::birdStatusCheck(bool &sameX, bool &collision) {
    sameX = false;
    collision = false;

    bool localSameX, localCollision;
    unsigned i = 0;
    while (i < mPipes.size() && !collision) {
        mPipes[i].positionCheck(*mBird, localSameX, localCollision);
        collision |= localCollision;
        sameX |= localSameX;
        i++;
    }
}