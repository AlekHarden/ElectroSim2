#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <iostream>
#include <cmath>
#include "Constants.hpp"

class Particle {
protected:
int mX, mY, mRadius;
float mCharge, mVelX, mVelY, mAccX, mAccY, mMass;

public:
Particle(int x, int y, int radius);
Particle(const Particle& p);
~Particle() {
};
void applyForces(Particle& p);
void collide(Particle& p);
virtual void render() = 0;
void tick();
void test();
};

#endif // ifndef PARTICLE_HPP
