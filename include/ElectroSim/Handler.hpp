#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <vector>
#include <algorithm>
#include <ElectroSim/Particle.hpp>

class Handler {
private:
float mUnitCircle[CIRCLERESOLUTION *2];
unsigned int mIndices[3*(CIRCLERESOLUTION-2)];


public:
std::vector<Particle> mParticles;
bool mHolding = false;
double mDelX = 0;
double mDelY = 0;
Handler();
float* getPoints();
unsigned int* getIndices();
void addParticle(Particle p);
void removeParticle();
unsigned int getNumInd();
unsigned int getNumPoints();
void addVelall();
bool grabParticles(double xPos, double yPos);
void releaseParticles();
void deleteHeld();
void setDelta(double delX, double delY);
void tick();
};

#endif
