#include "concrete_command.h"
#include "Object/s_timer.h"
#include "Object/s_factory.h"
#include "s_math.h"
#include <cstdlib>.
#include <algorithm>

void generate_monster_command::execute()
{
	//check ptr
	assert(sence_ptr);

	//rand
	static uint64 rand_seed = 0;
	rand_seed++;
	srand((s_timer::get_instance()->total_time()*1000) + rand_seed);

	auto time_ptr = s_timer::get_instance();

	sence_ptr->prev_monster_generate_time = time_ptr->total_time();
		
	//random position
	int monster_type = rand() % MONSTER_TYPE_NUMBER;
	s_monster* monster_ptr = s_factory::get_instance()->create_monster((MONSTER_TYPE)monster_type);
	//left
	int generate_dir = rand() % 4;
	if (generate_dir == 0)
	{
		monster_ptr->position[0] = rand() % (sence_ptr->map_width *2) - sence_ptr->map_width;
		monster_ptr->position[1] = 2;
		monster_ptr->position[2] = -sence_ptr->map_length;
	}//right
	else if (generate_dir == 1)
	{
		monster_ptr->position[0] = rand() % (sence_ptr->map_width * 2) - sence_ptr->map_width;
		monster_ptr->position[1] = 2;
		monster_ptr->position[2] = sence_ptr->map_length;
	}//top
	else if (generate_dir == 2)
	{
		monster_ptr->position[0] = sence_ptr->map_width;
		monster_ptr->position[1] = 2;
		monster_ptr->position[2] = rand() % (sence_ptr->map_length *2) - sence_ptr->map_length;
	}//buttom
	else
	{
		monster_ptr->position[0] = -sence_ptr->map_width;
		monster_ptr->position[1] = 2;
		monster_ptr->position[2] = rand() % (sence_ptr->map_length * 2) - sence_ptr->map_length;
	}
	monster_ptr->srearch_object = sence_ptr->player;
	monster_ptr->monster_behavior_tree_cmd = new monster_behavior_tree_command(monster_ptr, sence_ptr);
	//add monster in collision tree map
	monster_ptr->update_state();
	sence_ptr->get_monster_map_set(monster_ptr->position[0], monster_ptr->position[2]).insert(monster_ptr);
	sence_ptr->monster_group.insert(monster_ptr);

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

	//update monster postion in collision tree map
	sence_ptr->get_monster_map_set(monster_ptr->position[0], monster_ptr->position[2]).erase(monster_ptr);

	monster_ptr->position[0] += move_dir[0] * monster_ptr->speed * time_ptr->detail_time();
	monster_ptr->position[2] += move_dir[2] * monster_ptr->speed * time_ptr->detail_time();

	//update monster postion in collision tree map
	sence_ptr->get_monster_map_set(monster_ptr->position[0], monster_ptr->position[2]).insert(monster_ptr);
}

void player_control_command::execute()
{
	assert(player_ptr);
	assert(sence_ptr);
	assert(sence_ptr->camera_ptr);

	auto time_ptr = s_timer::get_instance();

	//keyboard control
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
	if (KeyD.pressed())
	{
		player_ptr->position[0] += player_ptr->speed * time_ptr->detail_time();
	}

	//attack control
	player_attack_command player_attack_cmd(player_ptr, sence_ptr);
	player_attack_cmd.execute();
	
	//stay in map
	player_ptr->position[0] = std::min(player_ptr->position[0], (double)sence_ptr->map_width);
	player_ptr->position[0] = std::max(player_ptr->position[0], -(double)sence_ptr->map_width);
	player_ptr->position[2] = std::min(player_ptr->position[2], (double)sence_ptr->map_length);
	player_ptr->position[2] = std::max(player_ptr->position[2], -(double)sence_ptr->map_length);
}

