#pragma once

class Point
{
public:
	Point();
	Point(int x, int y);

	void Set_X(int x);
	void Set_Y(int y);
	int Get_X() const;
	int Get_Y() const;

	/*Point& operator= (const Point& otherVector);*/
	Point& operator+ (const Point& otherVector);
	Point& operator+= (const Point& otherVector);
	Point& operator- (const Point& otherVector);
	Point& operator-= (const Point& otherVector);
	Point& operator* (const Point& otherVector);
	Point& operator*= (const Point& otherVector);
	Point& operator/ (const Point& otherVector);
	Point& operator/= (const Point& otherVector);

	void Move(int x, int y);

private:
	int m_XPos;
	int m_YPos;
};