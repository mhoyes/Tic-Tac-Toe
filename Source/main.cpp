#include <iostream>
#include "TicTacToe.h"

int main()
{
	TicTacToe game;

	if (game.Construct(800, 600, 1, 1))
	{
		game.Start();
	}

	return 0;
}
