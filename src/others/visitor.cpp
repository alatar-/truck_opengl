#include "visitor.h"
#include "random_time.h"

#include <unistd.h>

visitor_t::visitor_t (World *in_world, string in_path, string visiting_path, bool looped_path, float in_speed, float in_wait_time, float in_size, bool in_animated, float fps, unsigned frames, bool skeletal, glm::vec3 in_T, glm::vec3 in_R, glm::vec3 in_S)
: path(visiting_path, looped_path)
,	MV(1.0f) {
	pos = path.position();
	
	in_transform = glm::translate(glm::mat4(1.0f), in_T);
// 	printf("\nvisitor_t::constructor> T\n");
// 	print_mat4(in_transform);
	
	in_transform *= glm::rotate(glm::mat4(1.0f), in_R.x, glm::vec3(1.0f, 0.0f, 0.0f));
// 	printf("\nvisitor_t::constructor> R.x\n");
// 	print_mat4(in_transform);
	
	in_transform *= glm::rotate(glm::mat4(1.0f), in_R.y, glm::vec3(0.0f, 1.0f, 0.0f));
// 	printf("\nvisitor_t::constructor> R.y\n");
// 	print_mat4(in_transform);
	
	in_transform *= glm::rotate(glm::mat4(1.0f), in_R.z, glm::vec3(0.0f, 0.0f, 1.0f));
// 	printf("\nvisitor_t::constructor> R.z\n");
// 	print_mat4(in_transform);
	
// 	printf("S = (%.2f, %.2f, %.2f)\n", in_S.x, in_S.y, in_S.z);
	in_transform *= glm::scale(glm::mat4(1.0f), in_S);
// 	printf("\nvisitor_t::constructor> in_transform\n");
// 	print_mat4(in_transform);
	
	speed = in_speed / 1000;
	size = in_size;
	wait_time = in_wait_time;
	
	animated = in_animated;
	
	world = in_world;
	
	if (skeletal) {
		model = new anim_skeletal_model_t();
	} else {
		model = new anim_mesh_model_t();
	}
	model->load(in_path, fps, frames);
}

visitor_t::~visitor_t() {
	delete model;
}

int visitor_t::fire() {
	pthread_attr_t attr;
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	alive = true;
	
	int rc = pthread_create(&thread, &attr, visitor_routine, (void*)this);
	
	pthread_attr_destroy(&attr);
	
	return rc;
}

int visitor_t::join() {
	return pthread_join(thread, NULL);
}

void visitor_t::kill() {
	alive = false;
}

bool visitor_t::move (Vertex2D<float>itd) {
	bool f, res = true;
	
	world->lock();
		while (!(f = world->test_colls(pos, itd, size)) && alive) {
			world->unlock();
			res = false;
			usleep(random_time(world->get_visitor_collision_delay()));
			world->lock();
		}
		pos = itd;
		ang_h = path.rotation();
	world->unlock();
	
	return res;
}

void visitor_t::life() {
// 	printf("In life\n");
	int last_time = glutGet(GLUT_ELAPSED_TIME);
	while (alive) {
		int now = glutGet(GLUT_ELAPSED_TIME)
			,	dt = now - last_time;
		last_time = now;
		anim_time += dt;
		
		bool state = path.move(speed * dt);
		
		Vertex2D<float>itd = path.position();
// 		printf("life::In loop> path.position: (%f, %f), pos: (%f, %f)\n", itd.x, itd.y, pos.x, pos.y);
		if (itd != pos) {
// 			printf("life::move!\n");
			if (!move(itd)) {
				last_time = glutGet(GLUT_ELAPSED_TIME);
			}
		}
		
		if (!alive) {
// 			printf("life::dead!\n");
			break;
		}
		
		if (state == STOP_AND_STARE) {
// 			printf("life::stare...\n");
			anim_time = 0;
			usleep(random_time(wait_time));
			last_time = glutGet(GLUT_ELAPSED_TIME);
		} else {
// 			printf("life::@delay\n");
			usleep(world->get_visitor_thread_delay());
		}
	}
// 	printf("Out life\n");
}

void visitor_t::animate() {
	if (animated) {
		model->animate(anim_time / 1000);
	}
}

void visitor_t::draw (bool use_mv) {
	model->draw(use_mv);
}

void visitor_t::set_mv_matrix (glm::mat4 V) {
	MV = V * get_model_matrix();
}

void visitor_t::apply_mv(){
	glLoadMatrixf(glm::value_ptr(MV));
}

void visitor_t::set_mv_matrix_to_meshes() {
	model->set_mv_matrix(MV);
}

glm::mat4 visitor_t::get_model_matrix() {
	glm::mat4 M = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, 0.0f, pos.y));
// 	M = glm::rotate(M, (float)(PI / 2), glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::rotate(M, ang_h, glm::vec3(0.0f,1.0f,0.0f));
	return M * in_transform;
}

Vertex2D<float>visitor_t::get_pos() {
	return pos;
}

float visitor_t::get_size() {
	return size;
}

void* visitor_routine(void *arg) {
	((visitor_t*)arg)->life();
	
	pthread_exit(NULL);
	return NULL;
}
