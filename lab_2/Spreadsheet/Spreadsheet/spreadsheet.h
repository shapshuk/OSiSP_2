#pragma once
#include <windows.h>

#include <string>
#include <vector>

#include <fstream>
#include <regex>

class Spreadsheet
{
private:
	HDC hdc;
	PAINTSTRUCT ps;
	RECT clientRect;
	
	std::vector<std::vector<std::string>> vector;

	HPEN tablePen;
	HFONT hFont;

	INT rowCount;
	INT columnCount;

	INT GetRowCount();
	INT GetColumnCount();

	INT LoadFromFile(std::string path);

	void CreateMyFont(INT height, INT width, INT weight);
	INT DrawCell();

public:
	Spreadsheet(RECT clientRect, std::string path);
	~Spreadsheet();

	void Draw(HWND hWnd);
	void SetARect(RECT clientRect);
};

