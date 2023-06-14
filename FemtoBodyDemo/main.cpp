
#include <iostream>
#include <GL/freeglut.h>

/* Graphics */
#include <ModelCamera.h>
#include <Palette.h>
#include <StudioPalettes.h>

/* Camera */
#include <TrackingCamera.h>

/* OpenCV */
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace px;

std::shared_ptr<TrackingCamera> femto = nullptr;
std::shared_ptr<ModelCamera<Real>> camera = nullptr;
std::shared_ptr<Palette<Real>> palette = nullptr;

bool init() {


	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	camera = std::make_shared<ModelCamera<Real>>();

	femto = std::make_shared<TrackingCamera>(false, true);
    bool bFemtoConnected = femto->connect();

	palette = std::make_shared<Palette<Real>>();
	palette->load(palette_jet);

	return true;
}

inline void DrawBone(const Joint& p0, const Joint& p1, Real scale) {
	glVertex3f(p0.x*scale, p0.y*scale, p0.z*scale);
	glVertex3f(p1.x*scale, p1.y*scale, p1.z*scale);
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

		
		const auto& bodies = femto->getBodies();
		// std::cout << bodies.size() << std::endl;
		for ( std::size_t i = 0; i < bodies.size(); i++ ) {
			auto status = bodies[i].getStatus();
			if ( status == BODY_NOT_TRACKING ) continue;

			const auto& body = bodies[i];
			const auto& joints = body.getJoints();
				
			glPointSize(12.0f);
			glBegin(GL_POINTS);
			glColor3f(1.0f, 0.0f, 0.0f);
			for ( std::size_t i = 0; i < joints.size(); i++ ) {
				const auto& joint = joints[i];
				glVertex3f(joint.x*scale, joint.y*scale, joint.z*scale);
			}
			glEnd();

			const auto& jHead = joints[JOINT_HEAD];
			const auto& jNeck = joints[JOINT_NECK];
			const auto& jShoulder = joints[JOINT_SHOULDER_SPINE];
			const auto& jLeftShoulder = joints[JOINT_LEFT_SHOULDER];
			const auto& jRightShoulder = joints[JOINT_RIGHT_SHOULDER];
			const auto& jSpineMid = joints[JOINT_MID_SPINE];
			const auto& jSpineBase = joints[JOINT_BASE_SPINE];
			const auto& jLeftHip = joints[JOINT_LEFT_HIP];
			const auto& jRightHip = joints[JOINT_RIGHT_HIP];
			const auto& jLeftKnee = joints[JOINT_LEFT_KNEE];
			const auto& jRightKnee = joints[JOINT_RIGHT_KNEE];
			const auto& jLeftFoot = joints[JOINT_LEFT_FOOT];
			const auto& jRightFoot = joints[JOINT_RIGHT_FOOT];
			const auto& jLeftElbow = joints[JOINT_LEFT_ELBOW];
			const auto& jRightElbow = joints[JOINT_RIGHT_ELBOW];
			const auto& jLeftHand = joints[JOINT_LEFT_HAND];
			const auto& jRightHand = joints[JOINT_RIGHT_HAND];

			glLineWidth(4.0f);
			glColor3f(0.0f, 1.0f, 0.0f);
			glBegin(GL_LINES);
				DrawBone(jHead, jNeck, scale);
				DrawBone(jNeck, jShoulder, scale);
				DrawBone(jShoulder, jLeftShoulder, scale);
				DrawBone(jShoulder, jRightShoulder, scale);
				DrawBone(jLeftShoulder, jLeftElbow, scale);
				DrawBone(jRightShoulder, jRightElbow, scale);
				DrawBone(jLeftElbow, jLeftHand, scale);
				DrawBone(jRightElbow, jRightHand, scale);
				DrawBone(jShoulder, jSpineMid, scale);
				DrawBone(jSpineMid, jSpineBase, scale);
				DrawBone(jSpineBase, jLeftHip, scale);
				DrawBone(jSpineBase, jRightHip, scale);
				DrawBone(jLeftHip, jLeftKnee, scale);
				DrawBone(jRightHip, jRightKnee, scale);
				DrawBone(jLeftKnee, jLeftFoot, scale);
				DrawBone(jRightKnee, jRightFoot, scale);
			glEnd();
		}




		const auto& depth_cloud = femto->getDepthCloud();
		if ( depth_cloud ) {
			glPointSize(1.0f);
			glColor3f(0.4f, 0.4f, 0.4f);
			const auto& cloud_data = depth_cloud->constData();
			std::size_t width = depth_cloud->width();
			std::size_t height = depth_cloud->height();

			PaletteElement<Real> e;

			glBegin(GL_POINTS);
			for ( std::size_t i = 0; i < height; i++ ) {
				for ( std::size_t j = 0; j < width; j++ ) {
					const auto& p = cloud_data[i * width + j];
					Real depth = p.z*scale;
					Real depth_n = depth;
					Scale(depth_n, Real(0), Real(6), Real(0), Real(1));
					palette->get(depth_n, e);
					glColor3f(e.r, e.g, e.b);
					glVertex3f(p.x*scale, p.y*scale, depth);
				}
			}
			glEnd();
		}
		

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
	if ( femto ) {
		if ( femto->isConnected() ) femto->update();
	}

	glutPostRedisplay();
}

void mouseFunc(int button, int state, int x, int y) {
	if ( button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN ) camera->bPan = true;
	if ( button == GLUT_MIDDLE_BUTTON && state == GLUT_UP ) camera->bPan = false;
	if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) camera->bRotate = true;
	if ( button == GLUT_LEFT_BUTTON && state == GLUT_UP ) camera->bRotate = false;
	if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN ) camera->bZoom = true;
	if ( button == GLUT_RIGHT_BUTTON && state == GLUT_UP ) camera->bZoom = false;

	camera->mouse_last = {x, y};
	camera->mouse_cur = {x, y};
}

void mouseMotionFunc(int x, int y) {
	camera->mouse_cur = {x, y};

	const double panSen = 1.0;
	const double rotSen = 1.0;
	const double zoomSen = 0.1;

	Vector2<int32_t> diff = camera->mouse_last - camera->mouse_cur;
	if ( camera->bPan ) camera->pan(diff.x() * panSen, diff.y() * panSen);
	if ( camera->bRotate ) camera->rotate(diff.x()* rotSen, diff.y() * rotSen);
	if ( camera->bZoom ) camera->zoom(diff.y() * zoomSen);

	camera->mouse_last = camera->mouse_cur;
}

void atExit() {
	femto->disconnect();
}

void keyboard(unsigned char key, int xasdf, int yasdf) {
	
}

int main(int argc, char** argv) {
	glutInit (&argc, argv);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Orbbec Femto Tracking+Depth");
	if ( init() ==  false ) return 1;
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