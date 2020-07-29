#include <ElectroSim/Proton.hpp>

Proton::Proton(int x, int y, float radius) : Particle(x, y, radius) {
    // Protons have a positive charge
    mCharge = CHARGE;
}

void Proton::render() {
    return;
}
