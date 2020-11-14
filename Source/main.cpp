#include <iostream>
#include "TicTacToe.h"
using namespace std;

int main()
{
	cout << "=======================\n";
	cout << "Welcome to Tic-Tac-Toe!\n";
	cout << "=======================\n\n";

	int numOfPlayers = 0;

	while (numOfPlayers < 1 || numOfPlayers > 2)
	{
		cout << "Enter \"1\" for 1-Player vs AI " << endl;
		cout << "Enter \"2\" for 2-Player\n\n";

		cout << "Option: ";
		cin >> numOfPlayers;

		if (numOfPlayers != 1 && numOfPlayers != 2)
		{
			system("CLS");
			cout << "Invalid option. Please choose an option!\n\n";
		}
	}

	system("CLS");

	cout << "Generating Tic-Tac-Toe Game Board now.\n\nPlease wait...\n\n";

	TicTacToe game = TicTacToe(2, numOfPlayers == 1);
	if (game.Construct(800, 600, 1, 1))
	{
		game.Start();
	}

	return 0;
}
