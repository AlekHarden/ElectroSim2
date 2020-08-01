#include <ElectroSim/Proton.hpp>

Proton::Proton(float x, float y, float radius) : Particle(x, y, radius) {
    // Protons have a positive charge
    mCharge = CHARGE;
}

void Proton::render() {
    return;
}
