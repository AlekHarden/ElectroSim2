#include <ElectroSim/Electron.hpp>

Electron::Electron(float x, float y, float radius) : Particle(x, y, radius) {
	// Electrons have a negative charge
	mCharge = -CHARGE;
}

void Electron::render() {
	return;
}
