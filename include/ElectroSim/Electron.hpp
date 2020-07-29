#ifndef ELECTRON_HPP
#define ELECTRON_HPP

#include "Particle.hpp"

class Electron : public Particle {
public:
Electron(int x, int y, int radius);
void render();
};

#endif // ifndef ELECTRON_HPP
