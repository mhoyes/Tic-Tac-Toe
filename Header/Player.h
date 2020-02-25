#pragma once
#include "CellState.h"

class Player
{
public:
	Player();
	Player(CellState::Enum gamePieceType);
	
	CellState::Enum GetPlayerPiece();
	void WonGame();

	int GetWinCount() const;

private:
	CellState::Enum m_GamePieceType;
	int m_NumOfGamesWon;
};

