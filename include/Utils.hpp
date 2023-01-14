#ifndef UTILS_HPP
#define UTILS_HPP

#include <nano/nano.hpp>
#include <cstdlib>


namespace utils {

void seed(){
	srand(ns());
}

// return random number from -0.5 to 0.5
float randNum(){
	return (((float)rand() - (float)RAND_MAX/2) / (float)RAND_MAX);
}

}

#endif
