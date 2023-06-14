#include<stdio.h>
#include<iostream>
#include<stdlib.h>




#include<GL/freeglut.h>

#include<cmath>
#include "Particle.hpp"






unsigned int smokeTexture;


// NC
unsigned int _hTexture;

const double Particle::size = 0.04;
const double Particle::_pi = 3.141592;
const double Particle::angle[10] = { 2 * _pi * 1 / 10., 2 * _pi * 2 / 10., 2 * _pi * 3 / 10.,
2 * _pi * 4 / 10., 2 * _pi * 5 / 10., 2 * _pi * 6 / 10., 2 * _pi * 7 / 10., 2 * _pi * 8 / 10., 2 * _pi * 9 / 10., 2 * _pi };
const double Particle::_cos[10] = { size * cos(angle[0]),
size * cos(angle[1]), size * cos(angle[2]), size * cos(angle[3]),
size * cos(angle[4]), size * cos(angle[5]), size * cos(angle[6]),
size * cos(angle[7]), size * cos(angle[8]), size * cos(angle[9]) };
const double Particle::_sin[10] = { size * sin(angle[0]),
size * sin(angle[1]), size * sin(angle[2]), size * sin(angle[3]),
size * sin(angle[4]), size * sin(angle[5]), size * sin(angle[6]),
size * sin(angle[7]), size * sin(angle[8]), size * sin(angle[9]) };


Particle::Particle() {}
Particle::Particle(PVector l) {
	location = l;
	velocity = PVector(0.001, 0.001, 0.001);
	lifespan = 30;
}

Particle::Particle(PVector l, double vx, double vy, double vz) {
	location = l;
	// std::cout << l.x << std::endl;
	velocity = PVector(vx, vy, vz);
	lifespan = 30;
}

void Particle::run() {
	update();
	display();
}
double Particle::random(double upper, double lower) {
	double range = (upper - lower);
	double random_double = lower + (range * ((double)rand()) / (RAND_MAX));
	return random_double;
}
void Particle::update()
{
	double x_acc = velocity.x * 0.01 * (lifespan / 10) ;
	double y_acc = velocity.y * 0.01 * ((30 - lifespan) / 5);
	double z_acc = velocity.z * 0.01 * (lifespan / 10);
	acceleration = PVector(x_acc, y_acc, z_acc);
	velocity.add(acceleration);
	location.add(velocity);
	lifespan -= 0.2;
}
void Particle::display()
{
	glGenTextures(2, &smokeTexture);

	
	
	double inty = 1 - (1 - lifespan / 20);
	glColor4f(0.9, 0.9, 0.9, inty);//, inty, inty);
	float delta_theta = 0.01;
	double _pi = 3.141592;
	glBindTexture(GL_TEXTURE_2D, smokeTexture);
	glBegin(GL_POLYGON); // OR GL_LINE_LOOP
	for (std::size_t i = 0; i < 10; ++i)
	{
		glVertex3f(location.x + 0.15 * _cos[i], location.y + 0.15 * _sin[i], location.z + 0.2 * _cos[i]);
	}
	glEnd();
	glFlush();
}
bool Particle::isDead() {
	if (lifespan < 0.0) {
		return true;
	}
	else {
		return false;
	}
}
