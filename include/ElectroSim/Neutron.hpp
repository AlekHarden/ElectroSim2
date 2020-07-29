#ifndef NEUTRON_HPP
#define NEUTRON_HPP

#include "Particle.hpp"

class Neutron : public Particle {
public:
    Neutron(float x, float y, float radius);
    void render();
};

#endif // ifndef NEUTRON_HPP
