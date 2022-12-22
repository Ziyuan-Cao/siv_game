#pragma once
#include "../Command/s_command.h"
#include  <string>
# include <Siv3D.hpp>

enum LANGUAGE_TYPE
{
	LANGUAGE_TYPE_JAPANESE = 0,
	LANGUAGE_TYPE_CANTONESE = 1,
	LANGUAGE_TYPE_ENGLISH = 2,
	LANGUAGE_TYPE_UNKNOW = 3,
	LANGUAGE_TYPE_NUMBER = 4
};

enum WORDS_TYPE
{
	WORDS_TYPE_MONSTER_ATTACK = 0,
	WORDS_TYPE_MONSTER_WAS_HIT = 1,
	WORDS_TYPE_PLAYER_ATTACK = 2,
	WORDS_TYPE_PLAYER_HARD_ATTACK = 3,
	WORDS_TYPE_PLAYER_WAS_HIT = 4,
	WORDS_TYPE_BULLET_BOOM = 5,
	WORDS_TYPE_ITEM_WAS_PICKED = 6,
	WORDS_TYPE_MONSTER_NAME = 7,
	WORDS_TYPE_PLAYER_NAME = 8,
	WORDS_TYPE_BULLET_NAME = 9,
	WORDS_TYPE_ITEM_NAME = 10,
	WORDS_TYPE_LANGUAGE_NAME = 11,
	WORDS_TYPE_START_GAME_BUTTON_NAME = 12,
	WORDS_TYPE_EXIT_GAME_BUTTON_NAME = 13,
	WORDS_TYPE_RETURN_MAIN_MENU_BUTTON_NAME = 14,
	WORDS_TYPE_GAME_TITLE_NAME = 15,
	WORDS_TYPE_GAME_OVER_TITLE_NAME = 16,
	WORDS_TYPE_PLAYER_STATE_HP_NAME = 17,
	WORDS_TYPE_PLAYER_STATE_BULLET_NUMBER_NAME = 18,
	WORDS_TYPE_PLAYER_STATE_TIME_NAME = 19,
	WORDS_TYPE_SCORE_NAME = 20,
	WORDS_TYPE_NUMBER = 21
};

enum MONSTER_TYPE
{
	MONSTER_TYPE_NORMAL = 0,
	MONSTER_TYPE_DOG = 1,
	MONSTER_TYPE_BOSS = 2,
	MONSTER_TYPE_BOMB = 3,
	MONSTER_TYPE_NUMBER = 4
};

enum PLAYER_TYPE
{
	PLAYER_TYPE_NORMAL = 0,
	PLAYER_TYPE_NUMBER = 1
};

enum BULLET_TYPE
{
	BULLET_TYPE_NORMAL = 0,
	BULLET_TYPE_UNLIMITED = 1,
	BULLET_TYPE_FIREWORKS = 2,
	BULLET_TYPE_SHOTGUN = 3,
	BULLET_TYPE_HARD_ATTACK = 4,
	BULLET_TYPE_NUMBER = 5
};

enum ITEM_TYPE
{
	ITEM_TYPE_ADD_HP = 0,
	ITEM_TYPE_ADD_BULLET = 1,
	ITEM_TYPE_ADD_TIME = 2,
	ITEM_TYPE_BULLET_UNLIMITED = 3,
	ITEM_TYPE_BULLET_FIREWORKS = 4,
	ITEM_TYPE_BULLET_SHOTGUN = 5,
	ITEM_TYPE_NUMBER = 6
};

template<typename t_mesh_type>
struct s_object
{
	String name = U"Empty name";
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
	MONSTER_TYPE monster_type = MONSTER_TYPE_NORMAL;
	int hp = 1;
	int speed = 1;
	int damage = 1;
	int search_radius = 10;
	int score_value = 1;
	float standby_time = 3; // standby time

	WORDS_TYPE monster_attack_words_type = WORDS_TYPE_MONSTER_ATTACK;
	WORDS_TYPE monster_was_hit_words_type = WORDS_TYPE_MONSTER_WAS_HIT;

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

	s_monster(const s_monster& in_monster)
	{
		name = in_monster.name;
		monster_type = in_monster.monster_type;
		hp = in_monster.hp;
		speed = in_monster.speed;
		damage = in_monster.damage;
		score_value = in_monster.score_value;
		color[0] = in_monster.color[0];
		color[1] = in_monster.color[1];
		color[2] = in_monster.color[2];
		scale = in_monster.scale;
		monster_attack_words_type = in_monster.monster_attack_words_type;
		monster_was_hit_words_type = in_monster.monster_was_hit_words_type;
	};

	s_monster& operator=(const s_monster& in_monster)
	{
		s_monster res_monster;
		res_monster.name = in_monster.name;
		res_monster.monster_type = in_monster.monster_type;
		res_monster.hp = in_monster.hp;
		res_monster.speed = in_monster.speed;
		res_monster.damage = in_monster.damage;
		res_monster.score_value = in_monster.score_value;
		res_monster.color[0] = in_monster.color[0];
		res_monster.color[1] = in_monster.color[1];
		res_monster.color[2] = in_monster.color[2];
		res_monster.scale = in_monster.scale;
		res_monster.monster_attack_words_type = in_monster.monster_attack_words_type;
		res_monster.monster_was_hit_words_type = in_monster.monster_was_hit_words_type;
		return res_monster;
	};

