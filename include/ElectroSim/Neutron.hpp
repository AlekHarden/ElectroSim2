#ifndef NEUTRON_HPP
#define NEUTRON_HPP

#include "Particle.hpp"

class Neutron : public Particle {
public:
Neutron(int x, int y, int radius);
void render();
};

#endif // ifndef NEUTRON_HPP