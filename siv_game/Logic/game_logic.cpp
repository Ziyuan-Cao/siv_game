#include "game_logic.h"

s_logic* current_logic = nullptr;
LANGUAGE_TYPE current_language = LANGUAGE_TYPE_JAPANESE;

void init_process::init_game()
{
	s_factory* factory_ptr = s_factory::get_instance();
	factory_ptr->load_object_data();
	factory_ptr->load_words(current_language);
}

void main_logic::loop()
{
	s_factory* factory_ptr = s_factory::get_instance();

	Window::Resize(1280, 720);

	Scene::SetBackground(ColorF{ 0.8, 0.9, 1.0 });

	// 通常のフォントを作成 | Create a font
	const Font font{ 60 };

	while (System::Update())
	{
		font(factory_ptr->get_words_siv_string(WORDS_TYPE_GAME_TITLE_NAME)).drawAt(Scene::Center(), Palette::Black);

		if (SimpleGUI::Button(factory_ptr->get_words_siv_string(WORDS_TYPE_START_GAME_BUTTON_NAME), Vec2{ 600, 440 }))
		{
			current_logic = game_logic::get_instance();
			return;
		}
		if (SimpleGUI::Button(factory_ptr->get_words_siv_string(WORDS_TYPE_LANGUAGE_NAME), Vec2{ 600, 500 }))
		{
			current_language = (LANGUAGE_TYPE)((current_language + 1) % LANGUAGE_TYPE_NUMBER);
			factory_ptr->load_words(current_language);
		}
		if (SimpleGUI::Button(factory_ptr->get_words_siv_string(WORDS_TYPE_EXIT_GAME_BUTTON_NAME), Vec2{ 600, 560 }))
		{
			current_logic = nullptr;
			return;
		}

	}
	current_logic = nullptr;
}

void game_logic::loop()
{
	s_factory* factory_ptr = s_factory::get_instance();

	//init gpu
	const ColorF backgroundColor = ColorF{ 0.4, 0.6, 0.8 }.removeSRGBCurve();
	const MSRenderTexture renderTexture{ Scene::Size(), TextureFormat::R8G8B8A8_Unorm_SRGB, HasDepth::Yes };

	BasicCamera3D camera_3d(renderTexture.size(), 40_deg, Vec3{ 10, 16, -32 });

	//init game sence
	s_sence game_sence;
	game_sence.level_map->update_state();
	game_sence.camera_ptr = &camera_3d;
	game_sence.remain_time = 30;
	game_sence.generate_item_cmd = new generate_item_command(&game_sence);
	game_sence.generate_monster_cmd = new generate_monster_command(&game_sence);
	game_sence.player->player_control_cmd = new player_control_command(game_sence.player, &game_sence);
	

	//reset time
	s_timer::get_instance()->reset();
	s_timer::get_instance()->start();
	

	while (System::Update())
	{
		//Update event
		{
			//time float
			s_timer::get_instance()->tick();

			//remain time--
			game_sence.remain_time -= s_timer::get_instance()->detail_time();

			//generate monster
			if (game_sence.generate_monster_cmd && s_timer::get_instance()->total_time() > game_sence.prev_monster_generate_time + game_sence.monster_cooler_time)
			{
				game_sence.prev_monster_generate_time = s_timer::get_instance()->total_time();

				game_sence.generate_monster_cmd->execute();

				int time_hard = s_timer::get_instance()->total_time() * 10;
				time_hard %= game_sence.monster_period;

				for (int i = 0; i < time_hard; i++)
				{
					game_sence.generate_monster_cmd->execute();
				}
			}

			//generate item
			if (game_sence.generate_item_cmd && s_timer::get_instance()->total_time() > game_sence.prev_item_generate_time + game_sence.item_cooler_time)
			{
				game_sence.prev_item_generate_time = s_timer::get_instance()->total_time();

				game_sence.generate_item_cmd->execute();
			}

			//player move
			if (game_sence.player->player_control_cmd)game_sence.player->player_control_cmd->execute();

			//monster move
			for (auto it : game_sence.monster_group)
			{
				if (it->monster_behavior_tree_cmd) it->monster_behavior_tree_cmd->execute();
			}

			//collision check
			check_collision_command check_collision_cmd(&game_sence);
			check_collision_cmd.execute();

			//camera move (follow player)
			camera_3d.setView(
				Vec3{ 10, 16, -32 } +
				Vec3{ game_sence.player->position[0],
				game_sence.player->position[1],
				game_sence.player->position[2] },
				Vec3{ game_sence.player->position[0],
				game_sence.player->position[1],
				game_sence.player->position[2] });
			Graphics3D::SetCameraTransform(camera_3d);
		}

		//3d rendering
		{
			const ScopedRenderTarget3D target{ renderTexture.clear(backgroundColor) };// Clear renderTexture with the background color,// then make renderTexture the render target for the current 3D scene

			game_sence.level_map->draw();
			//update monster
			for (auto it : game_sence.monster_group)
			{
				it->update_state();
				it->draw();
			}
			//update bullet
			for (auto it : game_sence.bullet_group)
			{
				it->update_state();
				it->draw();
			}
			//update item
			for (auto it : game_sence.item_group)
			{

				it->update_state();
				it->draw();
			}

			//update player
			game_sence.player->update_state();
			game_sence.player->draw();
		}

		//2d rendering
		{
			Graphics3D::Flush();// Flush 3D rendering commands before multisample resolve
			renderTexture.resolve();// Multisample resolve
			Shader::LinearToScreen(renderTexture);// Transfer renderTexture to the current 2D scene (default scene)

			//update player state
			display_player_state_command display_player_state_cmd(&game_sence);
			display_player_state_cmd.execute();

			//update text
			refresh_text_command refresh_text_cmd(&game_sence);
			refresh_text_cmd.execute();
		}

		//check game over
		if (game_sence.player->hp < 0 || game_sence.remain_time <= 0)
		{
			auto logic_ptr = game_over_logic::get_instance();
			logic_ptr->score = game_sence.score;
			current_logic = logic_ptr;
			return;
		}

		//check button
		if (SimpleGUI::Button(factory_ptr->get_words_siv_string(WORDS_TYPE_RETURN_MAIN_MENU_BUTTON_NAME), Vec2{ 1100, 20 }))
		{
			current_logic = main_logic::get_instance();
			return;
		}
	}

	//focus exit
	current_logic = nullptr;
}

void game_over_logic::loop()
{
	s_factory* factory_ptr = s_factory::get_instance();

	Scene::SetBackground(ColorF{ 0.3, 0.2, 0.1 });

	// 通常のフォントを作成 | Create a new font
	const Font font{ 60 };

	while (System::Update())
	{
		font(factory_ptr->get_words_siv_string(WORDS_TYPE_GAME_OVER_TITLE_NAME)).drawAt(Scene::Center(), Palette::Black);

		font(factory_ptr->get_words_siv_string(WORDS_TYPE_SCORE_NAME) + U":" + Unicode::Widen(std::to_string(score))).drawAt(Vec2{ 640, 440 }, Palette::Black);

		if (SimpleGUI::Button(factory_ptr->get_words_siv_string(WORDS_TYPE_RETURN_MAIN_MENU_BUTTON_NAME), Vec2{ 600, 600 }))
		{
			current_logic = main_logic::get_instance();
			return;
		}
	}

	//focus exit
	current_logic = nullptr;
}
