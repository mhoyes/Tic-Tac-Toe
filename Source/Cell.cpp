#include "Cell.h"

Cell::Cell()
{
	m_Position = Point(0, 0);
	m_CellSize = Point(0, 0);
	m_IsCellOpen = true;
	m_CellState = CellState::Enum::Blank;
}

Cell::Cell(const Point topLeftPos, const Point cellSize)
{
	m_Position = topLeftPos;
	m_CellSize = cellSize;
	m_IsCellOpen = true;
	m_CellState = CellState::Enum::Blank;
}

const Point* Cell::GetPos() const
{
	return &m_Position;
}

const Point* Cell::GetCellSize() const
{
	return &m_CellSize;
}

bool Cell::IsPointWithinCell(const Point* point) const
{
	return  m_Position.Get_X() <= point->Get_X() &&
			m_Position.Get_Y() <= point->Get_Y() &&
			m_Position.Get_X() + m_CellSize.Get_X() >= point->Get_X() &&
			m_Position.Get_Y() + m_CellSize.Get_Y() >= point->Get_Y();
}

bool Cell::IsCellOpen() const
{
	return m_IsCellOpen;
}

CellState::Enum Cell::GetCellState() const
{
	return m_CellState;
}

void Cell::SelectCell(CellState::Enum state)
{
	m_IsCellOpen = false;

	m_CellState = state;
}

void Cell::Reset()
{
	m_IsCellOpen = true;
	m_CellState = CellState::Enum::Blank;
}
