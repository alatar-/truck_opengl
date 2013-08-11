#include <stdio.h>
#include <math.h>

#include "world.h"
#include "shared.h"

#define MOUSE_OVERFLOW 300

world_t *World = new world_t();

int screen_w, screen_h,	
    m_x, m_y;

bool f_key_up, f_key_down,
	 f_key_left, f_key_right,
	 f_key_lower, f_key_higher,
     f_key_w, f_key_s;

void key_down(int c, int x, int y) {
	switch (c) {
		case GLUT_KEY_LEFT:
			f_key_left = true;
			break;
		case GLUT_KEY_RIGHT:
			f_key_right = true;
			break;
		case GLUT_KEY_UP:
			f_key_up = true;
			break;
		case GLUT_KEY_DOWN:
			f_key_down = true;
			break;
		case GLUT_KEY_PAGE_UP:
			f_key_higher = true;
			break;
		case GLUT_KEY_PAGE_DOWN:
			f_key_lower = true;
			break;
	}
}

void key_down(unsigned char c, int x, int y) {
    switch (c) {
        case 'w':
            f_key_w = true;
            break;
        case 's':
            f_key_s = true;
            break;
    }
}

void key_up (int c, int x, int y) {
	switch (c) {
		case GLUT_KEY_LEFT:
			f_key_left = false;
			break;
		case GLUT_KEY_RIGHT:
			f_key_right = false;
			break;
		case GLUT_KEY_UP:
			f_key_up = false;
			break;
		case GLUT_KEY_DOWN:
			f_key_down = false;
			break;
		case GLUT_KEY_PAGE_UP:
			f_key_higher = false;
			break;
		case GLUT_KEY_PAGE_DOWN:
			f_key_lower = false;
			break;

	}
}

void key_up(unsigned char c, int x, int y) {
    switch (c) {
        case 'w':
            f_key_w = false;
            break;
        case 's':
            f_key_s = false;
            break;
    }
}

void mouse_motion(int x, int y) {
	int dx = x - m_x, 
        dy = y - m_y;
	
	if ((abs(dx) < MOUSE_OVERFLOW) && (abs(dy) < MOUSE_OVERFLOW)) {
		World->mouse_motion(dx, dy);
	}

	if (x <= 10 ||
    	x >= screen_w - 10 ||
    	y <= 10 ||
    	y >= screen_h - 10
	) {
		m_x = screen_w / 2;
		m_y = screen_h / 2;
		glutWarpPointer(m_x, m_y);
	} else {
		m_x = x;
		m_y = y;
	}
}

void display_frame() {
	glClearStencil(0);
	glClearColor(0.5, 0.5, 0.8, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	World->draw();
	glutSwapBuffers();
}

void next_frame() {
	World->next_frame(
		(direct_t)(BACK * f_key_right + FORW * f_key_left)
	,	(direct_t)(BACK * f_key_down + FORW * f_key_up)
	,	(direct_t)(BACK * f_key_lower + FORW * f_key_higher)
	);
	glutPostRedisplay();
}

void gl_init(int *argc, char **argv) {
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	
	screen_w = glutGet(GLUT_SCREEN_WIDTH);
	screen_h = glutGet(GLUT_SCREEN_HEIGHT);
	
	glutInitWindowSize(screen_w, screen_h);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Truck Master");
	glutFullScreen();
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutSetCursor(GLUT_CURSOR_NONE);
	
	glewInit();
	// Cieniowanie
	glEnable(GL_NORMALIZE);
	
	glEnable(GL_SMOOTH);
	glShadeModel(GL_SMOOTH);
	
	//?
	glEnable(GL_CULL_FACE);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	// Z bufor
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_TEXTURE_2D);
	
	glutDisplayFunc(display_frame);
	glutIdleFunc(next_frame);
	glutSpecialUpFunc(key_up);
	glutSpecialFunc(key_down);
	glutPassiveMotionFunc(mouse_motion);
	glutMotionFunc(mouse_motion);
}

int main(int argc, char **argv) {
	gl_init(&argc, argv);
	World->load("config.ini", screen_w, screen_h);
	
	glutMainLoop();
	return 0;
}
