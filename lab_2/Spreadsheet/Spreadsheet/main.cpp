#include <windows.h>
#include "resource.h"
#include "spreadsheet.h"

Spreadsheet *spreadheet;
RECT clientRect;

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
	}

	delete spreadheet;
	return (INT)msg.wParam;
}

LRESULT WINAPI MyWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
		spreadheet->Draw(hWnd);
		break;
	case WM_SIZE:
	{
		GetClientRect(hWnd, &clientRect);
		spreadheet->SetARect(clientRect);
		InvalidateRect(hWnd, NULL, TRUE);
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
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
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
	HWND hWnd = CreateWindow(L"WinAPI", L"Spreadsheet", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, 0, 1280, 720, NULL, NULL, hInstance, NULL);
	if (!hWnd)
	{
		return NULL;
	}

	GetClientRect(hWnd, &clientRect);

	spreadheet = new Spreadsheet(clientRect, "spreadsheet.txt");
	if (!spreadheet)
	{
		return NULL;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}