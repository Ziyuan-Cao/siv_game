#include "Logic/game_logic.h"

extern s_logic* current_logic;

void Main()
{
	current_logic = main_logic::get_instance();
	while (current_logic)
	{
		current_logic->loop();
	}
}
