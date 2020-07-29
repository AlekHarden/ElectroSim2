#ifndef ELECTRON_HPP
#define ELECTRON_HPP

#include "Particle.hpp"

class Electron : public Particle {
public:
    Electron(float x, float y, float radius);
    void render();
};

#endif // ifndef ELECTRON_HPP
