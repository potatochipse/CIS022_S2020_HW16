// Module:		BallClass.cpp
// Author:		Miguel Antonio Logarta
// Date:		April 18, 2020
// Purpose:		Implimentation file for BallClass
//				Create, move, delete ellipse
//

#include "framework.h"
#include "CIS023_S2020_HW16.h"

BallClass::BallClass(HWND hWnd)
{
	// get client window dimensions
	RECT rClient;
	GetClientRect(hWnd, &rClient);					// window

	// set start location
	rDim.left = rClient.right / 2;
	rDim.top = rClient.bottom / 2;

	// set ball size
	int dSize = GetRandomInt(10, 25);				// balls are round
	rDim.right = rDim.left + dSize;
	rDim.bottom = rDim.top + dSize;

	// set initial speed and direction, spit 'em out 
	do
	{
		rSpeed.x = GetRandomInt(-20, 20);			// left or right
	} while (rSpeed.x == 0);						// but not verticle
	do
	{
		rSpeed.y = GetRandomInt(-20, 20);			// up or down
	} while (rSpeed.y == 0);						// but not horizontal

	// pick a color
	r = GetRandomInt(0, 255);						// red
	g = GetRandomInt(0, 255);						// green
	b = GetRandomInt(0, 255);						// blue

}


void BallClass::Draw(HDC hdc)
{
	// create brushes
	HBRUSH brush, oldBrush;
	brush = CreateSolidBrush(RGB(r, g, b));
	oldBrush = (HBRUSH)SelectObject(hdc, brush);

	// draw spheare
	Ellipse(hdc, rDim.left, rDim.top, rDim.right, rDim.bottom);

	// release brushes
	SelectObject(hdc, oldBrush);
	DeleteObject(brush);
}

void BallClass::Move(HWND hWnd)
{
	// get client window dimensions
	RECT rClient;
	GetClientRect(hWnd, &rClient);					// window

	// check for collision
	if (rDim.left <= 0)								// did ball hit left wall
		rSpeed.x = abs(rSpeed.x);
	if (rDim.right >= rClient.right)				// did ball hit right wall
		rSpeed.x = abs(rSpeed.x) * -1.0;
	if (rDim.top <= 0)								// did ball hit top
		rSpeed.y = abs(rSpeed.y);
	if (rDim.bottom >= rClient.bottom)				// did ball hit bottom
		rSpeed.y = abs(rSpeed.y) * -1.0;

	// move according to speed
	rDim.top += rSpeed.y;
	rDim.left += rSpeed.x;
	rDim.bottom += rSpeed.y;
	rDim.right += rSpeed.x;
}

// Set the address to the next node
void BallClass::SetNext(BallClass* node)
{
	Next = node;
}

// Get the address to the next node
BallClass* BallClass::GetNext()
{
	return Next;
}

// return random integer between iMin and iMax
int BallClass::GetRandomInt(int iMin, int iMax)
{
	random_device rd;								// non-deterministic generator
	mt19937 gen(rd());								// to seed mersenne twister.
	uniform_int_distribution<> dist(iMin, iMax);	// distribute results inside center rect

	return dist(gen);								// return random # between iMin and iMax
}
