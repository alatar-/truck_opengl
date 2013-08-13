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

bool materials_ptr_less(const material_t *a, const material_t *b) {
	return !(*a < *b);
}

bool world_t::load(string in_config_file, unsigned in_screen_w, unsigned in_screen_h) {
	/** load config */
	ini_t ini(in_config_file, true);
	
	{
		ini.select("World");
		mouse_sensitivity_x = ini.get<float>("mouseSensitivityX", 0.8f);
		mouse_sensitivity_y = ini.get<float>("mouseSensitivityY", 0.8f);
		invert_mouse_y = ini.get<bool>("invertMouseY", false);

		shadow_map_size = ini.get<unsigned>("shadowMapSize", 512);
		shadows_working = ini.get<bool>("shadows", false);
	}
	
	{
		ini.select("Projection");
		P = glm::perspective(
			ini.get<float>("fovy", 50.0f)
		,	ini.get<float>("aspect", (float)screen_w/screen_h)
		,	ini.get<float>("zNear", 1.0f)
		,	ini.get<float>("zFar", 400.0f)
		);
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(P));
	}
	
	{
		ini.select("Player");
		player = new player_t(
			this
		,	ini.get<float>("posX", 0.0f)
		,	ini.get<float>("posZ", 0.0f)
		,	ini.get<float>("angX", 0.0f)
		,	ini.get<float>("height", 5.0f)
		,	ini.get<float>("speed", 20.0f)
		,	ini.get<float>("size", 2.0f)
		);
	}
	
	{
		ini.select("Parking");
		galery = new model_t();
		string model_file("./models/");
		model_file += ini.get<string>("model", "galery.obj");
		if (!galery->load(model_file)) {
			return false;
		}
		
		vector <material_t*> &gal_mats = galery->get_materials();
		materials.insert(materials.begin(), gal_mats.begin(), gal_mats.end());
		printf("done\n");
	}

	// {
	// 	ini.select("Truck");
	// 	string model_file("./models/Galery/Semi_truck.obj");
	// 	//model_file + ini.get<string>("model", "Semi_truck.obj");
	// 	truck = new model_t();
	// 	if(!truck->load(model_file)) {
	// 		return false;
	// 	}
	// 	vector <material_t*> &vis_mats = truck->get_materials();
	// 	materials.insert(materials.begin(), vis_mats.begin(), vis_mats.end());
	// 	printf("Got materials\n");
	// }

	{
		truck = new Vehicle();
		{
			string model_file("./models/");
			ini.select("Truck");
			truck->body = new TruckPart(this
					,	model_file + ini.get<string>("model", "Semi_truck.obj")
					,	ini.get<float>("angle", 0.0f)
					,	ini.get<float>("size", 2.0f)
					,	ini.get<float>("posX", 2.0f)
					,	ini.get<float>("posY", 2.0f)
					,	str_to_vec3(ini.get<string>("translate", "0/0/0"))
					,	str_to_vec3(ini.get<string>("rotate", "0/0/0"))
					,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
					);
			vector <material_t*> &vis_mats = truck->body->get_materials();
			materials.insert(materials.begin(), vis_mats.begin(), vis_mats.end());
			printf("Got materials\n");
		}

		{
			ini.select("Truck");
			float posX = ini.get<float>("posX", 0.0f), posY = ini.get<float>("posY", 0.0f); 
			ini.select("TruckSteeringWheels");
			string model_file("./models/");
			truck->left_steering_wheel = new TruckPart(this
						,	model_file + ini.get<string>("model", "single_wheel.obj")
						,	ini.get<float>("leftS", 2.0f)
						,	ini.get<float>("angleL", 0.0f)
						,	posX + ini.get<float>("leftX", 2.0f)
						,	posY + ini.get<float>("leftY", 2.0f)
						,	str_to_vec3(ini.get<string>("translate", "0/0/0"))
						,	str_to_vec3(ini.get<string>("rotate", "0/0/0"))
						,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
						);
			vector <material_t*> &vis_mats = truck->left_steering_wheel->get_materials();
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
						,	ini.get<float>("angleR", 0.0f)
						,	posX + ini.get<float>("rightX", 2.0f)
						,	posY + ini.get<float>("rightY", 2.0f)
						,	str_to_vec3(ini.get<string>("translate", "0/0/0"))
						,	str_to_vec3(ini.get<string>("rotate", "0/0/0"))
						,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
						);
			vector <material_t*> &vis_mats = truck->right_steering_wheel->get_materials();
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
							,	ini.get<float>("rightS", 2.0f)
							,	ini.get<float>("angleL", 0.0f)
							,	posX + ini.get<float>(leftX, 2.0f)
							,	posY + ini.get<float>(leftY, 2.0f)
							,	str_to_vec3(ini.get<string>("translate", "0/0/0"))
							,	str_to_vec3(ini.get<string>("rotate", "0/0/0"))
							,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
							));
				printf("loading double wheels 2\n");
				vector <material_t*> &vis_mats = truck->left_wheels.back()->get_materials();
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
							,	ini.get<float>("angleR", 0.0f)
							,	posX + ini.get<float>(rightX, 2.0f)
							,	posY + ini.get<float>(rightY, 2.0f)
							,	str_to_vec3(ini.get<string>("translate", "0/0/0"))
							,	str_to_vec3(ini.get<string>("rotate", "0/0/0"))
							,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
							));
				printf("loading double wheels 2\n");
				vector <material_t*> &vis_mats = truck->right_wheels.back()->get_materials();
				printf("loading double wheels 3\n");
				materials.insert(materials.begin(), vis_mats.begin(), vis_mats.end());
				printf("Got materials\n");
			}
		}


		{
			first_trailer = new Vehicle();
			{
				string model_file("./models/");
				ini.select("Trailer1");
				printf("trailer1!!!!!!!\n");
				first_trailer->body = new TruckPart(this
						,	model_file + ini.get<string>("model", "Truck/Trailer_no_wheels.obj")
						,	ini.get<float>("size", 2.0f)
						,	ini.get<float>("angle", 0.0f)
						,	ini.get<float>("posX", 2.0f)
						,	ini.get<float>("posY", 2.0f)
						,	str_to_vec3(ini.get<string>("translate", "0/0/0"))
						,	str_to_vec3(ini.get<string>("rotate", "0/0/0"))
						,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
						);
				vector <material_t*> &vis_mats = first_trailer->body->get_materials();
				materials.insert(materials.begin(), vis_mats.begin(), vis_mats.end());
				printf("Got materials\n");
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
								,	ini.get<float>("rightS", 2.0f)
								,	ini.get<float>("angleL", 0.0f)
								,	posX + ini.get<float>(leftX, 2.0f)
								,	posY + ini.get<float>(leftY, 2.0f)
								,	str_to_vec3(ini.get<string>("translate", "0/0/0"))
								,	str_to_vec3(ini.get<string>("rotate", "0/0/0"))
								,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
								));
					printf("loading double wheels 2\n");
					vector <material_t*> &vis_mats = first_trailer->left_wheels.back()->get_materials();
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
								,	ini.get<float>("angleR", 0.0f)
								,	posX + ini.get<float>(rightX, 2.0f)
								,	posY + ini.get<float>(rightY, 2.0f)
								,	str_to_vec3(ini.get<string>("translate", "0/0/0"))
								,	str_to_vec3(ini.get<string>("rotate", "0/0/0"))
								,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
								));
					printf("loading double wheels 2\n");
					vector <material_t*> &vis_mats = first_trailer->right_wheels.back()->get_materials();
					printf("loading double wheels 3\n");
					materials.insert(materials.begin(), vis_mats.begin(), vis_mats.end());
					printf("Got materials\n");
				}
			}
		}

		{
			second_trailer = new Vehicle();
			{
				string model_file("./models/");
				ini.select("Trailer2");
				second_trailer->body = new TruckPart(this
						,	model_file + ini.get<string>("model", "Second_trailer_no_wheels.obj")
						,	ini.get<float>("size", 2.0f)
						,	ini.get<float>("angle", 0.0f)
						,	ini.get<float>("posX", 2.0f)
						,	ini.get<float>("posY", 2.0f)
						,	str_to_vec3(ini.get<string>("translate", "0/0/0"))
						,	str_to_vec3(ini.get<string>("rotate", "0/0/0"))
						,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
						);
				vector <material_t*> &vis_mats = second_trailer->body->get_materials();
				materials.insert(materials.begin(), vis_mats.begin(), vis_mats.end());
				printf("Got materials\n");
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
								,	ini.get<float>("rightS", 2.0f)
								,	ini.get<float>("angleL", 0.0f)
								,	posX + ini.get<float>(leftX, 2.0f)
								,	posY + ini.get<float>(leftY, 2.0f)
								,	str_to_vec3(ini.get<string>("translate", "0/0/0"))
								,	str_to_vec3(ini.get<string>("rotate", "0/0/0"))
								,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
								));
					printf("loading double wheels 2\n");
					vector <material_t*> &vis_mats = second_trailer->left_wheels.back()->get_materials();
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
								,	ini.get<float>("angleR", 0.0f)
								,	posX + ini.get<float>(rightX, 2.0f)
								,	posY + ini.get<float>(rightY, 2.0f)
								,	str_to_vec3(ini.get<string>("translate", "0/0/0"))
								,	str_to_vec3(ini.get<string>("rotate", "0/0/0"))
								,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
								));
					printf("loading double wheels 2\n");
					vector <material_t*> &vis_mats = second_trailer->right_wheels.back()->get_materials();
					printf("loading double wheels 3\n");
					materials.insert(materials.begin(), vis_mats.begin(), vis_mats.end());
					printf("Got materials\n");
				}
			}
		}
		truck->following_vehicle = first_trailer;
		first_trailer-> following_vehicle = second_trailer;
		/*--------------------------------------------------------------------------------*/
		// {
		// 	ini.select("TruckDoubleWheels");
		// 	model_file = "./models/";
		// 	truck->right_steering_wheel = new TruckPart(this
		// 				,	model_file + ini.get<string>("model", "single_wheel.obj")
		// 				,	180.0f
		// 				,	ini.get<float>("leftS", 2.0f)
		// 				,	ini.get<float>("leftX", 2.0f)
		// 				,	ini.get<float>("leftY", 2.0f)
		// 				,	str_to_vec3(ini.get<string>("translate", "0/0/0"))
		// 				,	str_to_vec3(ini.get<string>("rotate", "0/0/0"))
		// 				,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
		// 				);
		// 	vector <material_t*> &vis_mats = truck->body->get_materials();
		// 	materials.insert(materials.begin(), vis_mats.begin(), vis_mats.end());
		// 	printf("Got materials\n");
		// }


		// for(ini_t::sectionsit_t i = ini.sections.begin(); i != ini.sections.end(); ++i) {
		// 	if (!strncmp(i->first.c_str(), "Truck", strlen("Truck"))) {
		// 		ini.select(i->first.c_str());
		// 		string model_file("./models/");


		// 		truck.push_back(new TruckPart(this
		// 			,	model_file + ini.get<string>("model", "Semi_truck.obj")
		// 			,	ini.get<float>("size", 2.0f)
		// 			,	ini.get<float>("posX", 2.0f)
		// 			,	ini.get<float>("posY", 2.0f)
		// 			,	str_to_vec3(ini.get<string>("translate", "0/0/0"))
		// 			,	str_to_vec3(ini.get<string>("rotate", "0/0/0"))
		// 			,	str_to_vec3(ini.get<string>("scale", "1/1/1"))
		// 			)
		// 		);
				
		// 		vector <material_t*> &vis_mats = truck.back()->get_materials();
		// 		materials.insert(materials.begin(), vis_mats.begin(), vis_mats.end());
		// 		printf("Got materials\n");
		// 	}
		// }

	}
	
	
	{
		int op;
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &op);
		max_textures = op;
	}
	
	
	glGenTextures(1, &shadow_map);
	glBindTexture(GL_TEXTURE_2D, shadow_map);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow_map_size, shadow_map_size, 0,
	GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    return true;
}

