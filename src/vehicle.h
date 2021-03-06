#ifndef VEHICLE_H
#define VEHICLE_H

#include "truck_part.h"
#include "rectangle.h"
#include "obstacle.h"
#include <vector>

class Obstacle;

class Vehicle : public Rectangle {
private:
    float max_acceleration;
    float max_velocity;
    float max_reverse_velocity;

    float turn_acceleration;
    float min_turn;
    float max_turn;

    float begining_turn_point;
    float max_turn_point;
    float final_turn_point;

    float current_max_following_bend;
    float max_following_bend;
    float time_following_bend;
    // int max_wheel_rotating_time;
    // float max_angle;
    float acceleration(direct_t front_back, float velocity);
    float turn_factor();
    float bend_acceleration(direct_t right_left, float in_following_bend, float in_velocity);

public:
  vector <TruckPart*> left_wheels;
  vector <TruckPart*> right_wheels;
  TruckPart* left_steering_wheel;
  TruckPart* right_steering_wheel;
  TruckPart* body;
  Vehicle* following_vehicle;
  vector <Vertex2D<float> > dimensions;

    float velocity;
    float angle;
    float size;
    float following_bend;
    Vertex2D<float>position;
    int last_time;

	
	Vehicle(float in_size,
    float in_max_acceleration,
    float in_max_velocity,
    float in_max_reverse_velocity,
    float in_turn_acceleration,
    float in_min_turn,
    float in_max_turn,
    float in_begining_turn_point,
    float in_max_turn_point,
    float in_final_turn_point,
    float in_max_following_bend,
    float in_time_following_bend
    );
 
	~Vehicle();


    void calculate(direct_t front_back, direct_t right_left, vector <Obstacle*> &obstacles);
    bool move(float parent_size, Vertex2D<float>in_position, float in_angle, float ds, float in_following_bend, vector <Obstacle*> &obstacles);
    vector <Vertex2D<float> > get_body_vertices();
    void set_vertices();
	bool detect_collision(vector <Obstacle*> &obstacles);
    void collision();
    // void set_mv_matrix(glm::mat4 V);
	// void apply_mv();
};

#endif