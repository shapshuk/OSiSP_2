#include <windows.h>
#include "resource.h"
#include "sprite.h"

Sprite *sprite;

RECT clientRect;
POINT mouseCoords;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM             MyRegisterClass(HINSTANCE);
HWND             InitInstance(HINSTANCE, INT);

INT CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	MSG msg;
	HWND hWnd;

	MyRegisterClass(hInstance);
	hWnd = InitInstance(hInstance, nCmdShow);

	msg.message = WM_NULL;
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&msg);
		}

		if (GetAsyncKeyState(VK_LBUTTON))
		{
			GetCursorPos(&mouseCoords);
			ScreenToClient(hWnd, &mouseCoords);

			sprite->Move(mouseCoords);

			InvalidateRect(hWnd, NULL, FALSE);
		}
	}

	delete sprite;
	return (INT)msg.wParam;
}

LRESULT WINAPI MyWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
		sprite->Draw(hWnd);
		break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_UP:
		case 'W':
			sprite->MakeAStep('W');
			break;
		case VK_DOWN:
		case 'S':
			sprite->MakeAStep('S');
			break;
		case VK_LEFT:
		case 'A':
			sprite->MakeAStep('A');
			break;
		case VK_RIGHT:
		case 'D':
			sprite->MakeAStep('D');
			break;
		case VK_RETURN:
			sprite->ChangeAState();
			break;
		}

		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			LOWORD(wParam) == MK_SHIFT ? sprite->MakeAStep('D') : sprite->MakeAStep('S');
		}
		else
		{
			LOWORD(wParam) == MK_SHIFT ? sprite->MakeAStep('A') : sprite->MakeAStep('W');
		}
	}
	case WM_SIZE:
	{
		GetClientRect(hWnd, &clientRect);

		sprite->SetARect(clientRect);
		sprite->MakeABounce();
		
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
	wcex.hCursor = LoadCursor(NULL, IDC_HAND);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.hInstance = hInstance;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.lpfnWndProc = MyWindowProc;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"WinAPI";

	return RegisterClassEx(&wcex);
}

HWND InitInstance(HINSTANCE hInstance, INT nCmdShow)
{
	HWND hWnd = CreateWindow(L"WinAPI", L"Sprite", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, 0, 1920, 1080, NULL, NULL, hInstance, NULL);
	if (!hWnd)
	{
		return NULL;
	}

	GetClientRect(hWnd, &clientRect);
	sprite = new Sprite(hWnd, clientRect);
	if (!sprite)
	{
		return NULL;
	}

	sprite->InitProperties((clientRect.right - clientRect.left - 80.0f) / 2, (clientRect.bottom - clientRect.top - 60.0f) / 2, 80.0f, 60.0f, 10.0f, 15.0f);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}
