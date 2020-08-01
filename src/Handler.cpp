#include <ElectroSim/Handler.hpp>

Handler::Handler(){

}

float* Handler::getPoints(float* points){
    free(points);
    points = (float*)malloc(sizeof(float) * 2 * CIRCLERESOLUTION * mParticles.size());
    int counter = 0;

    for(auto & p : mParticles){
        for(int i = 0; i < 2 * CIRCLERESOLUTION; i++){
            points[i + CIRCLERESOLUTION * counter] = p.mPoints[i];
        }
        counter++;
    }

    return points;
}

unsigned int* Handler::getIndices(unsigned int* indices){
    free(indices);
    indices = (unsigned int*)malloc(sizeof(unsigned int) * 3 * (CIRCLERESOLUTION - 2) * mParticles.size());
    int counter = 0;

    for(auto & p : mParticles){
        for(int i = 0; i < 3 * (CIRCLERESOLUTION - 2); i++){
            indices[i + CIRCLERESOLUTION * counter] = p.mIndices[i] + counter * (CIRCLERESOLUTION - 1);
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
