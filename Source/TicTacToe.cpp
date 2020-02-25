// Required for the olcPixelGameEngine
#define OLC_PGE_APPLICATION

#include <time.h>	// To seed the rand()
#include "TicTacToe.h"
#include "Point.h"

TicTacToe::TicTacToe() : m_TotalGamesPlayed(0), m_NumOfPlayers(2), m_GameBoard_Rows(3), m_GameBoard_Cols(3)
{
	sAppName = "Tic-Tac-Toe";
}

bool TicTacToe::OnUserCreate()
{
	GenerateCells();
	GeneratePlayers();

	srand(time(NULL));

	InitializeBoardState();

	return true;
}

bool TicTacToe::OnUserUpdate(float fElapsedTime)
{
	// Clears the console back to Black before next update
	// In order to essentially refresh the console if Game Board changes
	Clear(BLACK);
	
	DrawGameBoard();

	if (WinnerSelected() || IsDraw())
	{
		if (GetKey(Key::ENTER).bPressed || GetMouse(0).bPressed)
		{
			InitializeBoardState();
		}
	}
	else if(GetMouse(0).bPressed)
	{
		OnCellClickedEvent();
	}

	return true;
}

bool TicTacToe::OnUserDestroy()
{
	// Clean up Player memory
	for (int i = 0; i < m_Players.size(); i++)
	{
		delete(m_Players[i]);
		m_Players[i] = nullptr;
	}

	m_Players.clear();

	// Clean up Game Board Cells memory
	for (int i = 0; i < m_BoardCells.size(); i++)
	{
		for (int j = 0; j < m_BoardCells[i].size(); j++)
		{
			delete(m_BoardCells[i][j]);
			m_BoardCells[i][j] = nullptr;
		}
	}

	m_BoardCells.clear();

	return true;
}

void TicTacToe::GenerateCells()
{
	Point offset = Point(50, 50);

	// Compute the cell size from the screen width and screen height, dividing them up by the rows and columns
	Point cellSize = Point((ScreenWidth() - (offset.Get_X() * 2)) / m_GameBoard_Rows, (ScreenHeight() - (offset.Get_X() * 2)) / m_GameBoard_Cols);

	Point position = offset;

	for (int i = 1; i <= m_GameBoard_Cols; i++)
	{
		std::vector<Cell*> cells;

		for (int j = 1; j <= m_GameBoard_Rows; j++)
		{
			cells.push_back(new Cell(position, cellSize));

			position.Move(cellSize.Get_X(), 0);

		}

		m_BoardCells.push_back(cells);

		position = offset;
		position.Move(0, cellSize.Get_Y() * i);
	}
}

void TicTacToe::GeneratePlayers()
{
	m_Players.push_back(new Player(CellState::Enum::X));
	m_Players.push_back(new Player(CellState::Enum::O));
}

void TicTacToe::DrawGameBoard()
{
	DrawGameInfo();

	for (int i = 0; i < m_BoardCells.size(); i++)
	{
		for (int j = 0; j < m_BoardCells[i].size(); j++)
		{
			const Point* topLeft = m_BoardCells[i][j]->GetPos();
			const Point* cellSize = m_BoardCells[i][j]->GetCellSize();

			DrawCell(topLeft, cellSize);

			if (!m_BoardCells[i][j]->IsCellOpen())
			{
				if (m_BoardCells[i][j]->GetCellState() == CellState::Enum::X)
				{
					DrawX(topLeft, cellSize);
				}
				else if (m_BoardCells[i][j]->GetCellState() == CellState::Enum::O)
				{
					DrawO(topLeft, cellSize);
				}
			}
		}
	}
}

void TicTacToe::DrawCell(const Point* topLeft, const Point* cellSize)
{
	DrawRect(topLeft->Get_X(), topLeft->Get_Y(), cellSize->Get_X(), cellSize->Get_Y(), WHITE);
}

void TicTacToe::DrawX(const Point* topLeft, const Point* cellSize)
{
	Point offset = Point(50, 25);
	DrawLine(topLeft->Get_X() + offset.Get_X(), topLeft->Get_Y() + offset.Get_Y(), topLeft->Get_X() + cellSize->Get_X() - offset.Get_X(), topLeft->Get_Y() + cellSize->Get_Y() - offset.Get_Y(), GREEN);
	DrawLine(topLeft->Get_X() + offset.Get_X(), topLeft->Get_Y() + cellSize->Get_Y() - offset.Get_Y(), topLeft->Get_X() + cellSize->Get_X() - offset.Get_X(), topLeft->Get_Y() + offset.Get_Y(), GREEN);
}

void TicTacToe::DrawO(const Point* topLeft, const Point* cellSize)
{
	int radius = cellSize->Get_Y() / 3;
	DrawCircle(topLeft->Get_X() + (cellSize->Get_X() / 2), topLeft->Get_Y() + (cellSize->Get_Y() / 2), radius, RED);
}

