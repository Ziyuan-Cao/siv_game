#include "game_logic.h"

s_logic* current_logic = nullptr;

void main_logic::loop()
{
	Window::Resize(1280, 720);

	Scene::SetBackground(ColorF{ 0.8, 0.9, 1.0 });

	// 通常のフォントを作成 | Create a font
	const Font font{ 60 };

	while (System::Update())
	{
		font(U"Hello, Siv3D!🚀").drawAt(Scene::Center(), Palette::Black);

		if (SimpleGUI::Button(U"start game", Vec2{ 640, 40 }))
		{
			current_logic = game_logic::get_instance();
			return;
		}
		if (SimpleGUI::Button(U"exit game ", Vec2{ 640, 100 }))
		{
			current_logic = nullptr;
			return;
		}
	}
	current_logic = nullptr;
}

void game_logic::loop()
{
	const ColorF backgroundColor = ColorF{ 0.4, 0.6, 0.8 }.removeSRGBCurve();
	const Texture uvChecker{ U"example/texture/uv.png", TextureDesc::MippedSRGB };
	const MSRenderTexture renderTexture{ Scene::Size(), TextureFormat::R8G8B8A8_Unorm_SRGB, HasDepth::Yes };

	BasicCamera3D camera_3d(renderTexture.size(), 40_deg, Vec3{ 10, 16, -32 });

	

	s_sence game_sence;
	game_sence.level_map->update_state();
	game_sence.camera_ptr = &camera_3d;

	game_sence.generate_monster_cmd = new generate_monster_command(&game_sence);
	game_sence.player->player_control_cmd = new player_control_command(game_sence.player, &game_sence);
	game_sence.player->generate_bullet_cmd = new generate_bullet_command(&game_sence, &camera_3d);

	s_timer::get_instance()->reset();
	s_timer::get_instance()->start();
	srand(s_timer::get_instance()->total_time());

	while (System::Update())
	{
		//Update event
		{
			s_timer::get_instance()->tick();

			if (game_sence.generate_monster_cmd)game_sence.generate_monster_cmd->execute();
			if (game_sence.player->player_control_cmd)game_sence.player->player_control_cmd->execute();
			for (auto it : game_sence.monster_group)
			{
				if (it->monster_behavior_tree_cmd) it->monster_behavior_tree_cmd->execute();
			}

			check_collision_command check_collision_cmd(&game_sence);
			check_collision_cmd.execute();

			//camera follow player
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

		// [3D rendering]
		{

			const ScopedRenderTarget3D target{ renderTexture.clear(backgroundColor) };// Clear renderTexture with the background color,// then make renderTexture the render target for the current 3D scene

			game_sence.level_map->draw();

			for (auto it : game_sence.monster_group)
			{
				it->update_state();
				it->draw();
			}

			for (auto it : game_sence.bullet_group)
			{
				it->update_state();
				it->draw();
			}

			for (auto it : game_sence.item_group)
			{
				it->update_state();
				it->draw();
			}

			game_sence.player->update_state();
			game_sence.player->draw();
		}

		// [2D rendering]
		{



			Graphics3D::Flush();// Flush 3D rendering commands before multisample resolve
			renderTexture.resolve();// Multisample resolve
			Shader::LinearToScreen(renderTexture);// Transfer renderTexture to the current 2D scene (default scene)

			refresh_text_command refresh_text_cmd(&game_sence);
			refresh_text_cmd.execute();
		}

		if (game_sence.player->hp < 0)
		{
			current_logic = game_over_logic::get_instance();
			return;
		}

	}

	//focus exit
	current_logic = nullptr;
}

void game_over_logic::loop()
{
	Scene::SetBackground(ColorF{ 0.6, 0.4, 0.2 });

	// 通常のフォントを作成 | Create a new font
	const Font font{ 60 };

	while (System::Update())
	{
		font(U"Game over!🚀").drawAt(Scene::Center(), Palette::Black);

		if (SimpleGUI::Button(U"return to main menu", Vec2{ 640, 40 }))
		{
			current_logic = main_logic::get_instance();
			return;
		}
	}

	//focus exit
	current_logic = nullptr;
}
