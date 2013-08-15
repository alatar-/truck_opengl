#include <stdio.h>
#include <math.h>

#include "world.h"
#include "shared.h"

#define MOUSE_OVERFLOW 300

int screen_width, screen_height,	
    mouse_x, mouse_y;

bool f_key_up, f_key_down,
	 f_key_left, f_key_right,
	 f_key_lower, f_key_higher,
     f_key_w, f_key_s,
     f_key_a, f_key_d;

World *world = new World();

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

void keyDown(unsigned char c, int x, int y) {
    switch (c) {
        case 'w':
            f_key_w = true;
            break;
        case 's':
            f_key_s = true;
            break;
        case 'a':
            f_key_a = true;
            break;
        case 'd':
            f_key_d = true;
            break;
        case 'p':
            world->tmp_animate();
    }
}

void key_up(int c, int x, int y) {
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

void keyUp(unsigned char c, int x, int y) {
    switch (c) {
        case 'w':
            f_key_w = false;
            break;
        case 's':
            f_key_s = false;
            break;
        case 'a':
            f_key_a = false;
            break;
        case 'd':
            f_key_d = false;
            break;
    }
}

void mouse_motion(int x, int y) {
	int dx = x - mouse_x, 
        dy = y - mouse_y;
	
	if ((abs(dx) < MOUSE_OVERFLOW) && (abs(dy) < MOUSE_OVERFLOW)) {
		world->mouse_motion(dx, dy);
	}

	if (x <= 10 ||
    	x >= screen_width - 10 ||
    	y <= 10 ||
    	y >= screen_height - 10
	) {
		mouse_x = screen_width / 2;
		mouse_y = screen_height / 2;
		glutWarpPointer(mouse_x, mouse_y);
	} else {
		mouse_x = x;
		mouse_y = y;
	}
}

void display_frame() {
	glClearStencil(0);
	glClearColor(0.5, 0.5, 0.8, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	world->draw();
	glutSwapBuffers();
}

void next_frame() {
	world->next_frame(
		(direct_t)(BACK * f_key_right + FRONT * f_key_left),
		(direct_t)(BACK * f_key_down + FRONT * f_key_up),
		(direct_t)(BACK * f_key_lower + FRONT * f_key_higher),
		(direct_t)(BACK * f_key_s    + FRONT * f_key_w),
		(direct_t)(BACK * f_key_a + FRONT * f_key_d)
	);
	glutPostRedisplay();
}

void gl_init(int *argc, char **argv) {
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	
	screen_width = glutGet(GLUT_SCREEN_WIDTH);
	screen_height = glutGet(GLUT_SCREEN_HEIGHT);
	
	glutInitWindowSize(screen_width, screen_height);
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
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);

	glutSpecialUpFunc(key_up);
	glutSpecialFunc(key_down);
	glutPassiveMotionFunc(mouse_motion);
	glutMotionFunc(mouse_motion);
}

int main(int argc, char **argv) {
	gl_init(&argc, argv);
	world->load("config.ini", screen_width, screen_height);
	
	glutMainLoop();
	return 0;
}
