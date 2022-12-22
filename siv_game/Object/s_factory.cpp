#include "s_factory.h"
#include "Data/monsters_type_generated.h"
#include "Data/players_type_generated.h"
#include "Data/bullets_type_generated.h"
#include "Data/items_type_generated.h"
#include "Data/words_type_generated.h"
#include <fstream>
#include <Windows.h>
using namespace fbs_bandai_siv_game_bin_input;

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
	return Unicode::FromUTF8(get_words(in_words_type));
}

void s_factory::read_bin(string in_file_name, std::vector<unsigned char>& out_buffer)
{
	using namespace std;

	ifstream data_reader(in_file_name, ios::in | ios::binary);

	assert(data_reader);
	out_buffer.clear();
	out_buffer.assign(std::istreambuf_iterator<char>(data_reader), {});

	data_reader.close();
}

void s_factory::load_object_data()
{
	//Debug
	//wchar_t* path = new wchar_t[MAX_PATH];
	//GetCurrentDirectory(MAX_PATH, path);
	//printf("%s\n", path);

	//Monster reading
	{
		monster_type_map.clear();

		std::vector<unsigned char> data_buffer;
		read_bin("Data/monsters_data.bin", data_buffer);

		auto ptr = data_buffer.data();

		auto monsters = Getmonsters_type(ptr);
		auto m_list = monsters->monsters_data();

		for (auto it = m_list->begin(); it != m_list->end(); it++)
		{
			auto m = *it;
			auto m_type = convert_string_to_enum_type<MONSTER_TYPE>(m->monster_type()->c_str());
			monster_type_map[m_type].name = Unicode::FromUTF8(m->name()->c_str());
			auto& m_it = monster_type_map[m_type];
			m_it.monster_type = m_type;
			m_it.hp = m->hp();
			m_it.speed = m->speed();
			m_it.color[0] = m->color0();
			m_it.color[1] = m->color1();
			m_it.color[2] = m->color2();
			m_it.scale = m->scale();
			m_it.monster_attack_words_type = convert_string_to_enum_type<WORDS_TYPE>(m->monster_attack_words_type()->c_str());
			m_it.monster_was_hit_words_type = convert_string_to_enum_type<WORDS_TYPE>(m->monster_was_hit_words_type()->c_str());
		}
	}

	//Player reading
	{
		player_type_map.clear();

		std::vector<unsigned char> data_buffer;
		read_bin("Data/players_data.bin", data_buffer);

		auto ptr = data_buffer.data();

		auto players = Getplayers_type(ptr);
		auto p_list = players->players_data();

		for (auto it = p_list->begin(); it != p_list->end(); it++)
		{
			auto p = *it;
			auto p_type = convert_string_to_enum_type<PLAYER_TYPE>(p->player_type()->c_str());
			player_type_map[p_type].name = Unicode::FromUTF8(p->name()->c_str());
			auto& p_it = player_type_map[p_type];
			p_it.player_type = p_type;
			p_it.hp = p->hp();
			p_it.speed = p->speed();
			p_it.bullet_number = p->bullet_number();
			p_it.color[0] = p->color0();
			p_it.color[1] = p->color1();
			p_it.color[2] = p->color2();
			p_it.scale = p->scale();
			p_it.player_attack_words_type = convert_string_to_enum_type<WORDS_TYPE>(p->player_attack_words_type()->c_str());
			p_it.player_hard_attack_words_type = convert_string_to_enum_type<WORDS_TYPE>(p->player_hard_attack_words_type()->c_str());
			p_it.player_was_hit_words_type = convert_string_to_enum_type<WORDS_TYPE>(p->player_was_hit_words_type()->c_str());
		}
	}

	//Bullet reading
	{
		bullet_type_map.clear();

		std::vector<unsigned char> data_buffer;
		read_bin("Data/bullets_data.bin", data_buffer);

		auto ptr = data_buffer.data();

		auto bullets = Getbullets_type(ptr);
		auto b_list = bullets->bullets_data();

		for (auto it = b_list->begin(); it != b_list->end(); it++)
		{
			auto b = *it;
			auto b_type = convert_string_to_enum_type<BULLET_TYPE>(b->bullet_type()->c_str());
			bullet_type_map[b_type].name = Unicode::FromUTF8(b->name()->c_str());
			auto& b_it = bullet_type_map[b_type];
			b_it.bullet_type = b_type;
			b_it.speed = b->speed();
			b_it.color[0] = b->color0();
			b_it.color[1] = b->color1();
			b_it.color[2] = b->color2();
			b_it.scale = b->scale();
			b_it.bullet_boom_words_type = convert_string_to_enum_type<WORDS_TYPE>(b->bullet_boom_words_type()->c_str());
		}
	}

	//Item reading
	{
		item_type_map.clear();

		std::vector<unsigned char> data_buffer;
		read_bin("Data/items_data.bin", data_buffer);

		auto ptr = data_buffer.data();

		auto items = Getitems_type(ptr);
		auto i_list = items->items_data();

		for (auto it = i_list->begin(); it != i_list->end(); it++)
		{
			auto i = *it;
			auto i_type = convert_string_to_enum_type<ITEM_TYPE>(i->item_type()->c_str());
			item_type_map[i_type].name = Unicode::FromUTF8(i->name()->c_str());
			auto& i_it = item_type_map[i_type];
			i_it.item_type = i_type;
			i_it.item_data = i->item_data();
			i_it.time = i->time();
			i_it.color[0] = i->color0();
			i_it.color[1] = i->color1();
			i_it.color[2] = i->color2();
			i_it.scale = i->scale();
			i_it.item_was_picked_words_type = convert_string_to_enum_type<WORDS_TYPE>(i->item_was_picked_words_type()->c_str());
		}
	}
}

