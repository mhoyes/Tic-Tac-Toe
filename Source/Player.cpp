#include "Player.h"

Player::Player()
{
	m_GamePieceType = CellState::Enum::Blank;
	m_NumOfGamesWon = 0;
}

Player::Player(CellState::Enum gamePieceType)
{
	m_GamePieceType = gamePieceType;
	m_NumOfGamesWon = 0;
}

CellState::Enum Player::GetPlayerPiece()
{
	return m_GamePieceType;
}

void Player::WonGame()
{
	m_NumOfGamesWon++;
}

int Player::GetWinCount() const
{
	return m_NumOfGamesWon;
}
