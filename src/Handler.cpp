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
	points = (float*)malloc(sizeof(float) * CIRCLERESOLUTION * mParticles.size() * 6);
	int counter = 0;

	for(auto & p : mParticles) {
		for(int i = 0; i < CIRCLERESOLUTION; i++) {
			points[6 * (i + CIRCLERESOLUTION * counter)] = mUnitCircle[2*i] * p.mRadius + p.mX;
			points[6 * (i + CIRCLERESOLUTION * counter) + 1] = mUnitCircle[2*i+1] * p.mRadius + p.mY;
			points[6 * (i + CIRCLERESOLUTION * counter) + 2] = (p.mCharge < 0 ? 0.30980 : 0.94187);
			points[6 * (i + CIRCLERESOLUTION * counter) + 3] = (p.mCharge < 0 ? 0.40392 : 0.24706);
			points[6 * (i + CIRCLERESOLUTION * counter) + 4] = (p.mCharge < 0 ? 0.94118 : 0.19608);
			points[6 * (i + CIRCLERESOLUTION * counter) + 5] = (p.mHeld ? 0.5 : 1);
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

	for(auto & p : mParticles) {
		for(int i = 0; i < 3 * (CIRCLERESOLUTION - 2); i++) {
			indices[i + (3* (CIRCLERESOLUTION - 2) * counter)] = counter * (CIRCLERESOLUTION) + mIndices[i];
		}
		counter++;
	}

	return indices;
}

void Handler::addParticle(Particle p){
	mParticles.push_back(p);
}

void Handler::addVelall(){
	for( int j = 0; j < mParticles.size(); j++) {
		mParticles[j].mVelX +=1;
	}
}

void Handler::removeParticle(){
	mParticles.erase(std::remove_if(mParticles.begin(), mParticles.end(), [&](Particle& p){
		return p.mHeld;
	}), mParticles.end());
	mHolding = 0;
}

unsigned int Handler::getNumInd(){
	return mParticles.size() * (CIRCLERESOLUTION - 2) * 3;
}

unsigned int Handler::getNumPoints(){
	return mParticles.size() * (CIRCLERESOLUTION);
}

void Handler::selectArea(glm::vec2 start, glm::vec2 end){
	float swap;
	if(end.x < start.x) {
		swap = end.x;
		end.x = start.x;
		start.x = swap;
	}

	if(end.y > start.y) {
		swap = end.y;
		end.y = start.y;
		start.y = swap;
	}

	for(int i = 0; i < mParticles.size(); i++) {
		if(mParticles[i].mX > start.x && mParticles[i].mY < start.y && mParticles[i].mX < end.x && mParticles[i].mY > end.y) {
			mParticles[i].mHeld = 1;
			mParticles[i].mGrabX = mParticles[i].mX;
			mParticles[i].mGrabY = mParticles[i].mY;
			//std::cout << "holding " << i << std::endl;
		}
	}
}

bool Handler::grabParticles(double xPos, double yPos){
	for(int i = 0; i < mParticles.size(); i++) {
		if(mParticles[i].contains(xPos, yPos)) {
			mHolding = true;
			mParticles[i].mHeld = 1;
			mParticles[i].mGrabX = mParticles[i].mX;
			mParticles[i].mGrabY = mParticles[i].mY;
		}
	}
	return mHolding;
}

void Handler::releaseParticles(){
	if(!mHolding) return;
	for(int i = 0; i <  mParticles.size(); i++) {
		mParticles[i].mHeld = 0;
	}
	mHolding = 0;
}

// void Handler::deleteHeld(){
// 	if(!mHolding) return;
// 	for(int i = 0; i <  mParticles.size(); i++) {
// 		if(mParticles[i].mHeld) removeParticle(mParticles[i]);
// 	}
// 	mHolding = 0;
// }

void Handler::setDelta(double delX, double delY){
	mDelX = delX;
	mDelY = delY;
}

void Handler::tick(){
	for( int j = 0; j < mParticles.size(); j++) {
		for( int i = j+1; i < mParticles.size(); i++) {
			mParticles[i].collide(mParticles[j]);
			mParticles[i].applyForces(mParticles[j]);
		}
		if(mParticles[j].mHeld) mParticles[j].tick(mDelX, mDelY);
		else mParticles[j].tick();

	}
}
