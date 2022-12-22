#pragma once
#include "s_object.h"
#include <vector>
#include <string>
#include <map>
using std::vector;
using std::string;
using std::map;

//
// 0. change s_object.h enum struct .etc
// 1. first create/change .fbs
// 2. use flatc.exe in cmd to generate .h
// 3. change s_factory function
// 4. change .xlsx
// 5. use powershell to generate .json
// 6. use flatc.exe in cmd to generate .bin
//


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

	void read_bin(string in_file_name, std::vector<unsigned char> & buffer);

	template<typename t_type>
	t_type convert_string_to_enum_type(string in_type_name);

	template<>
	MONSTER_TYPE convert_string_to_enum_type<MONSTER_TYPE>(string in_type_name);

	template<>
	PLAYER_TYPE convert_string_to_enum_type<PLAYER_TYPE>(string in_type_name);

	template<>
	BULLET_TYPE convert_string_to_enum_type<BULLET_TYPE>(string in_type_name);

	template<>
	ITEM_TYPE convert_string_to_enum_type<ITEM_TYPE>(string in_type_name);

	template<>
	WORDS_TYPE convert_string_to_enum_type<WORDS_TYPE>(string in_type_name);


	void load_words(LANGUAGE_TYPE in_language_type);

	string get_words(WORDS_TYPE in_words_type);

	String get_words_siv_string(WORDS_TYPE in_words_type);

private:
	
	map<MONSTER_TYPE,s_monster> monster_type_map;
	map<PLAYER_TYPE, s_player> player_type_map;
	map<BULLET_TYPE,s_bullet> bullet_type_map;
	map<ITEM_TYPE,s_item> item_type_map;
	map<WORDS_TYPE, string> words_type_map;
};
