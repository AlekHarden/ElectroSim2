#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <vector>
#include <algorithm>
#include <ElectroSim/Proton.hpp>
#include <ElectroSim/Neutron.hpp>
#include <ElectroSim/Electron.hpp>

class Handler{
private:
    std::vector<Particle> mParticles;

public:
    Handler();
    float* getPoints(float* points);
    unsigned int* getIndices(unsigned int* indices);
    void addParticle(Particle p);
    void removeParticle();
    unsigned int getNumInd();
    unsigned int getNumPoints();
};


#endif
