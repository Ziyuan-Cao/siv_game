#pragma once
#include "s_object.h"
#include "s_factory.h"
#include "Command/s_command.h"
#include <vector>
#include <set>

using std::vector;
using std::set;

struct s_sence
{
	float prev_monster_generate_time = 0;
	float monster_cooler_time = 1;
	int map_width = 30;
	int map_length = 30;

	set<s_monster*> monster_group;
	set<s_bullet*> bullet_group;
	set<s_item*> item_group;
	s_player* player;
	s_map* level_map;
	BasicCamera3D* camera_ptr = nullptr;

	vector<vector<set<s_monster*>>> monster_hash_map;
	vector<s_text> text_group;
	s_command* generate_monster_cmd = nullptr;
	s_command* generate_item_cmd = nullptr;

	s_sence()
	{
		monster_hash_map.assign(map_width*2 +1, vector<set<s_monster*>>(map_length*2 +1, set<s_monster*>()));

		player = s_factory::get_instance()->create_player();

		level_map = new s_map();
		player->position[1] = 2;
		level_map->scale = map_width * 2 +4;
	}

	~s_sence()
	{
		for (auto it : monster_group)
		{
			if (it)delete it;
		}
		for (auto it : bullet_group)
		{
			if (it)delete it;
		}
		for (auto it : item_group)
		{
			if (it)delete it;
		}
		if (player)delete player;
		if (level_map)delete level_map;
		if (generate_monster_cmd)delete generate_monster_cmd;
		if (generate_item_cmd)delete generate_item_cmd;
		player = nullptr;
		level_map = nullptr;
		generate_monster_cmd = nullptr;
		generate_item_cmd = nullptr;
	}

	set<s_monster*>& get_monster_map_set(double in_x,double in_z)
	{
		int map_x_index = in_x + map_width;
		int map_z_index = in_z + map_length;
		return monster_hash_map[map_x_index][map_z_index];
	}
};
