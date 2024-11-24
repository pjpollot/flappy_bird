#include "application.hpp"
#include "properties.hpp"
#include "display.hpp"


using namespace std;


int main() {
    Application app("Flappy bird", WIDTH, HEIGHT, 2);

    app.start();

    cout << "Final score: " << app.getScore() << endl;

    return 0;
}


float Property::forwardSpeed = 300;

float Bird::jumpSpeed = -150;

float Bird::fallingAcceleration = 200;

float Bird::cooldownLimit = 0.3; // cooldown set to x seconds

float Pipe::minHeightRatio = 0.2;