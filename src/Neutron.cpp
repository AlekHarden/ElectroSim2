#include <ElectroSim/Neutron.hpp>

Neutron::Neutron(int x, int y, int radius) : Particle(x, y, radius) {
    mCharge = CHARGE;
}

void Neutron::render() {
    return;
}
