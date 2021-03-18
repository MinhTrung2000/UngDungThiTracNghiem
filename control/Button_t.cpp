#include "Button_t.h"

Button_t::Button_t() : Rect_t()
{
    setDefaultColor();
}

Button_t::Button_t(const string& val)
    : Button_t(0, 0, val)
{
    setDefaultColor();
}

Button_t::Button_t(int l, int t, const string& val)
    : Rect_t(BorderFactory::NONE, l, t,
{
    val.size() + 2, 0
}, ""), value(val)
{
    setDefaultColor();
}

void Button_t::setValue(const string& newVal)
{
    value = newVal;
    setCoor(left, top);
}

string Button_t::getValue()
{
    return value;
}

void Button_t::setCoor(int l, int t)
{
    Rect_t::setCoor(l, t, {value.size() + 2, 0});
}

void Button_t::draw(Graphics& g)
{
    g.setColor(attribute);
    string format = "[" + value + "]";
    g.write(left, top, format);
}

void Button_t::setDefaultColor()
{
    setColor(DEFAULT_BUTTON_UNFOCUS_ATTRIBUTES);
}

bool Button_t::mouseClick(int l, int t, Graphics &g)
{
    if (! ableFocusFlag)
        return false;

    if (g.isInside(l, t, left, top, cord.X - 1, cord.Y))
    {
        Rect_t::setFocus(*this);
        return true;
    }
    return false;
}

