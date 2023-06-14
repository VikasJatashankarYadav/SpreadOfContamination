#include "ParticleSystem.hpp"
#include<iostream>
#include<cmath>
#include<stdlib.h>
ParticleSystem::ParticleSystem(PVector l) {
	emitter = l;
}

void ParticleSystem::ParticleSystemUpdate(PVector l) {
	emitter.x = l.x;
	emitter.y = l.y;
	emitter.z = l.z;
}


void ParticleSystem::addParticle()
{
	PVector tmp = random();
	tmp.add(emitter);
	Particle p(tmp, 0.001, 0.001, 0.001);
	particles.push_back(p);
}

void ParticleSystem::addParticleVelocity(double vx, double vy, double vz)
{
	PVector tmp = random();
	tmp.add(emitter);
	Particle p(tmp, vx, vy, vz);
	particles.push_back(p);
	PVector tmp1 = random();
	tmp1.add(emitter);
	Particle p1(tmp1, -0.001, 0.01, -0.01);
	particles.push_back(p1);
	PVector tmp2 = random();
	tmp2.add(emitter);
	Particle p2(tmp2, -0.001, 0.01, -0.01);
	particles.push_back(p2);
	PVector tmp3 = random();
	tmp3.add(emitter);
	Particle p3(tmp3, -0.001, 0.01, -0.01);
	particles.push_back(p3);
	PVector tmp4 = random();
	tmp4.add(emitter);
	Particle p4(tmp4, -0.001, 0.01, -0.01);
	particles.push_back(p4);
	PVector tmp5 = random();
	tmp5.add(emitter);
	Particle p5(tmp5, -0.001, 0.01, -0.01);
	particles.push_back(p5);
}


void ParticleSystem::run() {
	std::vector<Particle>::iterator it;

	for (int i = particles.size()-1; i > -1; i--) {
		if (!(particles[i]).isDead()) {
			(particles[i]).run();
		}
		else {
			particles.erase(particles.begin() + i);
		}
	}
	
}
PVector ParticleSystem::random()
{
	double lower = -0.02;
	double upper = 0.02;
	double range = upper - lower;
	double rand_x = lower + (range * ((double)rand()) / (RAND_MAX));
	double rand_y = lower + (range * ((double)rand()) / (RAND_MAX));	return PVector(rand_x, rand_y, 0);
}
bool ParticleSystem::isEmpty() {
	if (particles.size() == 0)
		return true;
	else
		return false;
}