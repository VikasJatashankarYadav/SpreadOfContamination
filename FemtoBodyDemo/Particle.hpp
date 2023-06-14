#pragma once
#ifndef _Particle
#define _Particle
#include "PVector.hpp"
class Particle {
private:
	PVector location;
	PVector inital_location;
	PVector velocity;
	PVector acceleration;
	float lifespan;
public:
	static const double size;
	static const double _pi;
	static const double angle[10];
	static const double _cos[10];
	static const double _sin[10];
	Particle();
	Particle(PVector l);
	Particle(PVector l, double vx, double vy, double vz);
	void run();
	void update();
	void display();
	bool isDead();
	double random(double upper, double lower);
};
#endif