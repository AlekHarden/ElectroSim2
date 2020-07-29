#ifndef PROTON_HPP
#define PROTON_HPP

#include "Particle.hpp"

class Proton : public Particle {
public:
    Proton(int x, int y, float radius);
    void render();
};

#endif // ifndef PROTON_HPP
