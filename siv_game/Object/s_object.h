#pragma once
#include "../Command/s_command.h"
#include  <string>
# include <Siv3D.hpp>
template<typename t_mesh_type>
struct s_object
{
	double position[3] = { 0,0,0 };
	double rotation[3] = { 0,0,0 };
	double scale = 1;
	double color[3] = { 0.6,0.6,0.6 };
	t_mesh_type mesh;

	void update_state();

	void draw()
	{
		//insert into sence queue?
		mesh.draw(ColorF{ color[0], color[1], color[2] }.removeSRGBCurve());
	}
};
struct s_monster : public s_object<Box>
{
	int hp = 10;
	int speed = 1;
	int search_radius = 10;
	float standby_time = 3; // standby time

	s_object<Sphere>* srearch_object = nullptr;
	s_monster()
	{
		scale = 2;
		color[0] = 0.8;
		color[1] = 0.6;
		color[2] = 0.4;
	}

	~s_monster()
	{
		if(monster_behavior_tree_cmd)delete(monster_behavior_tree_cmd);
		monster_behavior_tree_cmd = nullptr;
	}

	s_command* monster_behavior_tree_cmd = nullptr;
};

struct s_player : public s_object<Sphere>
{
	int hp = 10;
	int speed = 10;
	int bullet_number = 10;
	s_player()
	{
		color[0] = 0.4;
		color[1] = 0.8;
		color[2] = 0.6;
	}

	~s_player()
	{
		if (player_control_cmd)delete(player_control_cmd);
		player_control_cmd = nullptr;
	}

	s_command* player_control_cmd = nullptr;
	s_command* generate_bullet_cmd = nullptr;
};

struct s_bullet : public s_object<Sphere>
{
	int speed = 20;
	double move_dir[3] = { 0,0,0 };
	s_bullet()
	{
		scale = 0.3;
		color[0] = 0.0;
		color[1] = 0.0;
		color[2] = 0.6;
	}

	~s_bullet()
	{
		if (bullet_behavior_tree_cmd) delete bullet_behavior_tree_cmd;
		bullet_behavior_tree_cmd = nullptr;
	}

	s_command* bullet_behavior_tree_cmd = nullptr;
};

struct s_item : public s_object<Box>
{
	int time = 10;
	s_item()
	{
		color[0] = 0.9;
		color[1] = 0.1;
		color[2] = 0.1;
	}
};

struct s_map : public s_object<Plane>
{
	s_map()
	{
		color[0] = 0.7;
		color[1] = 0.7;
		color[2] = 0.7;
	}
};

struct s_text
{
	double screen_position[2] = {0};
	float stay_time = 1;
	int speed = 10;
	float start_time = 0;
	int size = 24;
	std::string text;
};