void player_attack_command::execute()
{
	assert(player_ptr);
	assert(sence_ptr);
	assert(sence_ptr->camera_ptr);

	static int attack_time = 0;
	static float count_time_start = 0;
	const float count_time_range = 1;
	static bool hard_atteck = false;

	auto time_ptr = s_timer::get_instance();

	bool is_shooting = false;

	//left click attack
	if (MouseL.down() && player_ptr->bullet_number > 0)
	{
		player_ptr->bullet_number--;

		Ray mouse_ray = sence_ptr->camera_ptr->screenToRay(Cursor::Pos());

		auto result = mouse_ray.intersectsAt(Plane(0, 2, 0, 1024));
		Float3 bullet_dir = result.value();
		Float3 player_pos{ sence_ptr->player->position[0],sence_ptr->player->position[1] ,sence_ptr->player->position[2] };
		bullet_dir = bullet_dir - player_pos;
		bullet_dir.normalize();

		generate_bullet_command(sence_ptr, BULLET_TYPE_NORMAL, bullet_dir,player_pos).execute();

		is_shooting = true;
	}

	//special attack
	if (player_ptr->special_bullet_remain_time > 0)
	{
		player_ptr->special_bullet_remain_time -= time_ptr->detail_time();

		switch (player_ptr->current_bullet_type)
		{
		case BULLET_TYPE_UNLIMITED:
		{
			Ray mouse_ray = sence_ptr->camera_ptr->screenToRay(Cursor::Pos());

			auto result = mouse_ray.intersectsAt(Plane(0, 2, 0, 1024));
			Float3 bullet_dir = result.value();
			Float3 player_pos{ sence_ptr->player->position[0],sence_ptr->player->position[1] ,sence_ptr->player->position[2] };
			bullet_dir = bullet_dir - player_pos;
			bullet_dir.normalize();

			generate_bullet_command(sence_ptr, BULLET_TYPE_UNLIMITED, bullet_dir, player_pos).execute();

		}
		break;
		case BULLET_TYPE_FIREWORKS:
		{
			static float prev_fireworks_time = 0;
			const float cool_time = 0.3;
			const int fireworks_bullet_number = 8; // 1/4 circle
			if(prev_fireworks_time + cool_time < time_ptr->total_time())
			{
				prev_fireworks_time = time_ptr->total_time();

				for (int i = 0; i < fireworks_bullet_number; i++)
				{
					Float3 bullet_dir = { 0,0,0 };
					Float3 player_pos{ sence_ptr->player->position[0],sence_ptr->player->position[1] ,sence_ptr->player->position[2] };
					bullet_dir.x = i - fireworks_bullet_number; //-1~0
					bullet_dir.z = i; // 0~1
					bullet_dir.normalize();
					generate_bullet_command(sence_ptr, BULLET_TYPE_FIREWORKS, bullet_dir, player_pos).execute();

				}
				for (int i = 0; i < fireworks_bullet_number; i++)
				{
					Float3 bullet_dir = { 0,0,0 };
					Float3 player_pos{ sence_ptr->player->position[0],sence_ptr->player->position[1] ,sence_ptr->player->position[2] };
					bullet_dir.x = i; //0~1
					bullet_dir.z = fireworks_bullet_number - i; //1~0
					bullet_dir.normalize();
					generate_bullet_command(sence_ptr, BULLET_TYPE_FIREWORKS, bullet_dir, player_pos).execute();
				}
				for (int i = 0; i < fireworks_bullet_number; i++)
				{
					Float3 bullet_dir = { 0,0,0 };
					Float3 player_pos{ sence_ptr->player->position[0],sence_ptr->player->position[1] ,sence_ptr->player->position[2] };
					bullet_dir.x = fireworks_bullet_number - i; //1~0
					bullet_dir.z = -i;//0~-1
					bullet_dir.normalize();
					generate_bullet_command(sence_ptr, BULLET_TYPE_FIREWORKS, bullet_dir, player_pos).execute();
				}
				for (int i = 0; i < fireworks_bullet_number; i++)
				{
					Float3 bullet_dir = { 0,0,0 };
					Float3 player_pos{ sence_ptr->player->position[0],sence_ptr->player->position[1] ,sence_ptr->player->position[2] };
					bullet_dir.x = -i; //0~-1
					bullet_dir.z = i - fireworks_bullet_number;//-1~0
					bullet_dir.normalize();
					generate_bullet_command(sence_ptr, BULLET_TYPE_FIREWORKS, bullet_dir, player_pos).execute();
				}
			}
		}
		break;
		case BULLET_TYPE_SHOTGUN:
		{
			const float shotgun_angle = 30;
			const int shotgun_bullet_number = 10;
			if (MouseL.down())
			{
				Ray mouse_ray = sence_ptr->camera_ptr->screenToRay(Cursor::Pos());

				auto result = mouse_ray.intersectsAt(Plane(0, 2, 0, 1024));
				Float3 bullet_dir = result.value();
				Float3 player_pos{ sence_ptr->player->position[0],sence_ptr->player->position[1] ,sence_ptr->player->position[2] };
				bullet_dir = bullet_dir - player_pos;
				bullet_dir.normalize();

				for (int i = 0; i < shotgun_bullet_number; i++)
				{
					float current_angle = shotgun_angle / i;
					Mat4x4::Rotate({ 0,1,0 }, 30_deg / i, { 0,0,0 }).transformPoint(bullet_dir);
					bullet_dir.normalize();
					generate_bullet_command(sence_ptr, BULLET_TYPE_UNLIMITED, bullet_dir, player_pos).execute();

				}
				for (int i = 0; i < shotgun_bullet_number; i++)
				{
					float current_angle = shotgun_angle / i;
					Mat4x4::Rotate({ 0,1,0 }, -30_deg / i, { 0,0,0 }).transformPoint(bullet_dir);
					bullet_dir.normalize();
					generate_bullet_command(sence_ptr, BULLET_TYPE_UNLIMITED, bullet_dir, player_pos).execute();

				}

			}
		}
		break;
		}
		is_shooting = true;
	}

	//hard attack count
	if (is_shooting)
	{
		count_time_start = time_ptr->total_time();
		attack_time++;
		if (attack_time > 4)
		{
			hard_atteck = true;
		}
	}
	else if(count_time_start + count_time_range < time_ptr->total_time())
	{
		hard_atteck = false;
		attack_time = 0;
	}

	//hard attack
	if (hard_atteck)
	{
		Ray mouse_ray = sence_ptr->camera_ptr->screenToRay(Cursor::Pos());

		auto result = mouse_ray.intersectsAt(Plane(0, 2, 0, 1024));
		Float3 bullet_dir = result.value();
		Float3 player_pos{ sence_ptr->player->position[0],sence_ptr->player->position[1] ,sence_ptr->player->position[2] };
		bullet_dir = bullet_dir - player_pos;
		bullet_dir.normalize();

		generate_bullet_command(sence_ptr, BULLET_TYPE_HARD_ATTACK, bullet_dir, player_pos).execute();

	}

	//generate text
	if (is_shooting)
	{
		//add atk text
		auto screen_pos = sence_ptr->camera_ptr->worldToScreenPoint({ player_ptr->position[0] ,player_ptr->position[1] ,player_ptr->position[2] });
		s_text text_atk;

		text_atk.text = s_factory::get_instance()->get_words_siv_string(player_ptr->player_attack_words_type);

		if (hard_atteck)
		{
			text_atk.text = s_factory::get_instance()->get_words_siv_string(player_ptr->player_hard_attack_words_type);
		}

		add_text_command add_text_cmd(sence_ptr, text_atk, time_ptr->total_time(), screen_pos.x, screen_pos.y);
		add_text_cmd.execute();
	}

}