void s_factory::load_words(LANGUAGE_TYPE in_language_type)
{
	//Words reading
	{
		words_type_map.clear();

		std::vector<unsigned char> data_buffer;
		read_bin("Data/words_data.bin", data_buffer);

		auto ptr = data_buffer.data();

		auto words = Getwords_type(ptr);
		auto w_list = words->words_data();

		for (auto it = w_list->begin(); it != w_list->end(); it++)
		{
			auto w = *it;
			auto w_type = convert_string_to_enum_type<WORDS_TYPE>(w->word_type()->c_str());

			switch (in_language_type)
			{
			case LANGUAGE_TYPE_JAPANESE:
				words_type_map[w_type] = w->japanese()->c_str();
				break;
			case LANGUAGE_TYPE_CANTONESE:
				words_type_map[w_type] = w->cantonese()->c_str();
				break;
			case LANGUAGE_TYPE_ENGLISH:
				words_type_map[w_type] = w->english()->c_str();
				break;
			case LANGUAGE_TYPE_UNKNOW:
				words_type_map[w_type] = w->unknow()->c_str();
				break;
			default:
				break;
			}
		}
	}
}

template<>
MONSTER_TYPE s_factory::convert_string_to_enum_type<MONSTER_TYPE>(string in_type_name)
{
	if (in_type_name == "MONSTER_TYPE_NORMAL")
	{
		return MONSTER_TYPE_NORMAL;
	}

	//defualt
	return MONSTER_TYPE_NORMAL;
}

template<>
PLAYER_TYPE s_factory::convert_string_to_enum_type<PLAYER_TYPE>(string in_type_name)
{
	if (in_type_name == "PLAYER_TYPE_NORMAL")
	{
		return PLAYER_TYPE_NORMAL;
	}

	//defualt
	return PLAYER_TYPE_NORMAL;
}

template<>
BULLET_TYPE s_factory::convert_string_to_enum_type<BULLET_TYPE>(string in_type_name)
{
	if (in_type_name == "BULLET_TYPE_NORMAL")
	{
		return BULLET_TYPE_NORMAL;
	}

	if (in_type_name == "BULLET_TYPE_UNLIMITED")
	{
		return BULLET_TYPE_UNLIMITED;
	}

	if (in_type_name == "BULLET_TYPE_FIREWORKS")
	{
		return BULLET_TYPE_FIREWORKS;
	}

	if (in_type_name == "BULLET_TYPE_SHOTGUN")
	{
		return BULLET_TYPE_SHOTGUN;
	}

	if (in_type_name == "BULLET_TYPE_HARD_ATTACK")
	{
		return BULLET_TYPE_HARD_ATTACK;
	}

	//defualt
	return BULLET_TYPE_NORMAL;
}

