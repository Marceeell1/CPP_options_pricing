#include "MT.h"

MT::MT() : mt(std::random_device{}()), uniformDist(0.0,1.0), normalDist(0.0,1.0){}

MT& MT::getInstance() {
	static MT instance;
	return instance;
}
double MT::rand_unif() {
	return getInstance().uniformDist(getInstance().mt);
}
double MT::rand_norm() {
	return getInstance().normalDist(getInstance().mt);
}