#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <stdint.h>
#include <iostream>
#include <windows.h>
#include <conio.h>

#include <cassert>

#include "lib_utility.h"

//debug
using namespace std;

inline string to_string(const string & s)
{
    return '"' + s + '"';
}
inline string to_string(bool b)
{
    return (b ? "true" : "false");
}
inline string to_string(const int& b)
{
    return StringUtility::to_string(b);
}

template<typename A> string to_string(const Vector<A>& v)
{
    bool f = 1;
    string res = "{";
    for (int i = 0; i < v.size(); ++i)
    {
        if(!f)
            res += ", ";
        f = 0;
        res += to_string(v[i]);
    }
    return res + "}";
}
inline void deb_out()
{
    cerr << endl;
}
template<typename H,typename... T> void deb_out(H h, T... t)
{
    cerr << " " << to_string(h);
    deb_out(t...);
}
#ifndef LOCAL
#define deb(...) cerr << "[" << #__VA_ARGS__ << "]:"; deb_out(__VA_ARGS__);
#define _deb(...) cerr << "[" << #__VA_ARGS__ << "]:"; deb_out(__VA_ARGS__); _getch();
#else
#define deb(...)
#endif

//https://www.codetable.net/unicodecharacters?page=2

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow ();

//Color: https://ss64.com/nt/color.html

//ANSCII key Codes
const char CHAR_DIRECTUP = '\x1e';
const char CHAR_DIRECTDOWN = '\x1f';
const char CHAR_DIRECTRIGHT = '\x1a';
const char CHAR_DIRECTLEFT = '\x1b';
const char CHAR_CONTROL = '\x11';

//keyCode
#define KEY_BACKSPACE				VK_BACK
#define KEY_ENTER 					VK_RETURN
#define KEY_ESCAPE 					VK_ESCAPE
#define KEY_SPACE						VK_SPACE
#define KEY_F1 							VK_F1
#define KEY_F2 							VK_F2
#define KEY_F3 							VK_F3
#define KEY_F4 							VK_F4
#define KEY_F5 							VK_F5
#define KEY_F6 							VK_F6
#define KEY_F7 							VK_F7
#define KEY_F8 							VK_F8
#define KEY_F9 							VK_F9
#define KEY_F10 						VK_F10
#define KEY_HOME 						VK_HOME
#define KEY_UP 							VK_UP
#define KEY_PAGEUP 					VK_PRIOR
#define KEY_LEFT 						VK_LEFT
#define KEY_RIGHT 					VK_RIGHT
#define KEY_END 						VK_END
#define KEY_DOWN 						VK_DOWN
#define KEY_PAGEDOWN 				VK_NEXT
#define KEY_INSERT 					VK_INSERT
#define KEY_DELETE					VK_DELETE

#define CONTROLKEY_CTRLLEFT 	LEFT_CTRL_PRESSED
#define CONTROLKEY_CTRLRIGHT 	RIGHT_CTRL_PRESSED
#define CONTROLKEY_ALTLEFT 		LEFT_ALT_PRESSED
#define CONTROLKEY_ALTRIGHT 	RIGHT_ALT_PRESSED
#define CONTROLKEY_SHIFT 			SHIFT_PRESSED

//Default attributes
#define DEFAULT_BUTTON_FOCUS_ATTRIBUTES 0xb1
#define DEFAULT_BUTTON_UNFOCUS_ATTRIBUTES 0x79

class Graphics
{
private:
    static Graphics* currentActiveGraphics;

    HANDLE _console;
    uint8_t _attribute;
    COORD currentCursorPosition;
    COORD maxWindowSize;

    Graphics(DWORD stdHandle = STD_OUTPUT_HANDLE);

    friend class BufferInfo;

public:

    void setConsoleTitle(const string& title);

    void setScreenSize(int w, int h);

    void clearScreen();

    void clearEndOfLine();

    void clearRect(short l, short t, COORD cord, uint8_t attributes);

    void moveTo(int x, int y);

    COORD getMaxCoord();

