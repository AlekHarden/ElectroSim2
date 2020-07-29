#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <iostream>
#include <cmath>
#include "Constants.hpp"

class Particle {
protected:
    float mCharge, mVelX, mVelY, mAccX, mAccY, mMass, mRadius, mX, mY;

public:
    unsigned int mIndices[3 * (RESOLUTION - 2)];
    float mPoints[2 * RESOLUTION];

    Particle(float x, float y, float radius);
    Particle(const Particle& p);
    ~Particle() {
    };
    void applyForces(Particle& p);
    void collide(Particle& p);
    void calcPoints();
    void calcIndices();
    virtual void render() = 0;
    void tick();
    void test();
};

#endif // ifndef PARTICLE_HPP
