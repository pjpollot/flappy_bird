#include "application.hpp"
#include "properties.hpp"
#include "display.hpp"


using namespace std;


int main() {
    Application app("Flappy bird", WIDTH, HEIGHT);

    app.start();

    return 0;
}


float Property::forwardSpeed = 300;

float Bird::jumpSpeed = -150;

float Bird::fallingAcceleration = 200;

float Bird::cooldownLimit = 0.5; // cooldown set to x seconds