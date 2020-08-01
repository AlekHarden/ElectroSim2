#ifndef PROTON_HPP
#define PROTON_HPP

#include <ElectroSim/Particle.hpp>

class Proton : public Particle {
public:
    Proton(float x, float y, float radius);
    void render();
};

#endif // ifndef PROTON_HPP
