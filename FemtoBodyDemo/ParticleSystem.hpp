#pragma once
#ifndef _ParticleSystem
#define _ParticleSystem
#include<vector>
#include"PVector.hpp"
#include "Particle.hpp"
class ParticleSystem {
private:
	std::vector<Particle> particles;
	PVector emitter;
public:
	ParticleSystem() {};
	ParticleSystem(PVector l);
	void ParticleSystemUpdate(PVector l);
	void addParticle();
	void addParticleVelocity(double vx, double vy, double vz);
	PVector random();
	void run();
	bool isEmpty();
};
#endif