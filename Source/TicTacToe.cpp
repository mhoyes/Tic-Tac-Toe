// Required for the olcPixelGameEngine
#define OLC_PGE_APPLICATION

#include <time.h>	// To seed the rand()
#include "TicTacToe.h"
#include "Point.h"

TicTacToe::TicTacToe(int numOfPlayers, bool aiEnabled)
	: m_TotalGamesPlayed(0)
	, m_AIPlayerActive(aiEnabled)
	, m_AIsTurn(false)
	, m_AITurnsElapsedTime(0.0f)
	, m_AITurnsSimulatedTurnTime(0.0f)
	, m_NumOfPlayers(numOfPlayers)
	, m_GameBoard_Rows(3)
	, m_GameBoard_Cols(3)
{
	sAppName = "Tic-Tac-Toe";
}

bool TicTacToe::OnUserCreate()
{
	srand(time(NULL));

	GenerateCells();
	GeneratePlayers();

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
	// AIPlayer is active and current index isn't Player 1.
	// Must be AIs turn
	else if (m_AIPlayerActive && m_CurrentPlayerIndex != 0)
	{
		// Track the elapsed time the AI has had it's turn
		m_AITurnsElapsedTime += fElapsedTime;

		// Turn Time hasn't been calculated. Let's calculate it.
		if (m_AITurnsSimulatedTurnTime == 0.0f)
		{
			float min = 0.5f;
			float max = 2.5f;

			// Get a random time between min and max
			m_AITurnsSimulatedTurnTime = (rand() / static_cast<float>(RAND_MAX * max - 1) + min);
		}

		// Simulate thinking by waiting until the specified time has elapsed before making a move.
		if (m_AITurnsElapsedTime >= m_AITurnsSimulatedTurnTime)
		{
			OnAIsTurn();
		}
		else
		{
			m_AIsTurn = true;
		}
	}
	// Must be a players turn
	else
	{
		if (GetMouse(0).bPressed)
		{
			OnCurrentPlayersTurn();
		}
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

	for (int i = 1; i <= m_GameBoard_Rows; i++)
	{
		std::vector<Cell*> cells;

		for (int j = 1; j <= m_GameBoard_Cols; j++)
		{
			cells.push_back(new Cell(position, cellSize));

			position.Move(0, cellSize.Get_Y());

		}

		m_BoardCells.push_back(cells);

		position = offset;
		position.Move(cellSize.Get_X() * i, 0);
	}
}

void TicTacToe::GeneratePlayers()
{
	int num = rand() % 2;

	CellState::Enum player1CellState = (CellState::Enum)num;

	m_Players.push_back(new Player(player1CellState));

	int otherPlayerVal = (num == 0) ? num+1 : num-1;
	
	CellState::Enum otherPlayerCellState = (CellState::Enum)otherPlayerVal;

	m_Players.push_back(new Player(otherPlayerCellState));
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

			topLeft = nullptr;
			cellSize = nullptr;
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
		std::string player = "Player";

		if (m_CurrentPlayerIndex == 1 && m_AIPlayerActive)
		{
			player = "AI";
		}

		std::string str = WinnerSelected() ? "Winner is " + player + " (" + currentPlayer + ")!"
										   : "Game Over! It's a Draw!";

		// Move the position away from the middle of the screen to try and center it
		pos.Move(-(int)(str.length() * 12), 0);
		DrawString(pos.Get_X(), pos.Get_Y(), str, WHITE, 3);

		// Move position again to draw the next string centered
		WinnerSelected() ? pos.Move(-20, 0) : pos.Move(80, 0);

		DrawString(pos.Get_X(), pos.Get_Y() + 30, "Press [Enter] key or Click Mouse to play again!", WHITE, 1);

		DrawWinningLine();
	}
	else
	{
		std::string aiStr = "AIs Turn: ";
		std::string playerStr = "Players Turn: ";
		std::string str = (m_AIsTurn ? aiStr : playerStr) + currentPlayer;

		pos.Move(-(int)(str.length() * 12), 0);
		DrawString(pos.Get_X(), pos.Get_Y(), str, WHITE, 3);
	}

	int oneQuarterScreenWidth = ScreenWidth() / 4;
	int playerScore_XPos = oneQuarterScreenWidth - 100;
	DrawString(playerScore_XPos, ScreenHeight() - 10, "Total Games: " + std::to_string(m_TotalGamesPlayed), WHITE);

	for (int i = 0; i < m_Players.size(); i++)
	{
		playerScore_XPos += (oneQuarterScreenWidth + 30);
		std::string player = "Player";

		if (i == 1 && m_AIPlayerActive)
		{
			player = "AI";
		}

		std::string score = player + " (" + CellState::ToString(m_Players[i]->GetPlayerPiece()) + ") Wins: " + std::to_string(m_Players[i]->GetWinCount());
		DrawString(playerScore_XPos, ScreenHeight() - 10, score, WHITE);
	}

}

