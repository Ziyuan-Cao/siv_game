#include "s_factory.h"

s_monster* s_factory::create_monster(MONSTER_TYPE in_monster_type)
{
	if (in_monster_type >= monster_type_map.size())
	{
		return new s_monster();
	}

	return new s_monster(monster_type_map[in_monster_type]);
}

s_player* s_factory::create_player(PLAYER_TYPE in_player_type)
{
	if (in_player_type >= player_type_map.size())
	{
		return new s_player();
	}

	return new s_player(player_type_map[in_player_type]);
	
}

s_bullet* s_factory::create_bullet(BULLET_TYPE in_bullet_type)
{
	if (in_bullet_type >= bullet_type_map.size())
	{
		return new s_bullet();
	}

	return new s_bullet(bullet_type_map[in_bullet_type]);
}

s_item* s_factory::create_item(ITEM_TYPE in_item_type)
{
	if (in_item_type >= item_type_map.size())
	{
		return new s_item();
	}

	return new s_item(item_type_map[in_item_type]);
}

string s_factory::get_words(WORDS_TYPE in_words_type)
{
	if (in_words_type >= words_type_map.size())
	{
		return "EMPTY WORDS!";
	}

	return words_type_map[in_words_type];
}

String s_factory::get_words_siv_string(WORDS_TYPE in_words_type)
{
	return Unicode::Widen(get_words(in_words_type));
}

void s_factory::load_object_data()
{

}

void s_factory::load_words()
{

}