void generate_bullet_command::execute()
{
	assert(sence_ptr);

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

	s_bullet* bullet_ptr = s_factory::get_instance()->create_bullet(create_type);
	bullet_ptr->move_dir[0] = bullet_dir.x;
	bullet_ptr->move_dir[1] = bullet_dir.y;
	bullet_ptr->move_dir[2] = bullet_dir.z;

	bullet_ptr->position[0] = bullet_start_position.x;
	bullet_ptr->position[1] = bullet_start_position.y;
	bullet_ptr->position[2] = bullet_start_position.z;

	bullet_ptr->bullet_behavior_tree_cmd = new bullet_behavior_tree_command(bullet_ptr,sence_ptr);

	bullet_ptr->update_state();

	sence_ptr->bullet_group.insert(bullet_ptr);

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
				text_atk.text = s_factory::get_instance()->get_words_siv_string(monster_ptr->monster_attack_words_type);
				add_text_command add_text_cmd(sence_ptr, text_atk, time_ptr->total_time(), screen_pos.x, screen_pos.y);
				add_text_cmd.execute();

				sence_ptr->monster_group.erase(monster_ptr);
				itt = monster_set.erase(itt);
				delete monster_ptr;
				is_hit = true;
				sence_ptr->score++;
				break;
				
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
			s_text text_boom;
			text_boom.text = s_factory::get_instance()->get_words_siv_string(bullet_ptr->bullet_boom_words_type);
			text_boom.size = 60;
			add_text_command add_text_cmd(sence_ptr, text_boom, time_ptr->total_time(), screen_pos.x, screen_pos.y);
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
			s_text text_hit;
			text_hit.text = s_factory::get_instance()->get_words_siv_string(monster_ptr->monster_was_hit_words_type);
			text_hit.size = 60;
			add_text_command add_text_cmd(sence_ptr, text_hit, time_ptr->total_time(), screen_pos.x, screen_pos.y);
			add_text_cmd.execute();

			sence_ptr->monster_group.erase(monster_ptr);
			itt = monster_set.erase(itt);

			delete monster_ptr;

			sence_ptr->player->hp--;
			sence_ptr->score++;
		}
		else
		{
			itt++;
		}
	}

	//item player collision 
	for (auto it = sence_ptr->item_group.begin(); it != sence_ptr->item_group.end();)
	{
		s_item* item_ptr = (*it);
		bool is_picked = false;
		if (item_ptr->mesh.intersects(sence_ptr->player->mesh))
		{
			is_picked = true;
			pick_item_command pick_item_cmd(sence_ptr, item_ptr);
			pick_item_cmd.execute();
		}

		if (is_picked
			|| item_ptr->generate_time + item_ptr->time < time_ptr->total_time())
		{
			it = sence_ptr->item_group.erase(it);
			delete(item_ptr);
		}
		else
		{
			it++;
		}
	}
}

