#include "concrete_command.h"
#include "Object/s_timer.h"
#include "s_math.h"
#include <cstdlib>.
#include <algorithm>



void generate_monster_command::execute()
{
	assert(sence_ptr);

	auto time_ptr = s_timer::get_instance();
	if (time_ptr->total_time() > sence_ptr->prev_monster_generate_time + sence_ptr->monster_cooler_time)
	{
		sence_ptr->prev_monster_generate_time = time_ptr->total_time();
		
		//random position
		s_monster* new_monster_ptr = new s_monster();
		//left
		int generate_dir = rand() % 4;
		if (generate_dir == 0)
		{
			new_monster_ptr->position[0] = rand() % (sence_ptr->map_width *2) - sence_ptr->map_width;
			new_monster_ptr->position[1] = 2;
			new_monster_ptr->position[2] = -sence_ptr->map_length;
		}//right
		else if (generate_dir == 1)
		{
			new_monster_ptr->position[0] = rand() % (sence_ptr->map_width * 2) - sence_ptr->map_width;
			new_monster_ptr->position[1] = 2;
			new_monster_ptr->position[2] = sence_ptr->map_length;
		}//top
		else if (generate_dir == 2)
		{
			new_monster_ptr->position[0] = sence_ptr->map_width;
			new_monster_ptr->position[1] = 2;
			new_monster_ptr->position[2] = rand() % (sence_ptr->map_length *2) - sence_ptr->map_length;
		}//buttom
		else
		{
			new_monster_ptr->position[0] = -sence_ptr->map_width;
			new_monster_ptr->position[1] = 2;
			new_monster_ptr->position[2] = rand() % (sence_ptr->map_length * 2) - sence_ptr->map_length;
		}
		new_monster_ptr->srearch_object = sence_ptr->player;
		new_monster_ptr->monster_behavior_tree_cmd = new monster_behavior_tree_command(new_monster_ptr, sence_ptr);
		//add monster in collision tree map
		sence_ptr->get_monster_map_set(new_monster_ptr->position[0], new_monster_ptr->position[2]).insert(new_monster_ptr);
		sence_ptr->monster_group.insert(new_monster_ptr);
	}
}

void player_control_command::execute()
{
	assert(player_ptr);
	assert(sence_ptr);
	assert(sence_ptr->camera_ptr);
	auto time_ptr = s_timer::get_instance();
	if (KeyW.pressed())
	{
		player_ptr->position[2] += player_ptr->speed * time_ptr->detail_time();
	}
	if (KeyS.pressed())
	{
		player_ptr->position[2] -= player_ptr->speed * time_ptr->detail_time();
	}
	if (KeyA.pressed())
	{
		player_ptr->position[0] -= player_ptr->speed * time_ptr->detail_time();
	}
	if (KeyD.down())
	{
		player_ptr->position[0] += player_ptr->speed * time_ptr->detail_time();
	}

	if (MouseL.pressed())
	{
		//add atk text
		auto screen_pos = sence_ptr->camera_ptr->worldToScreenPoint({ player_ptr->position[0] ,player_ptr->position[1] ,player_ptr->position[2] });
		s_text text_atk;
		text_atk.text = "gengengengengen";
		add_text_command add_text_cmd(sence_ptr, text_atk, time_ptr->total_time(), screen_pos.x, screen_pos.y);
		add_text_cmd.execute();

		if (player_ptr->generate_bullet_cmd) player_ptr->generate_bullet_cmd->execute();
	}

	//stay in map
	player_ptr->position[0] = std::min(player_ptr->position[0], (double)sence_ptr->map_width);
	player_ptr->position[0] = std::max(player_ptr->position[0], -(double)sence_ptr->map_width);
	player_ptr->position[2] = std::min(player_ptr->position[2], (double)sence_ptr->map_length);
	player_ptr->position[2] = std::max(player_ptr->position[2], -(double)sence_ptr->map_length);
}

void monster_behavior_tree_command::execute()
{
	assert(monster_ptr);
	assert(sence_ptr);
	monster_move_command monster_move_cmd(monster_ptr, sence_ptr);

	monster_move_cmd.execute();
}