	s_command* monster_behavior_tree_cmd = nullptr;
};

struct s_player : public s_object<Sphere>
{
	PLAYER_TYPE player_type = PLAYER_TYPE_NORMAL;
	int hp = 10;
	int speed = 10;
	int bullet_number = 50;

	WORDS_TYPE player_attack_words_type = WORDS_TYPE_PLAYER_ATTACK;
	WORDS_TYPE player_hard_attack_words_type = WORDS_TYPE_PLAYER_HARD_ATTACK;
	WORDS_TYPE player_was_hit_words_type = WORDS_TYPE_PLAYER_WAS_HIT;

	BULLET_TYPE current_bullet_type = BULLET_TYPE_NORMAL;
	float special_bullet_remain_time = 0;

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

	s_player(const s_player& in_player)
	{
		player_type = in_player.player_type;
		name = in_player.name;
		hp = in_player.hp;
		speed = in_player.speed;
		bullet_number = in_player.bullet_number;
		color[0] = in_player.color[0];
		color[1] = in_player.color[1];
		color[2] = in_player.color[2];
		scale = in_player.scale;
		player_attack_words_type = in_player.player_attack_words_type;
		player_hard_attack_words_type = in_player.player_hard_attack_words_type;
		player_was_hit_words_type = in_player.player_was_hit_words_type;
	};

	s_player& operator=(const s_player& in_player)
	{
		s_player res_player;
		res_player.player_type = in_player.player_type;
		res_player.name = in_player.name;
		res_player.hp = in_player.hp;
		res_player.speed = in_player.speed;
		res_player.bullet_number = in_player.bullet_number;
		res_player.color[0] = in_player.color[0];
		res_player.color[1] = in_player.color[1];
		res_player.color[2] = in_player.color[2];
		res_player.scale = in_player.scale;
		res_player.player_attack_words_type = in_player.player_attack_words_type;
		res_player.player_hard_attack_words_type = in_player.player_hard_attack_words_type;
		res_player.player_was_hit_words_type = in_player.player_was_hit_words_type;
		return res_player;
	};

	s_command* player_control_cmd = nullptr;
};

struct s_bullet : public s_object<Sphere>
{
	BULLET_TYPE bullet_type = BULLET_TYPE_NORMAL;
	int speed = 20;
	int damage = 1;
	double move_dir[3] = { 0,0,0 };

	WORDS_TYPE bullet_boom_words_type = WORDS_TYPE_BULLET_BOOM;

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

	s_bullet(const s_bullet& in_bullet)
	{
		bullet_type = in_bullet.bullet_type;
		name = in_bullet.name;
		speed = in_bullet.speed;
		damage = in_bullet.damage;
		color[0] = in_bullet.color[0];
		color[1] = in_bullet.color[1];
		color[2] = in_bullet.color[2];
		scale = in_bullet.scale;
		bullet_boom_words_type = in_bullet.bullet_boom_words_type;
	};

	s_bullet& operator=(const s_bullet& in_bullet)
	{
		s_bullet res_bullet;
		res_bullet.bullet_type = in_bullet.bullet_type;
		res_bullet.name = in_bullet.name;
		res_bullet.speed = in_bullet.speed;
		res_bullet.damage = in_bullet.damage;
		res_bullet.color[0] = in_bullet.color[0];
		res_bullet.color[1] = in_bullet.color[1];
		res_bullet.color[2] = in_bullet.color[2];
		res_bullet.scale = in_bullet.scale;
		res_bullet.bullet_boom_words_type = in_bullet.bullet_boom_words_type;

		return res_bullet;
	}

	s_command* bullet_behavior_tree_cmd = nullptr;
};

struct s_item : public s_object<Sphere>
{
	ITEM_TYPE item_type = ITEM_TYPE_ADD_HP;
	float item_data = 10;
	float time = 10;
	float generate_time = 0;
	WORDS_TYPE item_was_picked_words_type = WORDS_TYPE_ITEM_WAS_PICKED;

	s_item()
	{
		color[0] = 0.9;
		color[1] = 0.1;
		color[2] = 0.1;
	}

	s_item(const s_item& in_item)
	{
		item_type = in_item.item_type;
		name = in_item.name;
		item_data = in_item.item_data;
		time = in_item.time;
		color[0] = in_item.color[0];
		color[1] = in_item.color[1];
		color[2] = in_item.color[2];
		scale = in_item.scale;
		item_was_picked_words_type = in_item.item_was_picked_words_type;
	};

	s_item& operator=(const s_item& in_item)
	{
		s_item res_item;
		res_item.item_type = in_item.item_type;
		res_item.name = in_item.name;
		res_item.item_data = in_item.item_data;
		res_item.time = in_item.time;
		res_item.color[0] = in_item.color[0];
		res_item.color[1] = in_item.color[1];
		res_item.color[2] = in_item.color[2];
		res_item.scale = in_item.scale;
		res_item.item_was_picked_words_type = in_item.item_was_picked_words_type;

		return res_item;
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
	String text;
};
