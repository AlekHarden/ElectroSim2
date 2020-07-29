#ifndef PROTON_HPP
#define PROTON_HPP

#include "Particle.hpp"

class Proton : public Particle {
public:
Proton(int x, int y, int radius);
void render();
};

#endif // ifndef PROTON_HPP
