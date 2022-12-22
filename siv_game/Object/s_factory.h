#pragma once
#include "s_object.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

class s_factory
{
private:
	s_factory() {};
	s_factory(const s_factory&) {};
	s_factory& operator=(const s_factory&) {};

public:

	static s_factory * get_instance()
	{
		static s_factory factory_instance;
		return &factory_instance;
	}

	s_monster* create_monster(MONSTER_TYPE in_monster_type = MONSTER_TYPE_NORMAL);

	s_player* create_player(PLAYER_TYPE in_player_type = PLAYER_TYPE_NORMAL);

	s_bullet* create_bullet(BULLET_TYPE in_bullet_type = BULLET_TYPE_NORMAL);

	s_item* create_item(ITEM_TYPE in_item_type = ITEM_TYPE_ADD_HP);



	void load_object_data();

	void load_words();

	string get_words(WORDS_TYPE in_words_type);

	String get_words_siv_string(WORDS_TYPE in_words_type);

private:
	vector<string> words_type_map;
	vector<s_monster> monster_type_map;
	vector<s_bullet> bullet_type_map;
	vector<s_item> item_type_map;
	vector<s_player> player_type_map;
};
