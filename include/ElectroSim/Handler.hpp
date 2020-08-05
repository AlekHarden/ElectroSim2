#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <vector>
#include <algorithm>
#include <ElectroSim/Particle.hpp>


class Handler{
private:
    std::vector<Particle> mParticles;
	float mUnitCircle[CIRCLERESOLUTION *2];
	unsigned int mIndices[3*(CIRCLERESOLUTION-2)];


public:
    Handler();
    float* getPoints();
    unsigned int* getIndices();
    void addParticle(Particle p);
    void removeParticle();
    unsigned int getNumInd();
    unsigned int getNumPoints();
	void addVelall();


	void tick();
};


#endif
