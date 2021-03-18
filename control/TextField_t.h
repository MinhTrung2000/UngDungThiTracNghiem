#ifndef TEXTFIELD_T_H
#define TEXTFIELD_T_H

#include <string>
#include <cctype>
#include <windows.h>
#include "Rect_t.h"

using namespace std;

class TextField_t : public Rect_t
{
protected:
    string text;
    int length;

    //0 base
    int firstPosition;

    bool drawTextFlag;
    bool outOfLengthMessageFlag;

    Vector<bool (*)(const string&)> checkInputValid;

    //use currentPosition from Rect_t
public:
    TextField_t();
    TextField_t(int l, int t, int viewWidth);
    TextField_t(int borderType, int l, int t, int viewWidth);
    TextField_t(int borderType, int l, int t, int viewWidth, int length);

    void clear();

    void setOutOfLenghtMessageFlag(bool flag)
    {
        outOfLengthMessageFlag = flag;
    }

    void setValueLenght(int );
    int getValueLength()
    {
        return length;
    }

    void setValue(const string& newVal);
    string getValue()
    {
        return text;
    }

    void addCheckFunction(bool (*)(const string&));

    virtual void setDefaultColor();

    virtual void setCoor(int l, int t);

    virtual void draw(Graphics& g);

    virtual bool mouseClick(int l, int t, Graphics &g);

    virtual void keyPress(int keyCode, char ch, Graphics& g);

    COORD getCurrentPosition();

    bool IsCursorVisible();

    TextField_t operator = (const TextField_t& o)
    {
        Rect_t::operator=(o);
        text = o.text;
        length = o.length;
        firstPosition = o.firstPosition;
        checkInputValid = o.checkInputValid;
        outOfLengthMessageFlag = o.outOfLengthMessageFlag;
        return *this;
    }
};

#endif
