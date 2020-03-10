#pragma once
#include "olcPixelGameEngine.h"
#include "Player.h"
#include "Cell.h"
#include "WinningLine.h"
#include <vector>

using namespace olc;

class TicTacToe: public PixelGameEngine
{
public:
	TicTacToe();

	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
	bool OnUserDestroy() override;

private:
	void GenerateCells();
	void GeneratePlayers();

	void DrawGameBoard();
	void DrawCell(const Point* topLeft, const Point* cellSize);
	void DrawX(const Point* topLeft, const Point* cellSize);
	void DrawO(const Point* topLeft, const Point* cellSize);
	void DrawGameInfo();
	void DrawWinningLine();

	void InitializeBoardState();
	void OnCellClickedEvent();
	void ChangePlayer();
	void CalculateIfWon();
	bool WinnerSelected() const;
	bool IsDraw() const;

	Cell* GetClickedCell(Point *clickedPoint);

private:
	std::vector<Player*> m_Players;

	std::vector<std::vector<Cell*>> m_BoardCells;
	std::vector<Cell*> m_WinningCells;

	const int m_GameBoard_Rows;
	const int m_GameBoard_Cols;
	const int m_NumOfPlayers;

	int m_CurrentPlayerIndex;
	int m_TotalGamesPlayed;

	bool m_WinnerSelected;
	bool m_IsDraw;

	WinningLine m_WinningLine;
};
