#include "Point.h"

Point::Point() : m_XPos(0), m_YPos(0) {}

Point::Point(int x, int y) : m_XPos(x), m_YPos(y) {}

void Point::Set_X(int x)
{
	m_XPos = x;
}

void Point::Set_Y(int y)
{
	m_YPos = y;
}

int Point::Get_X() const
{
	return m_XPos;
}

int Point::Get_Y() const
{
	return m_YPos;
}

//Point& Point::operator=(const Point& otherVector)
//{
//	m_XPos = otherVector.Get_X();
//	m_YPos = otherVector.Get_Y();
//
//	return *this;
//}

Point& Point::operator+(const Point& otherVector)
{
	m_XPos += otherVector.Get_X();
	m_YPos += otherVector.Get_Y();

	return *this;
}

Point& Point::operator+=(const Point& otherVector)
{
	return *this + otherVector;
}

Point& Point::operator-(const Point& otherVector)
{
	m_XPos -= otherVector.Get_X();
	m_YPos -= otherVector.Get_Y();

	return *this;
}

Point& Point::operator-=(const Point& otherVector)
{
	return *this - otherVector;
}

Point& Point::operator*(const Point& otherVector)
{
	m_XPos *= otherVector.Get_X();
	m_YPos *= otherVector.Get_Y();

	return *this;
}

Point& Point::operator*=(const Point& otherVector)
{
	return *this * otherVector;
}

Point& Point::operator/(const Point& otherVector)
{
	m_XPos /= otherVector.Get_X();
	m_YPos /= otherVector.Get_Y();

	return *this;
}

Point& Point::operator/=(const Point& otherVector)
{
	return *this / otherVector;
}

void Point::Move(int x, int y)
{
	m_XPos += x;
	m_YPos += y;
}