void monster_move_command::execute()
{
	assert(sence_ptr);
	assert(monster_ptr);
	assert(monster_ptr->srearch_object);
	auto time_ptr = s_timer::get_instance();

	//if (distance_cal(monster_ptr->position, monster_ptr->srearch_object->position) < monster_ptr->search_radius)
	//{
	//	direction_cal(monster_ptr->position, monster_ptr->srearch_object->position, monster_ptr->move_dir[0], monster_ptr->move_dir[1], monster_ptr->move_dir[2]);
	//}
	//else if(time_ptr->total_time() > monster_ptr->perv_move_update_time + monster_ptr->standby_time)
	//{
	//	monster_ptr->perv_move_update_time = time_ptr->total_time();
	//	double rand_vector[3];
	//	rand_vector[0] = (double)(rand() % 1000) / 1000;
	//	rand_vector[1] = 0;
	//	rand_vector[2] = (double)(rand() % 1000) / 1000;
	//	direction_cal(monster_ptr->position,
	//		rand_vector, monster_ptr->move_dir[0], monster_ptr->move_dir[1], monster_ptr->move_dir[2]);
	//}
	
	//player direction
	double move_dir[3] = { 0 };
	direction_cal(monster_ptr->position, monster_ptr->srearch_object->position, move_dir[0], move_dir[1], move_dir[2]);
	//renew monster postion in collision tree map
	sence_ptr->get_monster_map_set(monster_ptr->position[0], monster_ptr->position[2]).erase(monster_ptr);

	monster_ptr->position[0] += move_dir[0] * monster_ptr->speed * time_ptr->detail_time();
	monster_ptr->position[2] += move_dir[2] * monster_ptr->speed * time_ptr->detail_time();

	//renew monster postion in collision tree map
	sence_ptr->get_monster_map_set(monster_ptr->position[0], monster_ptr->position[2]).insert(monster_ptr);
}

void generate_bullet_command::execute()
{
	assert(sence_ptr);
	assert(camera_ptr);

	Ray mouse_ray = camera_ptr->screenToRay(Cursor::Pos());

	auto result = mouse_ray.intersectsAt(Plane(0, 2, 0, 1024));
	Float3 bullet_dir = result.value();
	Float3 player_pos{ sence_ptr->player->position[0],sence_ptr->player->position[1] ,sence_ptr->player->position[2] };
	bullet_dir = bullet_dir - player_pos;
	bullet_dir.normalize();
	////wrong
	//Sphere(result.value(), 2).draw(ColorF{ 1.0, 1.0, 1.0 }.removeSRGBCurve());
	//Float3 debug{ game_sence->player->position[0],game_sence->player->position[1],game_sence->player->position[2] };
	//debug = camera_3d.getViewProj().transformPoint(debug);
	//debug = camera_3d.getInvViewProj().transformPoint(debug);
	//Float3 screen_mouse{ (float)Cursor::Pos().x / 1280,(float)Cursor::Pos().y / 720, 1 };
	//Float3 world_mouse = camera_3d.getInvViewProj().transformPoint(screen_mouse);
	//Float3 world_mouse_direction = world_mouse - camera_3d.getEyePosition();
	//world_mouse_direction.normalize();
	//world_mouse_direction *= camera_3d.getEyePosition().y - 2;
	//world_mouse_direction = world_mouse;
	//Sphere(world_mouse_direction, 2).draw(ColorF{ 1.0, 1.0, 1.0 }.removeSRGBCurve());

	s_bullet* new_bullet_ptr = new s_bullet();
	new_bullet_ptr->move_dir[0] = bullet_dir.x;
	new_bullet_ptr->move_dir[1] = bullet_dir.y;
	new_bullet_ptr->move_dir[2] = bullet_dir.z;

	new_bullet_ptr->position[0] = sence_ptr->player->position[0];
	new_bullet_ptr->position[1] = sence_ptr->player->position[1];
	new_bullet_ptr->position[2] = sence_ptr->player->position[2];

	new_bullet_ptr->bullet_behavior_tree_cmd = new bullet_behavior_tree_command(new_bullet_ptr,sence_ptr);

	sence_ptr->bullet_group.insert(new_bullet_ptr);

}

void bullet_behavior_tree_command::execute()
{
	assert(sence_ptr);
	assert(bullet_ptr);

	bullet_move_command bullet_move_cmd(bullet_ptr, sence_ptr);
	bullet_move_cmd.execute();

}

void bullet_move_command::execute()
{
	assert(sence_ptr);
	assert(bullet_ptr);

	auto time_ptr = s_timer::get_instance();

	bullet_ptr->position[0] += bullet_ptr->move_dir[0] * bullet_ptr->speed * time_ptr->detail_time();
	bullet_ptr->position[2] += bullet_ptr->move_dir[2] * bullet_ptr->speed * time_ptr->detail_time();

}

