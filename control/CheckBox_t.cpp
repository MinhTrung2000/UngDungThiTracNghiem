#include "CheckBox_t.h"

const CheckBox_t::CheckBoxStyle CheckBox_t::cbStyleArr[2] =
{
    {'x', "[ ]"}, {'\xfb', "   "}
};

CheckBox_t::CheckBox_t()
    : Label_t(), isChecked(false), style(BOX_STYLE)
{
    ableFocusFlag = true;
//  Rect_t::setCoor(left, top, {cord.X + 3, cord.Y});
}

CheckBox_t::CheckBox_t(const string& val)
    : CheckBox_t(0, 0, BOX_STYLE, val) {}

CheckBox_t::CheckBox_t(int l, int t, const string& val)
    : CheckBox_t(l, t, BOX_STYLE, val) {}

CheckBox_t::CheckBox_t(int l, int t, _checkBoxStyle_ _style, const string& val)
    : Label_t(l, t, val),
      isChecked(false),
      style(_style)
{

    ableFocusFlag = true;
    Rect_t::setCoor(left, top, {cord.X + 3, cord.Y});
}

void CheckBox_t::setIsChecked(bool check)
{
    isChecked = check;
}

bool CheckBox_t::getIsChecked()
{
    return isChecked;
}

void CheckBox_t::reverseChecked()
{
    isChecked ^= 1;
}

void CheckBox_t::setStyle(_checkBoxStyle_ newStyle)
{
    style = newStyle;
}

CheckBox_t::_checkBoxStyle_ CheckBox_t::getStyle()
{
    return style;
}

void CheckBox_t::setValue(const string& newVal)
{
    Label_t::setValue(newVal);
    Rect_t::setCoor(left, top, {cord.X + 4, cord.Y});
}

void CheckBox_t::setCoor(int l, int t)
{
    Label_t::setCoor(l, t);
    Rect_t::setCoor(left, top, {cord.X + 4, cord.Y});
}

void CheckBox_t::draw(Graphics& g)
{
    g.setColor(attribute);
    g.write(left, top, cbStyleArr[style].boxString + " ");

    for (int i = 0; i < valueLines.size(); ++i)
    {
        g.write(left + 4, top + i, valueLines[i]);
    }

    if (isChecked)
    {
        g.write(left + 1, top, string(1, cbStyleArr[style].checkChar));
    }
}

void CheckBox_t::draw(Graphics& g, int wid)
{
    assert(wid > 3);

    g.setColor(attribute);
    g.write(left, top, cbStyleArr[style].boxString + " ");

    for (int i = 0; i < valueLines.size(); ++i)
    {
        string text = valueLines[i];
        StringUtility::adjustStringToFit(text, wid - 3);
        g.write(left + 4, top + i, text);
    }

    if (isChecked)
    {
        g.write(left + 1, top, string(1, cbStyleArr[style].checkChar));
    }
}

bool CheckBox_t::mouseClick(int l, int t, Graphics &g)
{
    if (g.isInside(l, t, left, top, cord.X - 1, 0))
    {
        isChecked ^= 1;
        return true;
    }
    return false;
}

void CheckBox_t::keyPress(int keyCode, char charecter, Graphics &g)
{
    if (keyCode == KEY_SPACE || keyCode == KEY_ENTER)
    {
        isChecked ^= 1;
    }
}