void TicTacToe::DrawGameInfo()
{
	Point pos = Point(ScreenWidth() / 2, 10);

	std::string currentPlayer = CellState::ToString(m_Players[m_CurrentPlayerIndex]->GetPlayerPiece());

	if (WinnerSelected() || IsDraw())
	{
		std::string str = WinnerSelected() ? "Winner is " + currentPlayer + "!"
										   : "Game Over! It's a Draw!";

		// Move the position away from the middle of the screen to try and center it
		pos.Move(-(int)(str.length() * 12), 0);
		DrawString(pos.Get_X(), pos.Get_Y(), str, WHITE, 3);

		// Move position again to draw the next string centered
		WinnerSelected() ? pos.Move(-20, 0) : pos.Move(80, 0);

		DrawString(pos.Get_X(), pos.Get_Y() + 30, "Press [Enter] key or Click Mouse to play again!", WHITE, 1);
	}
	else
	{
		std::string str = "Current Player: " + currentPlayer;

		pos.Move(-(int)(str.length() * 12), 0);
		DrawString(pos.Get_X(), pos.Get_Y(), str, WHITE, 3);
	}

	int oneQuarterScreenWidth = ScreenWidth() / 4;
	int playerScore_XPos = oneQuarterScreenWidth - 100;
	DrawString(playerScore_XPos, ScreenHeight() - 10, "Total Games: " + std::to_string(m_TotalGamesPlayed), WHITE);

	for (int i = 0; i < m_Players.size(); i++)
	{
		playerScore_XPos += (oneQuarterScreenWidth + 30);
		DrawString(playerScore_XPos, ScreenHeight() - 10, "Player " + CellState::ToString(m_Players[i]->GetPlayerPiece()) + " Wins: " + std::to_string(m_Players[i]->GetWinCount()), WHITE);
	}

}

void TicTacToe::InitializeBoardState()
{
	m_CurrentPlayerIndex = rand() % 2;

	m_WinnerSelected = false;
	m_IsDraw = false;
	m_TotalGamesPlayed++;

	for (int i = 0; i < m_BoardCells.size(); i++)
	{
		for (int j = 0; j < m_BoardCells[i].size(); j++)
		{
			m_BoardCells[i][j]->Reset();
		}
	}
}

void TicTacToe::OnCellClickedEvent()
{
	// Get mouse location this frame and return the clicked cell
	Cell* clickedCell = GetClickedCell(new Point(GetMouseX(), GetMouseY()));
	if (clickedCell != nullptr && clickedCell->IsCellOpen())
	{
		clickedCell->SelectCell(m_CurrentPlayerIndex);

		CalculateIfWon();

		if (!WinnerSelected())
		{
			ChangePlayer();
		}
		else
		{
			m_Players[m_CurrentPlayerIndex]->WonGame();
		}
	}
}

void TicTacToe::ChangePlayer()
{
	m_CurrentPlayerIndex++;

	if (m_CurrentPlayerIndex > m_NumOfPlayers - 1)
	{
		m_CurrentPlayerIndex = 0;
	}
}

void TicTacToe::CalculateIfWon()
{
	// Check for Draw by checking if all cells are closed
	bool openCellsRemain = false;
	for (int col = 0; col < m_BoardCells[0].size(); col++)
	{
		for (int row = 0; row < m_BoardCells[0].size(); row++)
		{
			if (m_BoardCells[col][row]->IsCellOpen())
			{
				openCellsRemain = true;
				break;
			}
		}
	}

	if (!openCellsRemain)
	{
		m_IsDraw = true;
		m_WinnerSelected = false;
	}
	else
	{
		m_IsDraw = false;

		// Check Diagonal
		if (m_BoardCells[0][0]->GetCellState() == m_BoardCells[1][1]->GetCellState() && m_BoardCells[1][1]->GetCellState() == m_BoardCells[2][2]->GetCellState() && m_BoardCells[2][2]->GetCellState() != CellState::Enum::Blank)
		{
			m_WinnerSelected = true;
		}
		// Check Reverse Diagonal
		else if (m_BoardCells[0][2]->GetCellState() == m_BoardCells[1][1]->GetCellState() && m_BoardCells[1][1]->GetCellState() == m_BoardCells[2][0]->GetCellState() && m_BoardCells[2][0]->GetCellState() != CellState::Enum::Blank)
		{
			m_WinnerSelected = true;
		}
		else
		{
			// Check for any Horizontal Wins per Row
			// Each row consists of [m_GameBoard_Cols] columns
			for (int i = 0; i < m_GameBoard_Cols; i++)
			{
				if (m_BoardCells[i][0]->GetCellState() != CellState::Enum::Blank)
				{
					if (m_BoardCells[i][0]->GetCellState() == m_BoardCells[i][1]->GetCellState() && m_BoardCells[i][1]->GetCellState() == m_BoardCells[i][2]->GetCellState())
					{
						m_WinnerSelected = true;
						return;
					}
				}
			}

			// Check for any Vertical Wins per Column
			// Each column consists of [m_GameBoard_Rows] rows
			for (int i = 0; i < m_GameBoard_Rows; i++)
			{
				if (m_BoardCells[0][i]->GetCellState() != CellState::Enum::Blank)
				{
					if (m_BoardCells[0][i]->GetCellState() == m_BoardCells[1][i]->GetCellState() && m_BoardCells[1][i]->GetCellState() == m_BoardCells[2][i]->GetCellState())
					{
						m_WinnerSelected = true;
						return;
					}
				}
			}
		}
	}
}

bool TicTacToe::WinnerSelected() const
{
	return m_WinnerSelected;
}

bool TicTacToe::IsDraw() const
{
	return m_IsDraw;
}

Cell* TicTacToe::GetClickedCell(Point *clickedPoint)
{
	Cell* clickedCell = nullptr;

	for (int i = 0; i < m_BoardCells.size(); i++)
	{
		for (int j = 0; j < m_BoardCells[i].size(); j++)
		{
			if (m_BoardCells[i][j]->IsPointWithinCell(clickedPoint))
			{
				return m_BoardCells[i][j];
			}
		}
	}

	return clickedCell;
}
