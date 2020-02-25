#pragma once
#include <string>

class CellState
{
public:
	enum class Enum
	{
		X = 0,
		O,
		Blank
	};

	static std::string ToString(CellState::Enum state)
	{
		switch (state)
		{
		case CellState::Enum::X:
			return std::string("X's");

		case CellState::Enum::O:
			return std::string("O's");

		default:
			return std::string("Unknown Player");
		}
	}
};