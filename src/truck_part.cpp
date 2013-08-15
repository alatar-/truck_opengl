#include "truck_part.h"

#include <unistd.h>
TruckPart::TruckPart(World *in_world, 
					string in_path, 
					float in_size, 
					float x, float y, 
					glm::vec3 in_T, 
					glm::vec3 in_R, 
					glm::vec3 in_S): MV(1.0f) {
	set_pos(x,y);
	ang_forward = 0;
	ang_follow = 0;
	ang_bend = 0;
	in_transform = glm::translate(glm::mat4(1.0f), in_T);
	in_transform *= glm::rotate(glm::mat4(1.0f), in_R.x, glm::vec3(1.0f, 0.0f, 0.0f));
	in_transform *= glm::rotate(glm::mat4(1.0f), in_R.y, glm::vec3(0.0f, 1.0f, 0.0f));
	in_transform *= glm::rotate(glm::mat4(1.0f), in_R.z, glm::vec3(0.0f, 0.0f, 1.0f));
	in_transform *= glm::scale(glm::mat4(1.0f), in_S);
	size = in_size;
	world = in_world;
	model = new Model();//new anim_mesh_Model();
	model->load(in_path);
	last_time = glutGet(GLUT_ELAPSED_TIME);
	speed = 5;
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
	glm::mat4 R_forward = glm::rotate(glm::mat4(1.0f), to_degrees(ang_forward), glm::vec3(1.0f,0.0f,0.0f));
	glm::mat4 R_bend = glm::rotate(glm::mat4(1.0f), to_degrees(ang_bend), glm::vec3(0.0f,1.0f,0.0f));
	glm::mat4 R_follow = glm::rotate(glm::mat4(1.0f), to_degrees(ang_follow), glm::vec3(0.0f,1.0f,0.0f));
	glm::mat4 M = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, 0.0f, pos.y));
	return M * R_follow * in_transform * R_bend * R_forward;
}

Vertex2D<float>TruckPart::get_pos() {
	return pos;
}

float TruckPart::get_size() {
	return size;
}

void TruckPart::set_pos(float x, float y) {
	pos.x = x;
	pos.y = y;
}

void TruckPart::move (Vertex2D<float>position, float ang, float ds) {
	pos = position;
	ang_forward = normalize_angle(ang_forward + ds / size);
	ang_follow = ang;
}

void TruckPart::rotate (float in_ang_bend) {
	ang_bend = in_ang_bend;
}

float TruckPart::get_model_width() {
	return model->get_width();
}

float TruckPart::get_model_length() {
	return model->get_length();
}

Vertex2D<float>TruckPart::get_model_min_point() {
	return model->get_min_point();
}

Vertex2D<float>TruckPart::get_model_max_point() {
	return model->get_max_point();
}
