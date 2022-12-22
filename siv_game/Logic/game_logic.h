#pragma once
# include <Siv3D.hpp>
#include "Command/concrete_command.h"
#include "Object/s_timer.h"
#include "s_math.h"

class s_logic
{
public:
	virtual void loop() = 0;
};

class init_process
{
private:
	init_process() {};
public:
	static init_process* get_instance()
	{
		static init_process init_process_instance;
		return &init_process_instance;
	}

	void init_game();
};

class main_logic : public s_logic
{
private:
	main_logic() {};
public:
	static main_logic* get_instance()
	{
		static main_logic main_logic_instance;
		return &main_logic_instance;
	}
	virtual void loop() override;
};

class game_logic : public s_logic
{
private:
	game_logic() {};
public:
	static game_logic* get_instance()
	{
		static game_logic game_logic_instance;
		return &game_logic_instance;
	}
	virtual void loop() override;
};

class game_over_logic : public s_logic
{
private:
	game_over_logic() {};
public:
	int score = 0;
	static game_over_logic* get_instance()
	{
		static game_over_logic game_over_logic_instance;
		game_over_logic_instance.score = 0;
		return &game_over_logic_instance;
	}
	virtual void loop() override;
};