void generate_item_command::execute()
{
	assert(sence_ptr);

	//random type
	int item_type = rand() % ITEM_TYPE_NUMBER;

	s_item* item_ptr = s_factory::get_instance()->create_item((ITEM_TYPE)item_type);
	item_ptr->generate_time = s_timer::get_instance()->total_time();
	//random position
	item_ptr->position[0] = rand() % (sence_ptr->map_width * 2) - sence_ptr->map_width;
	item_ptr->position[1] = 2;
	item_ptr->position[2] = rand() % (sence_ptr->map_length * 2) - sence_ptr->map_length;
	item_ptr->update_state();
	sence_ptr->item_group.insert(item_ptr);
}

void pick_item_command::execute()
{
	assert(sence_ptr);
	assert(item_ptr);

	switch (item_ptr->item_type)
	{
	case ITEM_TYPE_ADD_HP:
	{
		sence_ptr->player->hp += item_ptr->item_data;
	}
	break;
	case ITEM_TYPE_ADD_BULLET:
	{
		sence_ptr->player->bullet_number += item_ptr->item_data;
	}
	break;
	case ITEM_TYPE_ADD_TIME:
	{
		sence_ptr->remain_time += item_ptr->item_data;
	}
	break;
	case ITEM_TYPE_BULLET_UNLIMITED:
	{
		sence_ptr->player->current_bullet_type = BULLET_TYPE_UNLIMITED;
		sence_ptr->player->special_bullet_remain_time = item_ptr->item_data;
	}
	break;
	case ITEM_TYPE_BULLET_FIREWORKS:
	{
		sence_ptr->player->current_bullet_type = BULLET_TYPE_FIREWORKS;
		sence_ptr->player->special_bullet_remain_time = item_ptr->item_data;
	}
	break;
	case ITEM_TYPE_BULLET_SHOTGUN:
	{
		sence_ptr->player->current_bullet_type = BULLET_TYPE_SHOTGUN;
		sence_ptr->player->special_bullet_remain_time = item_ptr->item_data;
	}
	break;
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

	//refresh all text in the screen
	for(auto it = sence_ptr->text_group.begin(); it != sence_ptr->text_group.end();)
	{
		if ((*it).start_time + (*it).stay_time < time_ptr->total_time())
		{
			it = sence_ptr->text_group.erase(it);
			continue;
		}

		(*it).screen_position[1] -= (*it).speed * time_ptr->detail_time();
		auto text_font = SimpleGUI::GetFont();
		text_font.setBufferThickness((*it).size);
		text_font((*it).text).draw((*it).screen_position[0], (*it).screen_position[1], ColorF{ 0.11 });
		it++;
	}
}

void display_player_state_command::execute()
{
	assert(sence_ptr);
	s_factory* factory_ptr = s_factory::get_instance();

	//HP
	{
		String hp_words = factory_ptr->get_words_siv_string(WORDS_TYPE_PLAYER_STATE_HP_NAME);
		hp_words += U":";
		hp_words += Unicode::Widen(std::to_string(sence_ptr->player->hp));

		auto hp_font = SimpleGUI::GetFont();
		hp_font.setBufferThickness(3);
		hp_font(hp_words).draw(10, 10, ColorF({ 0.0,0.0,0.0 }));
	}

	//BULLET NUMBER
	{
		String bullet_numbet_words = factory_ptr->get_words_siv_string(WORDS_TYPE_PLAYER_STATE_BULLET_NUMBER_NAME);
		bullet_numbet_words += U":";
		bullet_numbet_words += Unicode::Widen(std::to_string(sence_ptr->player->bullet_number));

		auto bullet_numbet_font = SimpleGUI::GetFont();
		bullet_numbet_font.setBufferThickness(3);
		bullet_numbet_font(bullet_numbet_words).draw(10, 100, ColorF({ 0.0,0.0,0.0 }));
	}

	//SCORE
	{
		String score_words;
		score_words = Unicode::Widen(std::to_string((int)sence_ptr->score));

		auto score_font = SimpleGUI::GetFont();
		score_font.setBufferThickness(3);
		score_font(score_words).draw(600, 10, ColorF({ 0.0,0.0,0.0 }));
	}
	//special gun remain time
	{
		if (sence_ptr->player->special_bullet_remain_time > 0)
		{
			String remain_words;
			remain_words = Unicode::Widen(std::to_string(sence_ptr->player->special_bullet_remain_time));

			auto remain_font = SimpleGUI::GetFont();
			remain_font.setBufferThickness(3);
			remain_font(remain_words).draw(600, 30, ColorF({ 0.0,0.0,0.0 }));
		}
	}

	//TIME
	{
		String time_words = factory_ptr->get_words_siv_string(WORDS_TYPE_PLAYER_STATE_TIME_NAME);
		time_words += U":";
		time_words += Unicode::Widen(std::to_string(sence_ptr->remain_time));

		auto time_font = SimpleGUI::GetFont();
		time_font.setBufferThickness(3);
		time_font(time_words).draw(800, 10, ColorF({ 0.0,0.0,0.0 }));
	}

	
}
