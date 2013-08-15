#include "world.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include "vehicle.h"

glm::vec3 str_to_vec3(const string &in_s) {
	float x, y,	z;
	printf("in_s: %s\n", in_s.c_str());
	sscanf(in_s.c_str(), " %f / %f / %f", &x, &y, &z);
	return glm::vec3(x, y, z);
}

vector <float> str_to_vec(const string &in_s) {
	float x, y, rotation;
	printf("in_s: %s\n", in_s.c_str());
	sscanf(in_s.c_str(), " %f / %f / %f", &x, &y, &rotation);
	vector <float> result;
	result.push_back(x);
	result.push_back(y);
	result.push_back(rotation);
	return result;
}

bool materials_ptr_less(const Material *a, const Material *b) {
	return *a < *b;
}

void World::tmp_animate() {
    this->camera->tmp_animate();
}

bool World::load(string in_config_file, unsigned in_screen_w, unsigned in_screen_h) {
	/** load config */
	ini_t ini(in_config_file, true);
	
	{
		ini.select("World");
		mouse_sensitivity_x = ini.get<float>("mouseSensitivityX", 0.8f);
		mouse_sensitivity_y = ini.get<float>("mouseSensitivityY", 0.8f);
		invert_mouse_y = ini.get<bool>("invertMouseY", false);
	}
	
	{
		ini.select("Projection");
		P = glm::perspective(
			ini.get<float>("fovy", 50.0f)
		,	ini.get<float>("aspect", (float)in_screen_w / in_screen_h)
		,	ini.get<float>("zNear", 1.0f)
		,	ini.get<float>("zFar", 400.0f)
		);
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(P));
	}
	
	{
		ini.select("Camera");
		camera = new Camera(
			ini.get<float>("posX", 0.0f),
			ini.get<float>("posY", 0.0f),
			ini.get<float>("posZ", 5.0f),
            ini.get<float>("angX", 0.0f),
			ini.get<float>("speed", 20.0f),
            ini.get<float>("max_vertical_angle_up", 0.5f),
            ini.get<float>("max_vertical_angle_down", 0.5f),
            ini.get<float>("animation_rand_range", 0.5f),
            ini.get<float>("animation_time_estimated", 5),
            ini.get<float>("animation_speed", 0.5f)
		);
	}
	
	{
		{
			ini.select("Parking");
			parking = new model_t();
			string model_file("./models/");
			model_file += ini.get<string>("model", "Parking.obj");
			if (!parking->load(model_file)) {
				return false;
			}
			vector <Material*> &parking_mats = parking->get_materials();
			materials.insert(materials.begin(), parking_mats.begin(), parking_mats.end());
		}

		{
			string model_file("./models/");
			model_file += ini.get<string>("meta_model", "Parking/meta.obj");
			meta = new Obstacle(
				this,
				model_file
			);
			vector <Material*> &meta_mats = meta->get_materials();
			meta->set_vertices();
			materials.insert(materials.begin(), meta_mats.begin(), meta_mats.end());
		}

		{
			int obstacle_number = ini.get<int>("obstacle_number", 0);
			string pre = "./models/";
			pre += ini.get<string>("obstacle_model", "Parking/Obstacle");
			string post = ini.get<string>("obstacle_suffix", ".obj");
			for(int i = 1 ; i <= obstacle_number; ++i) {
				char number[15];
				sprintf(number, "%03d", i);
				string name("");
				name += pre;
				name += number;
				name += post;
				printf("\n%s\n", name.c_str());
				obstacles.push_back(new Obstacle(
							this
						,	name
					));
				obstacles.back()->set_vertices();
				vector <Material*> &obst_mats = obstacles.back()->get_materials();
				materials.insert(materials.begin(), obst_mats.begin(), obst_mats.end());
			}
		}
	}

	{
		{
			ini.select("Vehicle_consts");

			truck = new Vehicle(ini.get<float>("size", 1.0f)
			,	ini.get<float>("max_acceleration", 0.1)
			,	ini.get<float>("max_velocity", 90)
			,	ini.get<float>("max_reverse_velocity", 30)
			, ini.get<float>("turn_acceleration", 1)
			, ini.get<float>("min_turn", 1)
			, ini.get<float>("max_turn", 10)
			, ini.get<float>("begining_turn_point", 0.15)
			, ini.get<float>("max_turn_point", 0.3)
			, ini.get<float>("final_turn_point", 0.6)
			, ini.get<float>("max_following_bend", 0.4)
			, ini.get<float>("time_following_bend", 2)
				);
		}

		{
			string model_file("./models/");
			ini.select("Truck");
			truck->body = new TruckPart(this
					,	model_file + ini.get<string>("model", "Semi_truck.obj")
					,	ini.get<float>("size", 2.0f)
					,	ini.get<float>("posX", 2.0f)
					,	ini.get<float>("posY", 2.0f)
					,	str_to_vec3(ini.get<string>("translate", "0/0/0"))
					,	str_to_vec3(ini.get<string>("rotate", "0/0/0"))
					,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
					);
			vector <Material*> &vis_mats = truck->body->get_materials();
			materials.insert(materials.begin(), vis_mats.begin(), vis_mats.end());
			printf("Got materials\n");
			truck->set_vertices();
		}

		{
			ini.select("Truck");
			float posX = ini.get<float>("posX", 0.0f), posY = ini.get<float>("posY", 0.0f); 
			ini.select("TruckSteeringWheels");
			string model_file("./models/");
			truck->left_steering_wheel = new TruckPart(this
						,	model_file + ini.get<string>("model", "single_wheel.obj")
						,	ini.get<float>("leftS", 2.0f)
						,	posX
						,	posY
						,	glm::vec3(ini.get<float>("leftX", 2.0f), 0.0f, ini.get<float>("leftY", 2.0f)) + str_to_vec3(ini.get<string>("translate", "0/0/0"))
						,	str_to_vec3(ini.get<string>("rotate", "0/0/0"))
						,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
						);
			vector <Material*> &vis_mats = truck->left_steering_wheel->get_materials();
			materials.insert(materials.begin(), vis_mats.begin(), vis_mats.end());
			printf("Got materials\n");
		}

		{
			ini.select("Truck");
			float posX = ini.get<float>("posX", 0.0f), posY = ini.get<float>("posY", 0.0f); 
			ini.select("TruckSteeringWheels");
			string model_file("./models/");
			truck->right_steering_wheel = new TruckPart(this
						,	model_file + ini.get<string>("model", "single_wheel.obj")
						,	ini.get<float>("rightS", 2.0f)
						,	posX
						,	posY
						,	glm::vec3(ini.get<float>("rightX", 2.0f), 0.0f, ini.get<float>("rightY", 2.0f)) + str_to_vec3(ini.get<string>("translate", "0/0/0"))
						,	str_to_vec3(ini.get<string>("rotate", "0/180/0"))
						,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
						);
			vector <Material*> &vis_mats = truck->right_steering_wheel->get_materials();
			materials.insert(materials.begin(), vis_mats.begin(), vis_mats.end());
			printf("Got materials\n");
		}

		{
			ini.select("Truck");
			float posX = ini.get<float>("posX", 0.0f), posY = ini.get<float>("posY", 0.0f); 
			printf("loading double wheels 1\n");
			ini.select("TruckDoubleWheels");
			string model_file("./models/");
			for(int i = 0; i < 2; ++i) {
				stringstream ssX;
				string leftX("leftX");
				ssX<<leftX<<i;
				leftX = ssX.str();
				stringstream ssY;
				string leftY("leftY");
				ssY<<leftY<<i;
				leftY = ssY.str();
				truck->left_wheels.push_back(new TruckPart(this
							,	model_file + ini.get<string>("model", "double_wheel.obj")
							,	ini.get<float>("leftS", 2.0f)
							,	posX
							,	posY
							,	glm::vec3(ini.get<float>(leftX, 2.0f), 0.0f, ini.get<float>(leftY, 2.0f)) + str_to_vec3(ini.get<string>("translate", "0/0/0"))
							,	str_to_vec3(ini.get<string>("rotate", "0/0/0"))
							,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
							));
				printf("loading double wheels 2\n");
				vector <Material*> &vis_mats = truck->left_wheels.back()->get_materials();
				printf("loading double wheels 3\n");
				materials.insert(materials.begin(), vis_mats.begin(), vis_mats.end());
				printf("Got materials\n");
			}
		}

		{
			ini.select("Truck");
			float posX = ini.get<float>("posX", 0.0f), posY = ini.get<float>("posY", 0.0f); 
			printf("loading double wheels 1\n");
			ini.select("TruckDoubleWheels");
			string model_file("./models/");
			for(int i = 0; i < 2; ++i) {
				stringstream ssX;
				string rightX("rightX");
				ssX<<rightX<<i;
				rightX = ssX.str();
				stringstream ssY;
				string rightY("rightY");
				ssY<<rightY<<i;
				rightY = ssY.str();
				truck->right_wheels.push_back(new TruckPart(this
							,	model_file + ini.get<string>("model", "double_wheel.obj")
							,	ini.get<float>("rightS", 2.0f)
							,	posX
							,	posY
							,	glm::vec3(ini.get<float>(rightX, 2.0f), 0.0f, ini.get<float>(rightY, 2.0f)) + str_to_vec3(ini.get<string>("translate", "0/0/0"))
							,	str_to_vec3(ini.get<string>("rotate", "0/180/0"))
							,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
							));
				printf("loading double wheels 2\n");
				vector <Material*> &vis_mats = truck->right_wheels.back()->get_materials();
				printf("loading double wheels 3\n");
				materials.insert(materials.begin(), vis_mats.begin(), vis_mats.end());
				printf("Got materials\n");
			}
		}


		{
			ini.select("Trailer1");
			first_trailer = new Vehicle(ini.get<float>("size", 1.0f), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
			{
				string model_file("./models/");
				printf("trailer1!!!!!!!\n");
				first_trailer->body = new TruckPart(this
						,	model_file + ini.get<string>("model", "Truck/Trailer_no_wheels.obj")
						,	ini.get<float>("size", 2.0f)
						,	ini.get<float>("posX", 2.0f)
						,	ini.get<float>("posY", 2.0f)
						,	str_to_vec3(ini.get<string>("translate", "0/0/0"))
						,	str_to_vec3(ini.get<string>("rotate", "0/0/0"))
						,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
						);
				vector <Material*> &vis_mats = first_trailer->body->get_materials();
				materials.insert(materials.begin(), vis_mats.begin(), vis_mats.end());
				printf("Got materials\n");
				first_trailer->set_vertices();
			}

			{
				ini.select("Trailer1");
				float posX = ini.get<float>("posX", 0.0f), posY = ini.get<float>("posY", 0.0f); 
				printf("loading double wheels 1\n");
				ini.select("Trailer1DoubleWheels");
				string model_file("./models/");
				for(int i = 0; i < 2; ++i) {
					stringstream ssX;
					string leftX("leftX");
					ssX<<leftX<<i;
					leftX = ssX.str();
					stringstream ssY;
					string leftY("leftY");
					ssY<<leftY<<i;
					leftY = ssY.str();
					first_trailer->left_wheels.push_back(new TruckPart(this
								,	model_file + ini.get<string>("model", "double_wheel.obj")
								,	ini.get<float>("leftS", 2.0f)
								,	posX
								,	posY
								,	glm::vec3(ini.get<float>(leftX, 2.0f), 0.0f, ini.get<float>(leftY, 2.0f)) + str_to_vec3(ini.get<string>("translate", "0/0/0"))
								,	str_to_vec3(ini.get<string>("rotate", "0/0/0"))
								,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
								));
					printf("loading double wheels 2\n");
					vector <Material*> &vis_mats = first_trailer->left_wheels.back()->get_materials();
					printf("loading double wheels 3\n");
					materials.insert(materials.begin(), vis_mats.begin(), vis_mats.end());
					printf("Got materials\n");
				}
			}

			{
				ini.select("Trailer1");
				float posX = ini.get<float>("posX", 0.0f), posY = ini.get<float>("posY", 0.0f); 
				printf("loading double wheels 1\n");
				ini.select("Trailer1DoubleWheels");
				string model_file("./models/");
				for(int i = 0; i < 2; ++i) {
					stringstream ssX;
					string rightX("rightX");
					ssX<<rightX<<i;
					rightX = ssX.str();
					stringstream ssY;
					string rightY("rightY");
					ssY<<rightY<<i;
					rightY = ssY.str();
					first_trailer->right_wheels.push_back(new TruckPart(this
								,	model_file + ini.get<string>("model", "double_wheel.obj")
								,	ini.get<float>("rightS", 2.0f)
								,	posX
								,	posY
								,	glm::vec3(ini.get<float>(rightX, 2.0f), 0.0f, ini.get<float>(rightY, 2.0f)) + str_to_vec3(ini.get<string>("translate", "0/0/0"))
								,	str_to_vec3(ini.get<string>("rotate", "0/180/0"))
								,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
								));
					printf("loading double wheels 2\n");
					vector <Material*> &vis_mats = first_trailer->right_wheels.back()->get_materials();
					printf("loading double wheels 3\n");
					materials.insert(materials.begin(), vis_mats.begin(), vis_mats.end());
					printf("Got materials\n");
				}
			}
		}

		{
			ini.select("Trailer2");
			second_trailer = new Vehicle(ini.get<float>("size", 1.0f), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
			{
				string model_file("./models/");
				second_trailer->body = new TruckPart(this
						,	model_file + ini.get<string>("model", "Second_trailer_no_wheels.obj")
						,	ini.get<float>("size", 2.0f)
						,	ini.get<float>("posX", 2.0f)
						,	ini.get<float>("posY", 2.0f)
						,	str_to_vec3(ini.get<string>("translate", "0/0/0"))
						,	str_to_vec3(ini.get<string>("rotate", "0/0/0"))
						,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
						);
				vector <Material*> &vis_mats = second_trailer->body->get_materials();
				materials.insert(materials.begin(), vis_mats.begin(), vis_mats.end());
				printf("Got materials\n");
				second_trailer->set_vertices();
			}

			{
				ini.select("Trailer2");
				float posX = ini.get<float>("posX", 0.0f), posY = ini.get<float>("posY", 0.0f); 
				printf("loading double wheels 1\n");
				ini.select("Trailer2DoubleWheels");
				string model_file("./models/");
				for(int i = 0; i < 4; ++i) {
					stringstream ssX;
					string leftX("leftX");
					ssX<<leftX<<i;
					leftX = ssX.str();
					stringstream ssY;
					string leftY("leftY");
					ssY<<leftY<<i;
					leftY = ssY.str();
					second_trailer->left_wheels.push_back(new TruckPart(this
								,	model_file + ini.get<string>("model", "double_wheel.obj")
								,	ini.get<float>("leftS", 2.0f)
								,	posX
								,	posY
								,	glm::vec3(ini.get<float>(leftX, 2.0f), 0.0f, ini.get<float>(leftY, 2.0f)) + str_to_vec3(ini.get<string>("translate", "0/0/0"))
								,	str_to_vec3(ini.get<string>("rotate", "0/0/0"))
								,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
								));
					printf("loading double wheels 2\n");
					vector <Material*> &vis_mats = second_trailer->left_wheels.back()->get_materials();
					printf("loading double wheels 3\n");
					materials.insert(materials.begin(), vis_mats.begin(), vis_mats.end());
					printf("Got materials\n");
				}
			}

			{
				ini.select("Trailer2");
				float posX = ini.get<float>("posX", 0.0f), posY = ini.get<float>("posY", 0.0f); 
				printf("trailer2X: %f trailer2Y: %f\n", posX, posY);
				printf("loading double wheels 1\n");
				ini.select("Trailer2DoubleWheels");
				string model_file("./models/");
				for(int i = 0; i < 4; ++i) {
					stringstream ssX;
					string rightX("rightX");
					ssX<<rightX<<i;
					rightX = ssX.str();
					stringstream ssY;
					string rightY("rightY");
					ssY<<rightY<<i;
					rightY = ssY.str();
					second_trailer->right_wheels.push_back(new TruckPart(this
								,	model_file + ini.get<string>("model", "double_wheel.obj")
								,	ini.get<float>("rightS", 2.0f)
								,	posX
								,	posY
								,	glm::vec3(ini.get<float>(rightX, 2.0f), 0.0f, ini.get<float>(rightY, 2.0f)) + str_to_vec3(ini.get<string>("translate", "0/0/0"))
								,	str_to_vec3(ini.get<string>("rotate", "0/180/0"))
								,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
								));
					printf("loading double wheels 2\n");
					vector <Material*> &vis_mats = second_trailer->right_wheels.back()->get_materials();
					printf("loading double wheels 3\n");
					materials.insert(materials.begin(), vis_mats.begin(), vis_mats.end());
					printf("Got materials\n");
				}
			}
		}
		truck->following_vehicle = first_trailer;
		first_trailer-> following_vehicle = second_trailer;
	}

	{
		red_marker = NULL;
		ini.select("RedMarker");
		if (ini.get<bool>("on", false)) {
			string model_file("./models/");
			model_file += ini.get<string>("model", "Parking/red_marker.obj");
			red_marker = new model_t();
			red_marker->load(model_file);
		}
	}

	{
		sort(materials.begin(), materials.end(), materials_ptr_less);
		unsigned i = 0, j = 1, len = materials.size();
		for (; j < len; ++j) {
			if (*materials[i] == *materials[j]) {
				materials[j]->substitute(materials[i]);
				delete materials[j];
			} else {
				i += 1;
				if (i < j) {
					materials[i] = materials[j];
				}
			}
		}
		if (i < len) {
			materials.resize(i + 1);
		}
	}

	{
		vector <Obstacle*> empty_obst;
		truck->calculate(STOP, STOP, empty_obst);
	}

  return true;
}

World::~World() {	
	delete parking;
	delete camera;
}

void World::clear() {
	delete parking;
	delete camera;
}

void World::draw() {
    camera->animate_crash();
	glm::mat4 V = camera->get_view_matrix();
	
	/**
	 * Recalculate models
	 */
	
	parking->set_mv_matrix(glm::mat4(1.0f));
	truck->body->set_mv_matrix(glm::mat4(1.0f));
	truck->left_steering_wheel->set_mv_matrix(glm::mat4(1.0f));
	truck->right_steering_wheel->set_mv_matrix(glm::mat4(1.0f));
	for (unsigned i = 0, len = truck->left_wheels.size(); i < len; ++i) {
		truck->left_wheels[i]->set_mv_matrix(glm::mat4(1.0f));
		truck->right_wheels[i]->set_mv_matrix(glm::mat4(1.0f));
	}
	first_trailer->body->set_mv_matrix(glm::mat4(1.0f));
	for (unsigned i = 0, len = first_trailer->left_wheels.size(); i < len; ++i) {
		first_trailer->left_wheels[i]->set_mv_matrix(glm::mat4(1.0f));
		first_trailer->right_wheels[i]->set_mv_matrix(glm::mat4(1.0f));
	}
	second_trailer->body->set_mv_matrix(glm::mat4(1.0f));
	for (unsigned i = 0, len = second_trailer->left_wheels.size(); i < len; ++i) {
		second_trailer->left_wheels[i]->set_mv_matrix(glm::mat4(1.0f));
		second_trailer->right_wheels[i]->set_mv_matrix(glm::mat4(1.0f));
	}
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDisable(GL_CULL_FACE);
	draw_all_markers(V);

	draw_in_material_order(V);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void World::draw_in_material_order(glm::mat4 V) {
	glMatrixMode(GL_MODELVIEW);
	
	for (unsigned i = 0, ilen = materials.size(); i < ilen; ++i) {
		materials[i]->draw_associated_meshes(V);
	}
}

void World::draw_all_markers(glm::mat4 V) {
	if (red_marker) {
		glMatrixMode(GL_MODELVIEW);
		
		red_marker->get_materials().back()->apply();
		for (unsigned i = 0, ilen = obstacles.size(); i < ilen; ++i) {
			draw_rectangle(*obstacles[i], V);
		}
		draw_rectangle(*meta, V);

		Vehicle *curr_vehicle = truck;
		bool flag = false;
		while (curr_vehicle != NULL) {
			draw_rectangle(*curr_vehicle, V);
			curr_vehicle = curr_vehicle->following_vehicle;
			red_marker->get_materials()[flag]->apply();
			flag = !flag;
		}
	}
}

void World::draw_rectangle(Rectangle &rect, glm::mat4 V) {
	vector <Vertex2D<float> >verts = rect.get_vertices();
	for (unsigned i = 0, ilen = verts.size(); i < ilen; ++i) {
		draw_marker(verts[i], V);
	}
}

void World::draw_marker (Vertex2D<float>pos, glm::mat4 V) {
	glm::mat4 M = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, 0.0f, pos.y));
	red_marker->set_mv_matrix(V * M);
	red_marker->draw(true, false);
}


void World::next_frame (direct_t cam_right_left, direct_t cam_front_back, direct_t cam_up_down, direct_t veh_front_back, direct_t veh_right_left) {
	camera->move(cam_right_left, cam_front_back, cam_up_down);
	truck->calculate(veh_front_back, veh_right_left, obstacles);
	if(is_win()) {
		printf("Wygrałeś!!!\n");
	}
}

void World::mouse_motion(float dang_h, float dang_v) {
	if (invert_mouse_y) {
		dang_v *= -1;
	}
	camera->mouse_motion(dang_h * mouse_sensitivity_x / 300, dang_v * mouse_sensitivity_y / 300);
}

bool World::is_win() {
	int i = 0;
	Vehicle *curr_vehicle = truck;

	while (curr_vehicle != NULL) {
		if (meta->full_inclusion(*curr_vehicle)) {
			i += 1;
			curr_vehicle = curr_vehicle->following_vehicle;
		} else {
			return false;
		}
	}
	return true;
}