void TicTacToe::DrawWinningLine()
{
	if (m_WinningLine == WinningLine::None)
		return;
	
	const Point* startPos = m_WinningCells[0]->GetPos();
	const Point* startCellSize = m_WinningCells[0]->GetCellSize();
	const Point* endPos = m_WinningCells[1]->GetPos();
	const Point* endCellSize = m_WinningCells[1]->GetCellSize();

	int xPadding = 30, yPadding = 30;
	
	if (m_WinningLine == WinningLine::Diagonal)
	{
		DrawLine(startPos->Get_X() + xPadding, startPos->Get_Y() + yPadding, endPos->Get_X() + endCellSize->Get_X() - xPadding, endPos->Get_Y() + endCellSize->Get_Y() - yPadding, WHITE);
	}
	else if (m_WinningLine == WinningLine::Diagonal_Reversed)
	{
		DrawLine(startPos->Get_X() + xPadding, startPos->Get_Y() + startCellSize->Get_Y()- yPadding, endPos->Get_X() + endCellSize->Get_X() - xPadding, endPos->Get_Y() + yPadding, WHITE);
	}
	else if (m_WinningLine == WinningLine::Horizontal)
	{
		DrawLine(startPos->Get_X() + xPadding, startPos->Get_Y() + startCellSize->Get_Y() / 2, endPos->Get_X() + endCellSize->Get_X() - xPadding, endPos->Get_Y() + endCellSize->Get_Y()/2, WHITE);
	}
	else if (m_WinningLine == WinningLine::Vertical)
	{
		DrawLine(startPos->Get_X() + startCellSize->Get_X() / 2, startPos->Get_Y() + yPadding, endPos->Get_X() + endCellSize->Get_X() / 2, endPos->Get_Y() + endCellSize->Get_Y() - yPadding, WHITE);
	}

	startPos = nullptr;
	startCellSize = nullptr;
	endPos = nullptr;
	endCellSize = nullptr;
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

	m_WinningLine = WinningLine::None;
	m_WinningCells.clear();
}

void TicTacToe::OnCurrentPlayersTurn()
{
	// Get mouse location this frame and return the clicked cell
	Cell* clickedCell = GetClickedCell(new Point(GetMouseX(), GetMouseY()));
	if (clickedCell != nullptr && clickedCell->IsCellOpen())
	{
		clickedCell->SelectCell(m_Players[m_CurrentPlayerIndex]->GetPlayerPiece());

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

void TicTacToe::OnAIsTurn()
{
	m_AITurnsElapsedTime = 0.0f;
	m_AITurnsSimulatedTurnTime = 0.0f;

	bool foundOpenCell = false;
	
	int boardSizeX = m_BoardCells[0].size();
	int boardSizeY = m_BoardCells[0].size();

	while (!foundOpenCell)
	{
		int randRow = rand() % m_GameBoard_Rows;
		int randCol = rand() % m_GameBoard_Cols;

		// Search for a random open cell
		Cell* chosenCell = m_BoardCells[randRow][randCol];
		if (chosenCell != nullptr && chosenCell->IsCellOpen())
		{
			foundOpenCell = true;
			chosenCell->SelectCell(m_Players[m_CurrentPlayerIndex]->GetPlayerPiece());

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

	m_AIsTurn = false;
}

void TicTacToe::ChangePlayer()
{
	m_CurrentPlayerIndex++;

	if (m_CurrentPlayerIndex > m_NumOfPlayers - 1)
	{
		m_CurrentPlayerIndex = 0;
	}
}

bool TicTacToe::AnyCellsOpen()
{
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

	return openCellsRemain;
}

void TicTacToe::CalculateIfWon()
{
	// Check for Draw by checking if all cells are closed
	bool openCellsRemain = AnyCellsOpen();

	// Check Diagonal
	if (m_BoardCells[0][0]->GetCellState() == m_BoardCells[1][1]->GetCellState() && m_BoardCells[1][1]->GetCellState() == m_BoardCells[2][2]->GetCellState() && m_BoardCells[2][2]->GetCellState() != CellState::Enum::Blank)
	{
		m_WinnerSelected = true;
		m_WinningCells.push_back(m_BoardCells[0][0]);
		m_WinningCells.push_back(m_BoardCells[2][2]);
		m_WinningLine = WinningLine::Diagonal;
		return;
	}
	// Check Reverse Diagonal
	else if (m_BoardCells[0][2]->GetCellState() == m_BoardCells[1][1]->GetCellState() && m_BoardCells[1][1]->GetCellState() == m_BoardCells[2][0]->GetCellState() && m_BoardCells[2][0]->GetCellState() != CellState::Enum::Blank)
	{
		m_WinnerSelected = true;
		m_WinningCells.push_back(m_BoardCells[0][2]);
		m_WinningCells.push_back(m_BoardCells[2][0]);
		m_WinningLine = WinningLine::Diagonal_Reversed;
		return;
	}
	else
	{
		// Check for any Horizontal Wins per Row
		// Each row consists of [m_GameBoard_Cols] columns

		for (int i = 0; i < m_GameBoard_Cols; i++)
		{
			if (m_BoardCells[0][i]->GetCellState() != CellState::Enum::Blank)
			{
				if (m_BoardCells[0][i]->GetCellState() == m_BoardCells[1][i]->GetCellState() && m_BoardCells[1][i]->GetCellState() == m_BoardCells[2][i]->GetCellState())
				{
					m_WinnerSelected = true;
					m_WinningCells.push_back(m_BoardCells[0][i]);
					m_WinningCells.push_back(m_BoardCells[2][i]);
					m_WinningLine = WinningLine::Horizontal;
					return;
				}
			}
		}

		// Check for any Vertical Wins per Column
		// Each column consists of [m_GameBoard_Rows] rows
		for (int i = 0; i < m_GameBoard_Rows; i++)
		{
			if (m_BoardCells[i][0]->GetCellState() != CellState::Enum::Blank)
			{
				if (m_BoardCells[i][0]->GetCellState() == m_BoardCells[i][1]->GetCellState() && m_BoardCells[i][1]->GetCellState() == m_BoardCells[i][2]->GetCellState())
				{
					m_WinnerSelected = true;
					m_WinningCells.push_back(m_BoardCells[i][0]);
					m_WinningCells.push_back(m_BoardCells[i][2]);
					m_WinningLine = WinningLine::Vertical;
					return;
				}
			}
		}
	}

	// Made it here without WinnerSelected, so if no cells are open, it must be a draw
	m_IsDraw = !openCellsRemain;
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
