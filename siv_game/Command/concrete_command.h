#pragma once
#include "s_command.h"
#include "../Object/s_sence.h"

class generate_monster_command : public s_command
{
private:
	s_sence* sence_ptr = nullptr;
public:
	generate_monster_command(s_sence* in_sence_ptr) : sence_ptr(in_sence_ptr) {};
	virtual void execute() override;
};

class monster_move_command : public s_command
{
private:
	s_monster* monster_ptr = nullptr;
	s_sence* sence_ptr = nullptr;
public:
	monster_move_command(s_monster* in_monster_ptr, s_sence* in_sence_ptr) : monster_ptr(in_monster_ptr), sence_ptr(in_sence_ptr) {};
	virtual void execute() override;
};

class monster_behavior_tree_command : public s_command
{
private:
	s_monster* monster_ptr = nullptr;
	s_sence* sence_ptr = nullptr;
public:
	monster_behavior_tree_command(s_monster* in_monster_ptr, s_sence* in_sence_ptr) : monster_ptr(in_monster_ptr), sence_ptr(in_sence_ptr) {};
	virtual void execute() override;
};

class player_control_command : public s_command
{
private:
	s_player* player_ptr = nullptr;
	s_sence* sence_ptr = nullptr;
public:
	player_control_command(s_player* in_player_ptr, s_sence* in_sence_ptr) : player_ptr(in_player_ptr), sence_ptr(in_sence_ptr) {};
	virtual void execute() override;
};

class player_attack_command : public s_command
{
private:
	s_player* player_ptr = nullptr;
	s_sence* sence_ptr = nullptr;
public:
	player_attack_command(s_player* in_player_ptr, s_sence* in_sence_ptr) : player_ptr(in_player_ptr), sence_ptr(in_sence_ptr) {};
	virtual void execute() override;
};

class generate_bullet_command : public s_command
{
private:
	BULLET_TYPE create_type = BULLET_TYPE_NORMAL;
	s_sence* sence_ptr = nullptr;
	Float3 bullet_dir = {0,0,0};
	Float3 bullet_start_position = { 0,0,0 };
public:
	generate_bullet_command(s_sence* in_sence_ptr, BULLET_TYPE in_create_type, Float3 in_bullet_dir, Float3 in_bullet_start_position) : sence_ptr(in_sence_ptr), create_type(in_create_type), bullet_dir(in_bullet_dir), bullet_start_position(in_bullet_start_position) {};
	virtual void execute() override;
};

class bullet_behavior_tree_command : public s_command
{
private:
	s_bullet* bullet_ptr = nullptr;
	s_sence* sence_ptr = nullptr;
public:
	bullet_behavior_tree_command(s_bullet* in_bullet_ptr, s_sence* in_sence_ptr) : bullet_ptr(in_bullet_ptr), sence_ptr(in_sence_ptr) {};
	virtual void execute() override;
};

class bullet_move_command : public s_command
{
private:
	s_bullet* bullet_ptr = nullptr;
	s_sence* sence_ptr = nullptr;
public:
	bullet_move_command(s_bullet* in_bullet_ptr, s_sence* in_sence_ptr) : bullet_ptr(in_bullet_ptr), sence_ptr(in_sence_ptr) {};
	virtual void execute() override;
};

class check_collision_command : public s_command
{
private:
	s_sence* sence_ptr = nullptr;
public:
	check_collision_command( s_sence* in_sence_ptr) : sence_ptr(in_sence_ptr) {};
	virtual void execute() override;
};

class generate_item_command : public s_command
{
private:
	s_sence* sence_ptr = nullptr;
public:
	generate_item_command(s_sence* in_sence_ptr) : sence_ptr(in_sence_ptr) {};
	virtual void execute() override;
};

class pick_item_command : public s_command
{
private:
	s_sence* sence_ptr = nullptr;
	s_item* item_ptr = nullptr;
public:
	pick_item_command(s_sence* in_sence_ptr, s_item* in_item_ptr) : sence_ptr(in_sence_ptr), item_ptr(in_item_ptr){};
	virtual void execute() override;
};

class add_text_command : public s_command
{
private:
	add_text_command() {};
private:
	s_sence* sence_ptr = nullptr;
	s_text text_refer;
	float screen_x = 0;
	float screen_y = 0;
	float start_time = 0;
public:
	add_text_command(
		s_sence* in_sence_ptr,
		s_text in_text_refer,
		float in_start_time,
		float in_screen_x,
		float in_screen_y) :
		sence_ptr(in_sence_ptr),
		text_refer(in_text_refer),
		start_time(in_start_time),
		screen_x(in_screen_x),
		screen_y(in_screen_y){};
	virtual void execute() override;
};

class refresh_text_command : public s_command
{
private:
	s_sence* sence_ptr = nullptr;
public:
	refresh_text_command(s_sence* in_sence_ptr) : sence_ptr(in_sence_ptr) {};
	virtual void execute() override;
};

class display_player_state_command : public s_command
{
private:
	s_sence* sence_ptr = nullptr;
public:
	display_player_state_command(s_sence* in_sence_ptr) : sence_ptr(in_sence_ptr) {};
	virtual void execute() override;
};

