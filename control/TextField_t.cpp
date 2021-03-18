#include "TextField_t.h"

TextField_t::TextField_t()
    : Rect_t(),
      length(0),
      firstPosition(0)
{

    setCurrentPosition(0, 0);
    setDefaultColor();
}

TextField_t::TextField_t(int l, int t, int viewWidth)
    : Rect_t(l, t,
{
    viewWidth + 2, 1
}, ""),
length(viewWidth),
firstPosition(0)
{

    setCurrentPosition(l + 1, t);
    setDefaultColor();
}

TextField_t::TextField_t(int borderType, int l, int t, int viewWidth)
    : TextField_t(borderType, l, t, viewWidth, viewWidth) {}

TextField_t::TextField_t(int borderType, int l, int t, int viewWidth, int length)
    : Rect_t(borderType, l, t,
{
    viewWidth + 2, 2
}, ""),
length(length),
firstPosition(0)
{

    setCurrentPosition(l + 1, t + 1);
    setDefaultColor();
}

void TextField_t::setValueLenght(int newLength)
{
    length = newLength;
}

void TextField_t::clear()
{
    text = "";
    firstPosition = 0;

    if (Border_t::isNoBorder(border))
    {
        currentPosition = {left + 1, top};
    }
    else
    {
        currentPosition = {left + 1, top + 1};
    }

//  drawTextFlag = true;
//  draw(*Graphics::getCurrentActiveGraphics());
}

void TextField_t::setValue(const string& newVal)
{
    text = newVal;
    StringUtility::adjustStringToFit(text, length);

    firstPosition = 0;
    if (text.size() > cord.X - 2)
    {
        firstPosition = text.size() - (cord.X - 2);
    }

    if (Border_t::isNoBorder(border))
    {
//    currentPosition = {left + 1, top};
        currentPosition = {std::min(left + 1 + (int) text.size(), left + (int) cord.X - 1), top};
    }
    else
    {
//    currentPosition = {left + 1, top + 1};
        currentPosition = {std::min(left + 1 + (int) text.size(), left + (int) cord.X - 1), top + 1};
    }
}

void TextField_t::setCoor(int l, int t)
{
    Rect_t::setCoor(l, t, cord);
}

void TextField_t::draw(Graphics& g)
{
    g.setColor(attribute);

    string output = text.substr(firstPosition, std::min((size_t) cord.X - 2, text.size()));

    if (Border_t::isNoBorder(border))
    {
        g.write(left, top, string(1, '|') + string(cord.X - 2, ' ') + string(1, '|'));
        g.write(left + 1, top, output);
    }
    else
    {
        border->draw(left, top, {cord.X - 2, cord.Y}, titleRect, g);
        g.write(left + 1, top + 1, output);
    }

    g.setCursorVisibility(true);
    g.moveTo(currentPosition.X, currentPosition.Y);
}

void TextField_t::setDefaultColor()
{
    setColor(0xf5);
}

void TextField_t::addCheckFunction(bool (*newFunc)(const string&))
{
    checkInputValid.push_back(newFunc);
}

bool TextField_t::mouseClick(int l, int t, Graphics &g)
{
    if ((Border_t::isNoBorder(border) && g.isInside(l, t, left + 1, top, cord.X - 2, 0)) ||
            (! Border_t::isNoBorder(border) && g.isInside(l, t, left + 1, top + 1, cord.X - 2, 0)))
    {
        //not allow to click
        if (! ableFocusFlag)
            return false;

        if (l >= left + 1 + text.size())
        {
            currentPosition = {left + 1 + text.size(), t};
        }
        else
        {
            currentPosition = {l, t};
        }
        g.setCursorVisibility(true);
        g.moveTo(currentPosition.X, currentPosition.Y);
        Rect_t::setFocus(*this);
        return true;
    }
    return false;
}