    void setColor(uint8_t attr);

    void setColor();

    void write(string s);

    void write(int x, int y, string s);

    void write(wstring s);

    void write(int x, int y, wstring s);

    void setCursorVisibility(bool isVisible);

    bool getCursorVisibility();

    bool isInside(int x, int y, int left, int top, int width, int height);

    COORD getCursorPos();

    void setCursorPos(int x, int y);

    COORD getCurrentPosition();

    static void setCurrentActiveGraphics(Graphics* g)
    {
        currentActiveGraphics = g;
    }

    static Graphics *getCurrentActiveGraphics()
    {
        return currentActiveGraphics;
    }

    Graphics& operator = (const Graphics& rhs)
    {
        _console = rhs._console;
        _attribute = rhs._attribute;
        currentCursorPosition = rhs.currentCursorPosition;
        maxWindowSize = rhs.maxWindowSize;
        return *this;
    }
};

class BufferInfo
{
private:
    int leftBuffer;
    int topBuffer;		//, right, bottom;
    COORD cordBuffer;
    CHAR_INFO *output;
    int szOutput;

public:
    BufferInfo()
        : leftBuffer(0), topBuffer(0), cordBuffer(
    {
        0, 0
    }), szOutput(0), output(NULL) {}

    BufferInfo(int l, int t, COORD c)
        : leftBuffer(l), topBuffer(t), cordBuffer(c)
    {
        szOutput = (cordBuffer.X + 2) * (cordBuffer.Y + 2);
        output = new CHAR_INFO[szOutput];
    }

    ~BufferInfo()
    {
        delete[] output;
    }

    void setCoor(int l, int t, COORD c)
    {
        if (output == NULL)
            return;
        leftBuffer = l;
        topBuffer = t;
        cordBuffer = c;
        szOutput = (cordBuffer.X + 2) * (cordBuffer.Y + 2);
        delete[] output;
        output = new CHAR_INFO[szOutput];
    }

    BufferInfo& operator = (const BufferInfo& o)
    {
        leftBuffer = o.leftBuffer;
        topBuffer = o.topBuffer;
        cordBuffer = o.cordBuffer;
        szOutput = o.szOutput;

        delete[] output;

        output = new CHAR_INFO[szOutput];
        for (int i = 0; i < szOutput; ++i)
        {
            output[i] = o.output[i];
        }
        return *this;
    }

    bool update(const Graphics& g)
    {
        SMALL_RECT srctReadRect;
        srctReadRect.Left = leftBuffer;
        srctReadRect.Top = topBuffer;
        srctReadRect.Right = leftBuffer + cordBuffer.X + 1;
        srctReadRect.Bottom = topBuffer + cordBuffer.Y + 1;

        COORD coordBufferSize;
        coordBufferSize.X = cordBuffer.X + 2;
        coordBufferSize.Y = cordBuffer.Y + 2;

        COORD coordBufferCoord;
        coordBufferCoord.X = 0;
        coordBufferCoord.Y = 0;

        if (! ReadConsoleOutput(g._console, output, coordBufferSize, coordBufferCoord, &srctReadRect))
            return false;

        return true;
    }

    bool get(const Graphics& g)
    {
        SMALL_RECT srctReadRect;
        srctReadRect.Left = leftBuffer;
        srctReadRect.Top = topBuffer;
        srctReadRect.Right = leftBuffer + cordBuffer.X + 1;
        srctReadRect.Bottom = topBuffer + cordBuffer.Y + 1;

        COORD coordBufferSize;
        coordBufferSize.X = cordBuffer.X + 2;
        coordBufferSize.Y = cordBuffer.Y + 2;

        COORD coordBufferCoord;
        coordBufferCoord.X = 0;
        coordBufferCoord.Y = 0;

        bool res;

        if (! WriteConsoleOutput(g._console, output, coordBufferSize, coordBufferCoord, &srctReadRect))
        {
            res = false;
        }
        else
            res = true;

        return res;
    }
};

#endif
