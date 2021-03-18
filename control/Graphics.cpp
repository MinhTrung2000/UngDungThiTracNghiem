#include "Graphics.h"

//codepage: https://docs.microsoft.com/en-us/windows/win32/intl/code-page-identifiers?redirectedfrom=MSDN

Graphics* Graphics::currentActiveGraphics = new Graphics;

Graphics::Graphics(DWORD stdHandle)
    : _console(GetStdHandle(stdHandle)), _attribute(0x0f), maxWindowSize{170, 45}
{
    setColor(0x0f);
//	SetConsoleCP(65001);  //unicode output
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(_console, &csbi);
    SHORT x = csbi.srWindow.Right - csbi.srWindow.Left;
    SHORT y = csbi.srWindow.Bottom - csbi.srWindow.Top;
//  maxWindowSize = {x, y};
}

void Graphics::setConsoleTitle(const string& title)
{
    assert(SetConsoleTitle(title.c_str()) == true);
}

void Graphics::setScreenSize(int w, int h)
{
    string cmd = "mode con: cols=" + StringUtility::to_string(w) + " lines=" + StringUtility::to_string(h);
    system(cmd.c_str());
    HWND console = GetConsoleWindow();
    SetWindowLong(console, GWL_STYLE, GetWindowLong(console, GWL_STYLE) & ~WS_MAXIMIZEBOX);
//	maxWindowSize = {w, h};
}

void Graphics::clearScreen()
{
    DWORD d;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(_console, &csbi);
    int size = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputAttribute(_console, csbi.wAttributes, size, { 0, 0 }, &d);
    FillConsoleOutputCharacter(_console, L' ', size, { 0, 0 }, &d);
}

void Graphics::moveTo(int x, int y)
{
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(_console, c);
}

void Graphics::clearEndOfLine()
{
    COORD coord;
    DWORD written;
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(_console, &info);
    coord.X = info.dwCursorPosition.X;
    coord.Y = info.dwCursorPosition.Y;
    FillConsoleOutputCharacter(_console, L' ', info.dwSize.X - info.dwCursorPosition.X * info.dwCursorPosition.Y, coord, &written);
    moveTo (info.dwCursorPosition.X, info.dwCursorPosition.Y );
}

void Graphics::clearRect(short left, short top, COORD cord, uint8_t attributes)
{
//	COORD szBuf = cord;
//	COORD BufferCoord = {left, top};
//	SMALL_RECT srWriteRegion = {left, top, left + cord.X, top + cord.Y};
//	CHAR_INFO *lpBuffer = (CHAR_INFO *)calloc(szBuf.X * szBuf.Y, sizeof(CHAR_INFO));
//	WriteConsoleOutput(_console, lpBuffer, szBuf, BufferCoord, &srWriteRegion);
//	free(lpBuffer);
    setColor(attributes);
    for (int i = 0; i < cord.Y + 1; ++i)
    {
        write(left, top + i, string(cord.X + 2, ' '));
    }
}

COORD Graphics::getMaxCoord()
{
    return maxWindowSize;
}

void Graphics::write(string s)
{
    WriteConsoleA(_console, s.c_str(), s.size(), NULL, NULL);
}

void Graphics::write(wstring s)
{
    WriteConsoleW(_console, s.c_str(), s.size(), NULL, NULL);
}

void Graphics::write(int x, int y, string s)
{
    moveTo(x, y);
    write(s);
}

void Graphics::write(int x, int y, wstring s)
{
    moveTo(x, y);
    write(s);
}

void Graphics::setCursorVisibility(bool isVisible)
{
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(_console, &cci);
    cci.bVisible = isVisible;
    SetConsoleCursorInfo(_console, &cci);
}

bool Graphics::getCursorVisibility()
{
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(_console, &cci);
    return cci.bVisible;
}

void Graphics::setColor(uint8_t attr)
{
    SetConsoleTextAttribute(_console, attr);
}

void Graphics::setColor()
{
    setColor(0x0f);
}

bool Graphics::isInside(int x, int y, int left, int top, int width, int height)
{
    return left <= x && x <= left + width && top <= y && y <= top + height;
}

COORD Graphics::getCursorPos()
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(_console, &info);
    return info.dwCursorPosition;
}

void Graphics::setCursorPos(int x, int y)
{
    currentCursorPosition.X = (short)x;
    currentCursorPosition.Y = (short)y;
}

COORD Graphics::getCurrentPosition()
{
    return currentCursorPosition;
}
