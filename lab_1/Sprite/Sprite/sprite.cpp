#include "sprite.h"

Sprite::Sprite(HWND hWnd, RECT clientRect)
{
	winDC = GetDC(hWnd);
	memDC = CreateCompatibleDC(winDC);

	this->clientRect = clientRect;
	hBitmap = CreateCompatibleBitmap(winDC, clientRect.right, clientRect.bottom);
	hPrevBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

	image = (HBITMAP)LoadImage(0, L"ghost.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GetObject(image, sizeof(BITMAP), &bm);
	spriteDC = CreateCompatibleDC(winDC);
	SelectObject(spriteDC, image);
}

Sprite::~Sprite()
{
	DeleteObject((HBITMAP)image);
	DeleteObject(hPrevBitmap);
	DeleteObject(hBitmap);
	DeleteDC(spriteDC);
	DeleteDC(memDC);
	DeleteDC(winDC);
}

void Sprite::DrawRectangle(HWND hWnd)
{
	BeginPaint(hWnd, &ps);
	PatBlt(memDC, 0, 0, clientRect.right, clientRect.bottom, WHITENESS);
	SelectObject(memDC, CreateSolidBrush(RGB(18, 188, 156)));
	Rectangle(memDC, (INT)properties.x, (INT)properties.y, (INT)properties.x + (INT)properties.width, (INT)properties.y + (INT)properties.height);	
	BitBlt(winDC, 0, 0, clientRect.right, clientRect.bottom, memDC, 0, 0, SRCCOPY);
	EndPaint(hWnd, &ps);
}

void Sprite::DrawSprite(HWND hWnd)
{
	BeginPaint(hWnd, &ps);
	PatBlt(memDC, 0, 0, clientRect.right, clientRect.bottom, WHITENESS);
	BitBlt(memDC, (INT)properties.x, (INT)properties.y, bm.bmWidth, bm.bmHeight, spriteDC, 0, 0, SRCCOPY);
	BitBlt(winDC, 0, 0, clientRect.right, clientRect.bottom, memDC, 0, 0, SRCCOPY);
	EndPaint(hWnd, &ps);
}

void Sprite::InitProperties(FLOAT x, FLOAT y, FLOAT width, FLOAT height, FLOAT speed, FLOAT rebound)
{
	properties.x = x;
	properties.y = y;
	properties.width = width;
	properties.height = height;
	properties.speed = speed;
	properties.rebound = rebound;
	properties.isImage = false;
}

void Sprite::MakeAStep(CHAR key)
{
	switch (key)
	{
	case 'W':
		properties.y -= properties.speed;
		break;
	case 'S':
		properties.y += properties.speed;
		break;
	case 'A':
		properties.x -= properties.speed;
		break;
	case 'D':
		properties.x += properties.speed;
		break;
	}

	MakeABounce();
}

void Sprite::MakeABounce()
{
	if (properties.y < clientRect.top)
	{
		properties.y = clientRect.top + properties.rebound;
	}
	else if (properties.y + properties.height > clientRect.bottom)
	{
		properties.y = clientRect.bottom - properties.height - properties.rebound;
	}
	else if (properties.x < clientRect.left)
	{
		properties.x = clientRect.left + properties.rebound;
	}
	else if (properties.x + properties.width > clientRect.right)
	{
		properties.x = clientRect.right - properties.width - properties.rebound;
	}
}

void Sprite::Move(POINT mouseCoords)
{
	mouseCoords.x -= properties.width / 2;
	mouseCoords.y -= properties.height / 2;
	FLOAT distance = sqrt((mouseCoords.x - properties.x) * (mouseCoords.x - properties.x) + 
						  (mouseCoords.y - properties.y) * (mouseCoords.y - properties.y));

	if (distance < 5.0f)
	{
		properties.x = mouseCoords.x;
		properties.y = mouseCoords.y;
	}
	else
	{
		properties.x += properties.speed * 0.7f * (mouseCoords.x - properties.x) / distance;
		properties.y += properties.speed * 0.7f * (mouseCoords.y - properties.y) / distance;
	}

	MakeABounce();
}

void Sprite::SetARect(RECT clientRect)
{
	this->clientRect = clientRect;
}

void Sprite::ChangeAState()
{
	properties.isImage = !properties.isImage;
}

void Sprite::Draw(HWND hWnd)
{
	if (properties.isImage)
	{
		properties.width = bm.bmWidth;
		properties.height = bm.bmHeight;
		MakeABounce();
		DrawSprite(hWnd);
	}
	else
	{
		properties.width = 80.0f;
		properties.height = 60.0f;
		MakeABounce();
		DrawRectangle(hWnd);
	}
}
