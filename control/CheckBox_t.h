#ifndef CHECKBOX_T_H
#define CHECKBOX_T_H

#include <string>
#include <sstream>
#include <iostream>
#include <stdint.h>

#include "Label_t.h"
#include "lib_vector.h"

using namespace std;

class CheckBox_t : public Label_t
{
    typedef short _checkBoxStyle_;

private:
    struct CheckBoxStyle
    {
        char checkChar;
        string boxString;
    };
    static const CheckBoxStyle cbStyleArr[2];

    _checkBoxStyle_ style;
    bool isChecked;

    friend class CheckList_t;
    friend class RadioBox_t;
    friend class HorRadioBox_t;

public:
    static const _checkBoxStyle_ BOX_STYLE = 0;
    static const _checkBoxStyle_ TICK_STYLE = 1;

    CheckBox_t();
    CheckBox_t(const string& val);
    CheckBox_t(int l, int t, const string& val);
    CheckBox_t(int l, int t, _checkBoxStyle_ _style, const string& val);

    //deprecated
    void clearValue() {}

    void setStyle(_checkBoxStyle_ newStyle);
    _checkBoxStyle_ getStyle();

    void setValue(const string& newVal);

    void setCoor(int l, int t);

    void setIsChecked(bool check);
    bool getIsChecked();
    void reverseChecked();

    void draw(Graphics &g);
    void draw(Graphics &g, int wid);

    bool mouseClick(int l, int t, Graphics &g);

    void keyPress(int keyCode, char ch, Graphics &g);

    CheckBox_t& operator = (const CheckBox_t& o)
    {
        Label_t::operator=(o);
        isChecked = o.isChecked;
        style = o.style;
        return *this;
    }
};

#endif