world_t::~world_t() {	
	delete galery;
	delete player;
}

void world_t::draw() {
	glm::mat4 V = player->get_view_matrix();
	
	/**
	 * Recalculate models
	 */
	
	for (unsigned i = 0, ilen = materials.size(); i < ilen; ++i) {
		materials[i]->set_marker(false);
	}
	galery->set_mv_matrix(glm::mat4(1.0f));
	// glm::mat4(
	// 	glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
	// 	glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
	// 	glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
	// 	glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	// 	glm::vec4(0.0f, 0.5f, 0.0f, 0.0f),
	// 	glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
	// 	glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)
	// ));

    //printf("mv matrix set\n");
	galery->set_mv_matrix(glm::mat4(1.0f));

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
	
	if (shadows_working) {
		glDisable(GL_LIGHT0);
		//draw_with_shadows(V);
	} else {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glDisable(GL_CULL_FACE);
		draw_in_material_order(V);
	}
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

// void world_t::draw_with_shadows (glm::mat4 V) {
// 	/**
// 	 * First draw - from all lights into shadow map
// 	 */
	
// 	glCullFace(GL_FRONT);
// 	glColorMask(0, 0, 0, 0);

// 	glMatrixMode(GL_PROJECTION);
// 	glMatrixMode(GL_MODELVIEW);
// 	glViewport(0, 0, shadow_map_size, shadow_map_size);
	
// 	glBindTexture(GL_TEXTURE_2D, shadow_map);
// 	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, shadow_map_size, shadow_map_size);
	
// 	glCullFace(GL_BACK);
// 	glShadeModel(GL_SMOOTH);
// 	glColorMask(1, 1, 1, 1);
	
// 	/**
// 	 * Second draw
// 	 */
// 	glClear(GL_DEPTH_BUFFER_BIT);
// 	glMatrixMode(GL_PROJECTION);
// 	glLoadMatrixf(glm::value_ptr(P));
	
// 	glViewport(0, 0, screen_w, screen_h);
// 	glEnable(GL_LIGHTING);
	
// 	draw_in_material_order(V);
	
// 	/**
// 	 * Third draw
// 	 */
	
// 	glEnable(GL_TEXTURE_GEN_S);
// 	glEnable(GL_TEXTURE_GEN_T);
// 	glEnable(GL_TEXTURE_GEN_R);
// 	glEnable(GL_TEXTURE_GEN_Q);
// 	glBindTexture(GL_TEXTURE_2D, shadow_map);
// 	glEnable(GL_TEXTURE_2D);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
// 	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);
// 	glAlphaFunc(GL_GEQUAL, 0.99f);
// 	glEnable(GL_ALPHA_TEST);
	
// 	glDisable(GL_CULL_FACE);
	
// 		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
// 		glTexGenfv(GL_S, GL_EYE_PLANE, glm::value_ptr(texture_matrix[0]));

// 		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
// 		glTexGenfv(GL_T, GL_EYE_PLANE, glm::value_ptr(texture_matrix[1]));

// 		glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
// 		glTexGenfv(GL_R, GL_EYE_PLANE, glm::value_ptr(texture_matrix[2]));

// 		glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
// 		glTexGenfv(GL_Q, GL_EYE_PLANE, glm::value_ptr(texture_matrix[3]));

// 		draw_in_material_order(V);
// 	glDisable(GL_TEXTURE_GEN_S);
// 	glDisable(GL_TEXTURE_GEN_T);
// 	glDisable(GL_TEXTURE_GEN_R);
// 	glDisable(GL_TEXTURE_GEN_Q);

// 	//Restore other states
// 	glDisable(GL_LIGHTING);
// 	glDisable(GL_TEXTURE_2D);
// 	glDisable(GL_ALPHA_TEST);
// }

void world_t::draw_in_material_order(glm::mat4 V) {
	glMatrixMode(GL_MODELVIEW);
	
	for (unsigned i = 0, ilen = materials.size(); i < ilen; ++i) {
		materials[i]->draw_associated_meshes(V);
	}
}


void world_t::next_frame (direct_t keys_h, direct_t keys_v, direct_t height, direct_t vehicle, direct_t v_turn) {
	player->move(keys_h, keys_v, height);
	truck->move(vehicle);
}

void world_t::mouse_motion(float dang_h, float dang_v) {
	if (invert_mouse_y) {
		dang_v *= -1;
	}
	player->mouse_motion(dang_h * mouse_sensitivity_x / 300, dang_v * mouse_sensitivity_y / 300);
}


bool world_t::test_colls_with_galery(vertex_2d pos, vertex_2d itd, float size, float height) {
	float scale = module(pos, itd);
	scale = (scale + size) / size;
	
	vertex_3d pos0(pos.x, height * 0.8, pos.y)
		,	pos1(pos.x + scale * (itd.x - pos.x), height * 0.8, pos.y + scale * (itd.y - pos.y));
	
	return !(abs(pos1.z) >= 37 || abs(pos1.x) >= 37
		|| (abs(pos1.x) <= 3 && abs(20 - abs(pos1.z)) >= 2)
		|| (abs(pos1.z - 1) <= 3 && abs(20 - abs(pos1.x)) >= 2)
		);
	return !galery->test_intersection(pos0, pos1);
}
