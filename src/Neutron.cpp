#include <ElectroSim/Neutron.hpp>

Neutron::Neutron(float x, float y, float radius) : Particle(x, y, radius) {
	// Neutrons don't have a charge
	mCharge = 0;
}

void Neutron::render() {
	return;
}
