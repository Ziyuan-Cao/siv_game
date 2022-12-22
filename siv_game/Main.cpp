#include "Logic/game_logic.h"

extern s_logic* current_logic;

void Main()
{
	init_process::get_instance()->init_game();

	current_logic = main_logic::get_instance();
	while (current_logic)
	{
		current_logic->loop();
	}
}
