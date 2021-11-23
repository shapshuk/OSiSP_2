#pragma once
#include <windows.h>
#include <cmath>

struct Properties
{
	FLOAT x;
	FLOAT y;

	FLOAT width;
	FLOAT height;

	FLOAT rebound;
	FLOAT speed;

	BOOL isImage;
};

class Sprite
{
private:
	HDC winDC;
	HDC memDC;

	HBITMAP hBitmap;
	HBITMAP hPrevBitmap;

	PAINTSTRUCT ps;
	RECT clientRect;
	Properties properties;

	BITMAP bm;
	HDC spriteDC;
	HBITMAP image;

	void DrawRectangle(HWND hWnd);
	void DrawSprite(HWND hWnd);

public:
	Sprite(HWND hWnd, RECT clientRect);
	~Sprite();

	void InitProperties(FLOAT x, FLOAT y, FLOAT width, FLOAT height, FLOAT speed, FLOAT rebound);

	void MakeAStep(CHAR key);
	void MakeABounce();
	void Move(POINT mouseCoords);
	
	void SetARect(RECT clientRect);
	void ChangeAState();
	
	void Draw(HWND hWnd);
};