void check_collision_command::execute()
{
	assert(sence_ptr);

	auto time_ptr = s_timer::get_instance();

	//bullet move & collision
	for (auto it = sence_ptr->bullet_group.begin();it != sence_ptr->bullet_group.end();)
	{
		if ((*it)->bullet_behavior_tree_cmd) (*it)->bullet_behavior_tree_cmd->execute();

		//monster map bullet collision
		auto & monster_set = sence_ptr->get_monster_map_set((*it)->position[0], (*it)->position[2]);
		bool is_hit = false;
		for (auto itt = monster_set.begin(); itt != monster_set.end();)
		{
			s_monster* monster_ptr = *itt;
			if ((*it)->mesh.intersects(monster_ptr->mesh))
			{
				//add atk text
				auto screen_pos = sence_ptr->camera_ptr->worldToScreenPoint({ monster_ptr->position[0] ,monster_ptr->position[1] ,monster_ptr->position[2]});
				s_text text_atk;
				text_atk.text = "E!~A!";
				add_text_command add_text_cmd(sence_ptr, text_atk, time_ptr->total_time(), screen_pos.x, screen_pos.y);
				add_text_cmd.execute();

				sence_ptr->monster_group.erase(monster_ptr);
				itt = monster_set.erase(itt);
				delete monster_ptr;
				is_hit = true;
			}
			else
			{
				itt++;
			}
		}

		//销毁
		if (is_hit
		|| (*it)->position[0] > sence_ptr->map_width
		|| (*it)->position[0] < -sence_ptr->map_width
		|| (*it)->position[2] > sence_ptr->map_length
		|| (*it)->position[2] < -sence_ptr->map_length)
		{
			s_bullet* bullet_ptr = *it;
			//add atk text
			auto screen_pos = sence_ptr->camera_ptr->worldToScreenPoint({ bullet_ptr->position[0] ,bullet_ptr->position[1] ,bullet_ptr->position[2] });
			s_text text_atk;
			text_atk.text = "BOOM";
			text_atk.size = 60;
			add_text_command add_text_cmd(sence_ptr, text_atk, time_ptr->total_time(), screen_pos.x, screen_pos.y);
			add_text_cmd.execute();

			delete bullet_ptr;
			it = sence_ptr->bullet_group.erase(it);
		}
		else
		{
			it++;
		}
	}

	//monster map player collision
	auto& monster_set = sence_ptr->get_monster_map_set(sence_ptr->player->position[0], sence_ptr->player->position[2]);
	for (auto itt = monster_set.begin(); itt != monster_set.end();)
	{
		s_monster* monster_ptr = *itt;
		if (sence_ptr->player->mesh.intersects((*itt)->mesh))
		{
			//add atk text
			auto screen_pos = sence_ptr->camera_ptr->worldToScreenPoint({ monster_ptr->position[0] ,monster_ptr->position[1] ,monster_ptr->position[2] });
			s_text text_atk;
			text_atk.text = "HHHHHHHHH";
			text_atk.size = 60;
			add_text_command add_text_cmd(sence_ptr, text_atk, time_ptr->total_time(), screen_pos.x, screen_pos.y);
			add_text_cmd.execute();

			sence_ptr->monster_group.erase(monster_ptr);
			itt = monster_set.erase(itt);

			delete monster_ptr;

			sence_ptr->player->hp--;
		}
		else
		{
			itt++;
		}
	}
}


void add_text_command::execute()
{
	assert(sence_ptr);
	
	text_refer.screen_position[0] = screen_x;
	text_refer.screen_position[1] = screen_y;
	text_refer.start_time = start_time;

	sence_ptr->text_group.push_back(text_refer);
}

void refresh_text_command::execute()
{
	assert(sence_ptr);

	auto time_ptr = s_timer::get_instance();

	for(auto it = sence_ptr->text_group.begin(); it != sence_ptr->text_group.end();)
	{
		if ((*it).start_time + (*it).stay_time < time_ptr->total_time())
		{
			it = sence_ptr->text_group.erase(it);
			continue;
		}

		(*it).screen_position[1] -= (*it).speed * time_ptr->detail_time();
		auto font_text = SimpleGUI::GetFont()(U"aaaaaaaaaaaaaaaaaaaaa").draw((*it).screen_position[0], (*it).screen_position[1], ColorF{ 0.11 });
		it++;
	}
}
