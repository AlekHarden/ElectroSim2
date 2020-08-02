#include <ElectroSim/Handler.hpp>

Handler::Handler(){
	int index = 0;
	float step = 2*PI / CIRCLERESOLUTION;

	for(float i = 0; i < 2*PI; i += step) {
		mUnitCircle[index] = cos(i);
		mUnitCircle[index + 1] = sin(i);
		index += 2;
	}
	for(int i = 0; i < CIRCLERESOLUTION - 2; i++) {
		mIndices[i * 3] = 0;
		mIndices[i * 3 + 1] = i + 1;
		mIndices[i * 3 + 2] = i + 2;
	}
}

float* Handler::getPoints(){
	float* points;
    points = (float*)malloc(sizeof(float) * 2 * CIRCLERESOLUTION * mParticles.size());
    int counter = 0;

    for(auto & p : mParticles){
        for(int i = 0; i < 2* CIRCLERESOLUTION; i+=2){
            points[i + CIRCLERESOLUTION * counter * 2 ] = mUnitCircle[i] * p.mRadius + p.mX ;
			points[i + CIRCLERESOLUTION * counter * 2 + 1] = mUnitCircle[i + 1] * p.mRadius + p.mY;
        }
        counter++;
    }

    return points;
}

unsigned int* Handler::getIndices(){
	unsigned int* indices;
    indices = (unsigned int*)malloc(sizeof(unsigned int) * 3 * (CIRCLERESOLUTION - 2) * mParticles.size());
	//which particle
    unsigned int counter = 0;

    for(auto & p : mParticles){
        for(int i = 0; i < 3 * (CIRCLERESOLUTION - 2); i++){
            indices[i + (3* (CIRCLERESOLUTION - 2) * counter)] = counter * (CIRCLERESOLUTION) + mIndices[i];
        }
        counter++;
    }

    return indices;
}

void Handler::addParticle(Particle p){
    mParticles.push_back(p);
}

void Handler::removeParticle(){
    std::remove_if(mParticles.begin(), mParticles.end(), [&](Particle p){return p.mHeld;});
}

unsigned int Handler::getNumInd(){
    return mParticles.size() * (CIRCLERESOLUTION - 2) * 3;
}

unsigned int Handler::getNumPoints(){
    return mParticles.size() * (CIRCLERESOLUTION);
}

void Handler::tick(){
	for( int j = 0; j < mParticles.size(); j++){



		for( int i = j+1; i < mParticles.size(); i++){
			mParticles[i].applyForces(mParticles[j]);
			mParticles[i].collide(mParticles[j]);
		}


	}
	for( int j = 0; j < mParticles.size(); j++){
		mParticles[j].tick();
	}


}
