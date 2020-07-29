#include <ElectroSim/Proton.hpp>

Proton::Proton(int x, int y, int radius) : Particle(x, y, radius) {
    mCharge = CHARGE;
}

void Proton::render() {
    return;
}