template<>
ITEM_TYPE s_factory::convert_string_to_enum_type<ITEM_TYPE>(string in_type_name)
{
	if (in_type_name == "ITEM_TYPE_ADD_HP")
	{
		return ITEM_TYPE_ADD_HP;
	}

	if (in_type_name == "ITEM_TYPE_ADD_BULLET")
	{
		return ITEM_TYPE_ADD_BULLET;
	}

	if (in_type_name == "ITEM_TYPE_ADD_TIME")
	{
		return ITEM_TYPE_ADD_TIME;
	}

	if (in_type_name == "ITEM_TYPE_BULLET_UNLIMITED")
	{
		return ITEM_TYPE_BULLET_UNLIMITED;
	}

	if (in_type_name == "ITEM_TYPE_BULLET_FIREWORKS")
	{
		return ITEM_TYPE_BULLET_FIREWORKS;
	}

	if (in_type_name == "ITEM_TYPE_BULLET_SHOTGUN")
	{
		return ITEM_TYPE_BULLET_SHOTGUN;
	}

	//defualt
	return ITEM_TYPE_ADD_HP;
}

template<>
WORDS_TYPE s_factory::convert_string_to_enum_type<WORDS_TYPE>(string in_type_name)
{
	if (in_type_name == "WORDS_TYPE_MONSTER_ATTACK")
	{
		return WORDS_TYPE_MONSTER_ATTACK;
	}

	if (in_type_name == "WORDS_TYPE_MONSTER_WAS_HIT")
	{
		return WORDS_TYPE_MONSTER_WAS_HIT;
	}

	if (in_type_name == "WORDS_TYPE_PLAYER_ATTACK")
	{
		return WORDS_TYPE_PLAYER_ATTACK;
	}

	if (in_type_name == "WORDS_TYPE_PLAYER_HARD_ATTACK")
	{
		return WORDS_TYPE_PLAYER_HARD_ATTACK;
	}

	if (in_type_name == "WORDS_TYPE_PLAYER_WAS_HIT")
	{
		return WORDS_TYPE_PLAYER_WAS_HIT;
	}

	if (in_type_name == "WORDS_TYPE_BULLET_BOOM")
	{
		return WORDS_TYPE_BULLET_BOOM;
	}

	if (in_type_name == "WORDS_TYPE_ITEM_WAS_PICKED")
	{
		return WORDS_TYPE_ITEM_WAS_PICKED;
	}

	if (in_type_name == "WORDS_TYPE_MONSTER_NAME")
	{
		return WORDS_TYPE_MONSTER_NAME;
	}

	if (in_type_name == "WORDS_TYPE_PLAYER_NAME")
	{
		return WORDS_TYPE_PLAYER_NAME;
	}

	if (in_type_name == "WORDS_TYPE_BULLET_NAME")
	{
		return WORDS_TYPE_BULLET_NAME;
	}

	if (in_type_name == "WORDS_TYPE_ITEM_NAME")
	{
		return WORDS_TYPE_ITEM_NAME;
	}

	if (in_type_name == "WORDS_TYPE_LANGUAGE_NAME")
	{
		return WORDS_TYPE_LANGUAGE_NAME;
	}

	if (in_type_name == "WORDS_TYPE_START_GAME_BUTTON_NAME")
	{
		return WORDS_TYPE_START_GAME_BUTTON_NAME;
	}

	if (in_type_name == "WORDS_TYPE_EXIT_GAME_BUTTON_NAME")
	{
		return WORDS_TYPE_EXIT_GAME_BUTTON_NAME;
	}

	if (in_type_name == "WORDS_TYPE_RETURN_MAIN_MENU_BUTTON_NAME")
	{
		return WORDS_TYPE_RETURN_MAIN_MENU_BUTTON_NAME;
	}

	if (in_type_name == "WORDS_TYPE_GAME_TITLE_NAME")
	{
		return WORDS_TYPE_GAME_TITLE_NAME;
	}

	if (in_type_name == "WORDS_TYPE_GAME_OVER_TITLE_NAME")
	{
		return WORDS_TYPE_GAME_OVER_TITLE_NAME;
	}

	if (in_type_name == "WORDS_TYPE_PLAYER_STATE_HP_NAME")
	{
		return WORDS_TYPE_PLAYER_STATE_HP_NAME;
	}

	if (in_type_name == "WORDS_TYPE_PLAYER_STATE_BULLET_NUMBER_NAME")
	{
		return WORDS_TYPE_PLAYER_STATE_BULLET_NUMBER_NAME;
	}

	if (in_type_name == "WORDS_TYPE_PLAYER_STATE_TIME_NAME")
	{
		return WORDS_TYPE_PLAYER_STATE_TIME_NAME;
	}

	if (in_type_name == "WORDS_TYPE_SCORE_NAME")
	{
		return WORDS_TYPE_SCORE_NAME;
	}

	//defualt
	return WORDS_TYPE_MONSTER_ATTACK;
}
