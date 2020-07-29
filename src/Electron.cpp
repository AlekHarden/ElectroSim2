#include <ElectroSim/Electron.hpp>

Electron::Electron(int x, int y, int radius) : Particle(x, y, radius) {
    mCharge = -CHARGE;
}

void Electron::render() {
    return;
}
