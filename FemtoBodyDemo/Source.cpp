#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include "ModelCamera.h"
#include <GL/freeglut.h>

#include <png.h>
#include "Particle.hpp"
#include "PVector.hpp"
#include "ParticleSystem.hpp"

#include <chrono>

using namespace strivision;

using namespace std;
using namespace std::chrono;


template <typename Real>
struct Point {
	Real x, y, z;
	Real r, g, b;
};


PVector emission_location(0, 0, 0);

PVector new_location(0.5, 0.5, 0.5);
ParticleSystem ps(emission_location);

std::shared_ptr<ModelCamera<Real>> camera = nullptr;

auto t1 = high_resolution_clock::now();

int ct = 0;

bool init() {
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	camera = std::make_shared<ModelCamera<Real>>();

	


	return true;
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const Real scale = 0.001;

	glPushMatrix();
	glLoadIdentity();
	glMultMatrixf(camera->getViewMatrix().data());

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();


	

	auto t2 = high_resolution_clock::now();

	
	if (ct<100) {
		
		ct++;
	}
	else {
	// ps.ParticleSystemUpdate(new_location);
		ct++;
	}

	// ps.addParticle();
	ps.addParticleVelocity(0.001, 0.001, 0.001);
	// glClearColor(1.0, 1.0, 1.0, 1.0);
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ps.run();

	glPopMatrix();
	glFlush();
}

void reshape(GLint width, GLint height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, (float)width / height, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}

void idle() {
	
	glutPostRedisplay();
}

void mouseFunc(int button, int state, int x, int y) {
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) camera->bPan = true;
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) camera->bPan = false;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) camera->bRotate = true;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) camera->bRotate = false;
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) camera->bZoom = true;
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) camera->bZoom = false;

	camera->mouse_last = { x, y };
	camera->mouse_cur = { x, y };
	glutPostRedisplay();
}

void mouseMotionFunc(int x, int y) {
	camera->mouse_cur = { x, y };

	const double panSen = 1.0;
	const double rotSen = 1.0;
	const double zoomSen = 0.1;

	Vector2<int32_t> diff = camera->mouse_last - camera->mouse_cur;
	if (camera->bPan) camera->pan(diff.x() * panSen, diff.y() * panSen);
	if (camera->bRotate) camera->rotate(diff.x() * rotSen, diff.y() * rotSen);
	if (camera->bZoom) camera->zoom(diff.y() * zoomSen);

	camera->mouse_last = camera->mouse_cur;
	glutPostRedisplay();
}

void atExit() {
	
}

void keyboard(unsigned char key, int xasdf, int yasdf) {

}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Orbbec Femto Tracking+Depth");
	if (init() == false) return 1;
	std::cout << "A";
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseFunc);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(mouseMotionFunc);
	glutIdleFunc(idle);
	std::atexit(atExit);
	glutMainLoop();
	return 0;
}