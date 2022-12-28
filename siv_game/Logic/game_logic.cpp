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
	//shadow
	struct light_data
	{
		Float3 light_color;
		Float3 light_direction;
		Mat4x4 shadow_transfrom;
	};

	const MSRenderTexture light_depth_texture(Scene::Size(), TextureFormat::R32_Float, HasDepth::Yes);
	BasicCamera3D light_camera{ light_depth_texture.size(), 30_deg, Vec3{ 100, 80, -160 } };
	const VertexShader shadow_vs = HLSL{ U"shader/shadow_shader.hlsl", U"VS" };
	const PixelShader shadow_ps = HLSL{ U"shader/shadow_shader.hlsl", U"PS" };
	//NDC (-1,1) -> (0,1)
	Mat4x4 T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);
	Mat4x4 shadow_tf = -light_camera.getView() * light_camera.getProj() * T;

	ConstantBuffer<light_data> shadow_data_cb{ {{ 1,0.9,0.8},-light_camera.getEyePosition().normalized(),shadow_tf} };

	//game forward light
	const ColorF background_color = ColorF{ 0.4, 0.6, 0.8 }.removeSRGBCurve();
	const MSRenderTexture final_render_target{ Scene::Size(), TextureFormat::R8G8B8A8_Unorm_SRGB, HasDepth::Yes };

	BasicCamera3D game_camera(final_render_target.size(), 40_deg, Vec3{ 10, 16, -32 });
	const VertexShader game_forward_light_vs = HLSL{ U"shader/game_forward_light.hlsl", U"VS" };
	const PixelShader game_forward_light_ps = HLSL{ U"shader/game_forward_light.hlsl", U"PS" };

	//init game sence
	s_sence game_sence;
	game_sence.level_map->update_state();
	game_sence.camera_ptr = &game_camera;
	game_sence.remain_time = 30;
	game_sence.generate_item_cmd = new generate_item_command(&game_sence);
	game_sence.generate_monster_cmd = new generate_monster_command(&game_sence);
	game_sence.player->player_control_cmd = new player_control_command(game_sence.player, &game_sence);



	//reset time
	s_timer::get_instance()->reset();
	s_timer::get_instance()->start();


	System::Update();
	shadow_data_cb->light_color = { 1,0.9,0.8 };
	shadow_data_cb->light_direction = -light_camera.getEyePosition().normalized();
	shadow_data_cb->shadow_transfrom = shadow_tf;


	while (System::Update())
	{
		//Update event
		{
			//time float
			s_timer::get_instance()->tick();

			//Mat4x4::Rotate(Vec3{ 0,1,0 }, (s_timer::get_instance()->detail_time() * 10_deg),);


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
			game_camera.setView(
				Vec3{ 10, 16, -32 } +
				Vec3{ game_sence.player->position[0],
				game_sence.player->position[1],
				game_sence.player->position[2] },
				Vec3{ game_sence.player->position[0],
				game_sence.player->position[1],
				game_sence.player->position[2] });
		}

		//shadow
		{
			Graphics3D::SetCameraTransform(light_camera);

			const ScopedCustomShader3D light_shader{ shadow_vs, shadow_ps };
			const ScopedRenderTarget3D light_target{ light_depth_texture.clear({1,1,1}) };

			//draw map
			game_sence.level_map->draw();
			//draw monster
			for (auto it : game_sence.monster_group)
			{
				it->update_state();
				it->draw();
			}
			//draw monster
			for (auto it : game_sence.bullet_group)
			{
				it->update_state();
				it->draw();
			}
			//draw monster
			for (auto it : game_sence.item_group)
			{

				it->update_state();
				it->draw();
			}

			//draw player
			game_sence.player->update_state();
			game_sence.player->draw();

			Graphics3D::Flush();
			light_depth_texture.resolve();
		}
		//forwar light
		{
			Graphics3D::SetCameraTransform(game_camera);
			const ScopedCustomShader3D shader{ game_forward_light_vs, game_forward_light_ps };

			const ScopedRenderTarget3D target{ final_render_target.clear(background_color) };
			Graphics3D::SetVSConstantBuffer(4, shadow_data_cb);
			Graphics3D::SetPSConstantBuffer(4, shadow_data_cb);
			Graphics3D::SetPSTexture(1, light_depth_texture);
			const ScopedRenderStates3D ss{ { ShaderStage::Pixel, 1, SamplerState::BorderAniso} };

			//draw map
			game_sence.level_map->draw();
			//draw monster
			for (auto it : game_sence.monster_group)
			{
				it->update_state();
				it->draw();
			}
			//draw monster
			for (auto it : game_sence.bullet_group)
			{
				it->update_state();
				it->draw();
			}
			//draw monster
			for (auto it : game_sence.item_group)
			{

				it->update_state();
				it->draw();
			}

			//draw player
			game_sence.player->update_state();
			game_sence.player->draw();


			Graphics3D::Flush();
			final_render_target.resolve();
		}

		//UI
		{
			Shader::LinearToScreen(final_render_target);// Transfer renderTexture to the current 2D scene (default scene)

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
		if (SimpleGUI::Button(factory_ptr->get_words_siv_string(WORDS_TYPE_RETURN_MAIN_MENU_BUTTON_NAME), Vec2{ 1150, 50 }))
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