void TextField_t::keyPress(int keyCode, char ch, Graphics& g)
{
//  deb(firstPosition, currentPosition.X);
    if (! ableFocusFlag)
        return;

    //1 base
    int currentX = currentPosition.X - 1 - left + firstPosition;

    switch (keyCode)
    {
    case KEY_BACKSPACE:  		// backspace
    {
        if (currentX == 0)
            return;
//      --currentPosition.X;

        text.erase(currentX - 1, 1);

        if (currentX == firstPosition && firstPosition > 0)
        {
            if (firstPosition <= cord.X - 2)
            {
                currentPosition.X = left + 1 + firstPosition - 1;
                firstPosition = 0;
            }
            else
            {
                firstPosition -= cord.X - 2;
                currentPosition.X = left + cord.X - 2;
            }
        }
        else
        {
            --currentPosition.X;
        }

        assert(firstPosition >= 0);
        assert(firstPosition <= text.size());
        assert(left < currentPosition.X);
        assert(currentPosition.X <= left + cord.X - 1);
//      deb(firstPosition, currLastPosition, currentPosition.X);
        break;
    }
    case KEY_DELETE:  	// del key
    {
        if (currentX == text.size())
            return;

        text.erase(currentX, 1);

        assert(firstPosition >= 0);
        assert(firstPosition <= text.size());
        assert(left < currentPosition.X);
        assert(currentPosition.X <= left + cord.X - 1);
//      deb(firstPosition, currLastPosition, currentPosition.X);

        break;
    }
    case KEY_LEFT:      // left arrow
    {
        if (currentX == 0 && firstPosition == 0)
            return;

        if (currentX == firstPosition && firstPosition > 0)
        {
            --firstPosition;
//        currentPosition.X = left + cord.X - 1;
        }
        else
        {
            --currentPosition.X;
        }

        assert(firstPosition >= 0);
        assert(firstPosition <= text.size());
        assert(left < currentPosition.X);
        assert(currentPosition.X <= left + cord.X - 1);
//      deb(firstPosition, currLastPosition, currentPosition.X);

        break;
    }
    case KEY_RIGHT:  	// right arrow
    {
        if (currentX  == text.size())
            return;
//      ++currentPosition.X;

        if (currentX == cord.X - 2 + firstPosition)
        {
            ++firstPosition;
//        currentPosition.X = left + cord.X - 1;
        }
        else
        {
            ++currentPosition.X;
        }

        assert(firstPosition >= 0);
        assert(firstPosition <= text.size());
        assert(left < currentPosition.X);
        assert(currentPosition.X <= left + cord.X - 1);
//      deb(firstPosition, currLastPosition, currentPosition.X);

        break;
    }
    case KEY_HOME:
    {
        currentPosition.X = left + 1;
        firstPosition = 0;
        break;
    }
    case KEY_END:
    {
        currentPosition.X = std::min(left + 1 + (int) text.size(), left + (int) cord.X - 1);
        if (text.size() > cord.X - 2)
        {
            firstPosition = text.size() - (cord.X - 2);
//        int n = text.size();
//        firstPosition = n - ((int) n / (cord.X - 2)) * (cord.X - 2);
        }
        break;
    }
    default:
    {
        if (iscntrl(ch))
            return;
//      if (text.size() == cord.X) return;
        if (text.size() == length)
            return;

        text.insert(currentX, 1, ch);
        for (int i = 0; i < checkInputValid.size(); ++i)
        {
            if (checkInputValid[i](text) == false)
            {
                text.erase(currentX, 1);
                return;
            }
        }

        if (currentX == cord.X - 2 + firstPosition)  // && text.size() >= cord.X - 2) {
        {
            ++firstPosition;
//        currentPosition.X = left + cord.X - 2;
        }
        else
        {
            ++currentPosition.X;
        }

//      if (currentPosition.X < left + cord.X - 1) {
//        ++currentPosition.X;
//      }

        assert(firstPosition >= 0);
        assert(firstPosition <= text.size());
        assert(left < currentPosition.X);
        assert(currentPosition.X <= left + cord.X - 1);

        break;
    }
    }
}

COORD TextField_t::getCurrentPosition()
{
    return currentPosition;
}

bool TextField_t::IsCursorVisible()
{
    return true;
}

