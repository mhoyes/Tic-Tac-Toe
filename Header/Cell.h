#pragma once
#include "Point.h"
#include "CellState.h"

class Cell
{
public:
	Cell();
	Cell(const Point position, const Point cellSize);

	const Point* GetPos() const;
	const Point* GetCellSize() const;

	bool IsPointWithinCell(const Point* point) const;
	bool IsCellOpen() const;

	CellState::Enum GetCellState() const;

	void SelectCell(int currentPlayerIndex);

	void Reset();

private:
	Point m_Position;
	Point m_CellSize;

	CellState::Enum m_CellState;

	bool m_IsCellOpen;
};

