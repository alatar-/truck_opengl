#include "truck_part.h"

#include <unistd.h>
TruckPart::TruckPart(world_t *in_world, 
					string in_path, 
					float in_size, 
					float ang_in, 
					float x, float y, 
					glm::vec3 in_T, 
					glm::vec3 in_R, 
					glm::vec3 in_S): MV(1.0f) {
	set_pos(x,y);
	ang_h = ang_in;
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
	
	size = in_size;
	
	world = in_world;
	
	model = new model_t();//new anim_mesh_model_t();
	model->load(in_path);

	last_time = glutGet(GLUT_ELAPSED_TIME);
	speed = 5.0 / 1000;
}

TruckPart::~TruckPart() {
	delete model;
}

void TruckPart::draw (bool use_mv) {
	model->draw(use_mv);
}

void TruckPart::set_mv_matrix (glm::mat4 V) {
	MV = V * get_model_matrix();
    model->set_mv_matrix(MV);
}

void TruckPart::apply_mv(){
	glLoadMatrixf(glm::value_ptr(MV));
}

glm::mat4 TruckPart::get_model_matrix() {
	glm::mat4 M = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, 0.0f, pos.y));
// 	M = glm::rotate(M, (float)(PI / 2), glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::rotate(M, ang_h, glm::vec3(0.0f,1.0f,0.0f));
	return M * in_transform;
}

vertex_2d TruckPart::get_pos() {
	return pos;
}

float TruckPart::get_size() {
	return size;
}

void TruckPart::set_pos(float x, float y) {
	pos.x = x;
	pos.y = y;
}

void TruckPart::move (direct_t X) {
    int now = glutGet(GLUT_ELAPSED_TIME); 
	int dt = now - last_time;
	last_time = now;
	set_pos(pos.x, pos.y - (speed * dt) * X);
}