#include "CheckList_t.h"

CheckList_t::CheckList_t()
    : Rect_t(),
      previousIndex(0),
      currentIndex(0),
      maxWidthItem(DEFAULT_MAX_WIDTH),
      cbStyle(CheckBox_t::BOX_STYLE),
      port(NULL)
{

    setDefaultColor();
}

CheckList_t::CheckList_t(int l, int t)
    : CheckList_t(l, t, Vector<string>()) {}

CheckList_t::CheckList_t(int l, int t, CheckBox_t::_checkBoxStyle_ _cbStyle)
    : CheckList_t(l, t, _cbStyle, Vector<string>()) {}

CheckList_t::CheckList_t(int l, int t, const Vector<string>& str)
    : CheckList_t(l, t, CheckBox_t::BOX_STYLE, str) {}

CheckList_t::CheckList_t(int l, int t, CheckBox_t::_checkBoxStyle_ _cbStyle, const Vector<string>& str)
    : previousIndex(0),
      currentIndex(0),
      maxWidthItem(DEFAULT_MAX_WIDTH),
      cbStyle(_cbStyle),
      port(NULL)
{

    int nextTop = 0;
    int wid = 0;

    if (!str.empty())
    {
        allCheckBoxes.push_back(new CheckBox_t(l, t, cbStyle, str[0]));

        nextTop = t + allCheckBoxes[0]->getCord().Y - 1;
        wid = allCheckBoxes[0]->getCord().X;

        for (int i = 1; i < str.size() ; i++)
        {
            allCheckBoxes.push_back(new CheckBox_t(l, nextTop + 1, cbStyle, str[i]));
            nextTop += allCheckBoxes[i]->getCord().Y;
            wid = std::max(wid, (int) allCheckBoxes[i]->getCord().X);
        }
    }

    Rect_t::setCoor(l, t, {std::min(wid, maxWidthItem) + 1, nextTop - t + 1});
    setDefaultColor();
}

//===========================================
//use port constructor
CheckList_t::CheckList_t(int borderType, int l, int t, int portLineNumber, const Vector<string>& str)
    : CheckList_t(borderType, l, t, portLineNumber, CheckBox_t::BOX_STYLE, str) {}

CheckList_t::CheckList_t(int l, int t, int portLineNumber, const Vector<string>& str)
    : CheckList_t(BorderFactory::SINGLE, l, t, portLineNumber, CheckBox_t::BOX_STYLE, str) {}

CheckList_t::CheckList_t(int l, int t, int portLineNumber, CheckBox_t::_checkBoxStyle_ _cbStyle, const Vector<string>& str)
    : CheckList_t(BorderFactory::SINGLE, l, t, portLineNumber, _cbStyle, str) {}

CheckList_t::CheckList_t(int l, int t, int portLineNumber)
    : CheckList_t(BorderFactory::SINGLE, l, t, portLineNumber, CheckBox_t::BOX_STYLE, Vector<string>()) {}

CheckList_t::CheckList_t(int borderType, int l, int t, int portLineNumber)
    : CheckList_t(borderType, l, t, portLineNumber, CheckBox_t::BOX_STYLE, Vector<string>()) {}

CheckList_t::CheckList_t(int l, int t, int portLineNumber, CheckBox_t::_checkBoxStyle_ _cbStyle)
    : CheckList_t(BorderFactory::SINGLE, l, t, portLineNumber, _cbStyle, Vector<string>()) {}

CheckList_t::CheckList_t(int borderType, int l, int t, int portLineNumber, CheckBox_t::_checkBoxStyle_ _cbStyle, const Vector<string>& str)
    : CheckList_t(l, t, _cbStyle, str)
{
    border = BorderFactory::makeBorder(borderType);
    port = new ViewPort(std::min(portLineNumber, str.size()));
    Rect_t::setCoor(left, top, {cord.X, portLineNumber + 1});
}

bool CheckList_t::isBufferViewPort()
{
    return (port != NULL && dynamic_cast<BufferViewPort *>(port) != NULL);
}

void CheckList_t::setEmptyLineFormatTable(const string& lineFormat)
{
    emptyLineFormatTable = lineFormat;
}

void CheckList_t::reset()
{
    previousIndex = currentIndex = 0;

    firstDrawFlag = true;

    if (port != NULL)
    {
        port->reset();
    }
}

void CheckList_t::setMaxWidthItem(int wid)
{
    maxWidthItem = wid;

    Rect_t::setCoor(left, top, {std::min((int) cord.X, maxWidthItem + 1), cord.Y});

    if (port != NULL && isBufferViewPort())
    {
        ((BufferViewPort *) port)->buffer.setCoor(left, top, cord);
    }
}

void CheckList_t::setBuffer()
{
    if (port != NULL)
    {
        int nline = port->lineNumber;

        delete port;

        port = new BufferViewPort(nline);

        ((BufferViewPort *) port)->buffer = BufferInfo(left, top, cord);
    }
}

void CheckList_t::setCoor(int l, int t)
{
    Rect_t::setCoor(l, t, cord);
    if (isBufferViewPort())
    {
        ((BufferViewPort *) port)->buffer.setCoor(left, top, cord);
    }
}

void CheckList_t::setDefaultColor()
{
    setColor(DEFAULT_CHECKLIST_ATTRIBUTES);
    setColorEle(DEFAULT_CHECKLIST_FOCUS_ROW_ATTRIBUTES, DEFAULT_CHECKLIST_UNFOCUS_ROW_ATTRIBUTES);
//  reset();
}

void CheckList_t::setFirstSelected()
{
    setSelectedIndex(0);
}

//set check and focus
void CheckList_t::setSelectedIndex(int id)
{
    if (allCheckBoxes.empty())
        return;

    assert(0 <= id && id < allCheckBoxes.size());

    if (currentIndex != id)
    {
        allCheckBoxes[currentIndex]->setColor(unFocusEleAttr);
    }

    previousIndex = currentIndex = id;
    allCheckBoxes[currentIndex]->setIsChecked(true);
    allCheckBoxes[currentIndex]->setColor(focusEleAttr);

    setFocusAtIndex(id);

    if (port != NULL)
    {
        port->drawAllLineFlag = true;
    }
    else
    {
        firstDrawFlag = true;
    }
}

//set focus but not set check
void CheckList_t::setFocusAtIndex(int id)
{
    if (allCheckBoxes.empty())
        return;

    assert(0 <= id && id < allCheckBoxes.size());

    allCheckBoxes[currentIndex]->setColor(unFocusEleAttr);
    currentIndex = id;
    allCheckBoxes[currentIndex]->setColor(focusEleAttr);

    if (port != NULL)
    {
        if (port->currentTopIndex > currentIndex)
        {
            port->currentTopIndex = currentIndex;
            port->currentBottomIndex = port->currentTopIndex + port->lineNumber - 1;
        }
        else if (port->currentBottomIndex < currentIndex)
        {
            port->currentBottomIndex = currentIndex;
            port->currentTopIndex = port->currentBottomIndex - port->lineNumber +1;
        }
        port->drawAllLineFlag = true;
    }
    else
    {
        firstDrawFlag = true;
    }
}

void CheckList_t::setDrawAllLinePort(bool flag)
{
    if (port != NULL)
    {
        port->drawAllLineFlag = flag;
    }
}

void CheckList_t::setToggleAll(bool drawFlag)
{
    for (int i = 0; i < allCheckBoxes.size(); ++i)
    {
        allCheckBoxes[i]->setIsChecked(true);
    }

    firstDrawFlag = true;
    setDrawAllLinePort(true);

    if (drawFlag)
    {
        draw(*Graphics::getCurrentActiveGraphics());
    }
}

void CheckList_t::setClearAll(bool drawFlag)
{
    for (int i = 0; i < allCheckBoxes.size(); ++i)
        allCheckBoxes[i]->setIsChecked(false);

    firstDrawFlag = true;
    setDrawAllLinePort(true);

    if (drawFlag)
    {
        draw(*Graphics::getCurrentActiveGraphics());
    }
}

void CheckList_t::setData(const Vector<string>& newData)
{
    allCheckBoxes.clear();

    int nextTop = top;
    int wid = cord.X - 1;

    if (! newData.empty())
    {
        allCheckBoxes.push_back(new CheckBox_t(left, nextTop + 1, cbStyle, newData[0]));

        wid = std::max(wid, (int) allCheckBoxes[0]->getCord().X);

        for (int i = 1; i < newData.size() ; i++)
        {
            nextTop += allCheckBoxes[i - 1]->getCord().Y;

            allCheckBoxes.push_back(new CheckBox_t(left, nextTop + 1, cbStyle, newData[i]));

            wid = std::max(wid, (int) allCheckBoxes[i]->getCord().X);
        }

        nextTop += allCheckBoxes.back()->getCord().Y;
    }

    if (port != NULL)
    {
        port->lineNumber = std::min((int) cord.Y - 1, allCheckBoxes.size());

        Rect_t::setCoor(left, top, {std::min(wid, maxWidthItem) + 1, cord.Y});
    }
    else
    {
        //clear old bufferscreen
        Graphics::getCurrentActiveGraphics()->clearRect(left, top, cord, unFocusEleAttr);

        Rect_t::setCoor(left, top, {std::min(wid, maxWidthItem) + 1, nextTop - top + 1});
    }

    firstDrawFlag = true;

    reset();

    if (isBufferViewPort())
    {
        ((BufferViewPort *) port)->buffer.setCoor(left, top, cord);
    }
}

void CheckList_t::setColorEle(uint8_t focusAttr, uint8_t unFocusAttr)
{
    focusEleAttr = focusAttr;
    unFocusEleAttr = unFocusAttr;
}

void CheckList_t::setFirstDrawFlag(bool flag)
{
    if (flag == true)
    {
        firstDrawFlag = true;
    }
}

void CheckList_t::draw(Graphics &g)
{
    if (port != NULL)
    {
        drawByPort(g);
        return;
    }

    if (firstDrawFlag)
    {
        g.setColor(this->attribute);
        border->draw(left, top, cord, g);

        if (! allCheckBoxes.empty())
        {
            for (int i = 0; i < allCheckBoxes.size(); ++i)
            {
                allCheckBoxes[i]->setColor(unFocusEleAttr);
            }

            allCheckBoxes[currentIndex]->setColor(focusEleAttr);

            for (int i = 0; i < allCheckBoxes.size(); ++i)
            {
                allCheckBoxes[i]->draw(g, maxWidthItem);
            }
        }

        firstDrawFlag = false;
    }

    if (!allCheckBoxes.empty())
    {
        allCheckBoxes[currentIndex]->draw(g, maxWidthItem);
    }
}

bool CheckList_t::havingCheck()
{
    for (int i = 0; i < allCheckBoxes.size(); ++i)
    {
        if (allCheckBoxes[i]->getIsChecked())
        {
            return true;
        }
    }

    return false;
}

bool CheckList_t::mouseScroll(int state, Graphics& g)
{
    if (port == NULL)
        return false;

    return mouseScrollByPort(state, g);
}

bool CheckList_t::mouseClick(int l, int t, Graphics &g)
{
    if (allCheckBoxes.empty())
        return false;

    if (port != NULL)
    {
        return mouseClickByPort(l, t, g);
    }

    if (g.isInside(l, t, left, top, cord.X - 1, cord.Y - 1))
    {
        for (int i = 0; i < allCheckBoxes.size(); ++i)
        {
            if (allCheckBoxes[i]->mouseClick(l, t, g))
            {
                if (currentIndex != i)
                {
                    allCheckBoxes[currentIndex]->setColor(unFocusEleAttr);
                    allCheckBoxes[currentIndex]->draw(g, maxWidthItem);

                    previousIndex = currentIndex = i;

                    allCheckBoxes[currentIndex]->setColor(focusEleAttr);
                }

                Rect_t::setFocus(*this);
                return true;
            }
        }
    }
    return false;
}

void CheckList_t::keyPress(int keyCode, char ch, Graphics &g)
{
    if (allCheckBoxes.empty())
        return;

    if (port != NULL)
    {
        keyPressByPort(keyCode, ch, g);
        return;
    }

    switch (keyCode)
    {
    case KEY_DOWN:	//down arrow
        if (currentIndex == allCheckBoxes.size() - 1)
        {
            canDrawFlag = false;
            break;
        }
        allCheckBoxes[currentIndex]->setColor(unFocusEleAttr);
        allCheckBoxes[currentIndex]->draw(g, maxWidthItem);

        allCheckBoxes[++currentIndex]->setColor(focusEleAttr);
        break;
    case KEY_UP:	// up arrow key
        if (currentIndex == 0)
        {
            canDrawFlag = false;
            break;
        }
        allCheckBoxes[currentIndex]->setColor(unFocusEleAttr);
        allCheckBoxes[currentIndex]->draw(g, maxWidthItem);

        allCheckBoxes[--currentIndex]->setColor(focusEleAttr);
        break;
    case KEY_SPACE:	// Space key
    case KEY_ENTER:	// Enter key
        allCheckBoxes[currentIndex]->reverseChecked();
        previousIndex = currentIndex;
        break;
    }
}

void CheckList_t::getAllRects(Vector<Rect_t *> &rects)
{
    rects.clear();

    for (int i = 0; i < allCheckBoxes.size(); ++i)
    {
        rects.push_back(allCheckBoxes[i]);
    }
}

Vector<int> CheckList_t::getSelectedIndexList()
{
    Vector<int> idList;

    for (int i = 0; i < allCheckBoxes.size(); ++i)
    {
        if (allCheckBoxes[i]->getIsChecked())
        {
            idList.push_back(i);
        }
    }

    return idList;
}

Vector<string> CheckList_t::getSelectedValueList()
{
    Vector<string> list;

    for (int i = 0; i < allCheckBoxes.size(); ++i)
    {
        if (allCheckBoxes[i]->getIsChecked())
        {
            list.push_back(allCheckBoxes[i]->getValue());
        }
    }

    return list;
}

string CheckList_t::getCheckBoxValueAtIndex(int id)
{
    assert(0 <= id && id < allCheckBoxes.size());

    return allCheckBoxes[id]->getValue();
}

void CheckList_t::drawLineByPort(int id, Graphics& g)
{
    assert(0 <= id && id < allCheckBoxes.size());

    Label_t *ele = allCheckBoxes[id];

    ele->setCoor(left + 1, top + 1 + id - port->currentTopIndex);
    ele->draw(g, maxWidthItem);

    g.setColor(ele->getAttribute());

    if (cord.X > ele->getCord().X)
    {
        g.write(left + ele->getCord().X + 1, top + 1 + id - port->currentTopIndex, string(cord.X - ele->getCord().X, ' '));
    }
}

void CheckList_t::hide(Graphics& g)
{
    if (isBufferViewPort())
    {
        port->getBuffer(g);

        setFocusAtIndex(previousIndex);

        closeFlag = false;

        port->setCanUpdateBufferFlag(true);
        port->drawAllLineFlag = true;
    }
}

void CheckList_t::drawByPort(Graphics& g)
{
    if (! canDrawFlag)
    {
        canDrawFlag = true;
        return;
    }

    if (port->getCanUpdateBufferFlag())
    {
        port->updateBuffer(g);
        port->setCanUpdateBufferFlag(false);
    }

    if (firstDrawFlag)
    {
        if (!allCheckBoxes.empty())
        {
            for (int i = 0; i < allCheckBoxes.size(); ++i)
            {
                allCheckBoxes[i]->setColor(unFocusEleAttr);
            }

            allCheckBoxes[currentIndex]->setColor(focusEleAttr);
        }

        firstDrawFlag = false;
    }

    if (port->drawAllLineFlag)
    {
        g.setColor(attribute);
        border->draw(left, top, cord, g);

        if (! allCheckBoxes.empty())
        {
            for (int i = port->currentTopIndex; i <= port->currentBottomIndex; ++i)
            {
                drawLineByPort(i, g);
            }
        }

        if (emptyLineFormatTable != "")
        {
            if (port->lineNumber < cord.Y - 1)
            {
                g.setColor(this->attribute);
                for (int i = port->lineNumber + 1; i <= cord.Y - 1; ++i)
                {
                    g.write(left + 1, top + i, emptyLineFormatTable);
                }
            }
        }
    }
    else
    {
        if (! allCheckBoxes.empty())
        {
            drawLineByPort(currentIndex, g);
        }
    }

    port->drawAllLineFlag = false;
}

bool CheckList_t::mouseClickByPort(int l, int t, Graphics& g)
{
    if (g.isInside(l, t, left + 1, top + 1, cord.X - 1, cord.Y - 1))
    {
        for (int i = port->currentTopIndex; i <= port->currentBottomIndex; ++i)
        {
            if (t == top + i - port->currentTopIndex + 1)
            {
                allCheckBoxes[i]->reverseChecked();

                if (currentIndex != i)
                {
                    allCheckBoxes[currentIndex]->setColor(unFocusEleAttr);
                    drawLineByPort(currentIndex, g);

                    previousIndex = currentIndex = i;

                    allCheckBoxes[currentIndex]->setColor(focusEleAttr);
                }

                port->drawAllLineFlag = false;

                Rect_t::setFocus(*this);
                return true;
            }
        }
    }

    return false;
}

bool CheckList_t::mouseScrollByPort(int state, Graphics& g)
{
    if (allCheckBoxes.empty())
        return false;

    if (state == 1)
    {
        keyPressByPort(KEY_UP, ' ', g);
    }
    else
    {
        keyPressByPort(KEY_DOWN, ' ', g);
    }

    Rect_t::setFocus(*this);
    return true;
}

void CheckList_t::keyPressByPort(int keyCode, char ch, Graphics& g)
{
    if (allCheckBoxes.empty())
        return;

    switch (keyCode)
    {
    case KEY_DOWN:	//down arrow
        if (currentIndex == allCheckBoxes.size() - 1)
        {
            canDrawFlag = false;
            break;
        }

        allCheckBoxes[currentIndex]->setColor(unFocusEleAttr);
        drawLineByPort(currentIndex, g);

        allCheckBoxes[++currentIndex]->setColor(focusEleAttr);

        if (currentIndex > port->currentBottomIndex)
        {
            port->currentBottomIndex = currentIndex;
            ++port->currentTopIndex;
            port->drawAllLineFlag = true;
        }
        else
        {
            port->drawAllLineFlag = false;
        }
        break;
    case KEY_UP:	// up arrow key
        if (currentIndex == 0)
        {
            canDrawFlag = false;
            break;
        }

        allCheckBoxes[currentIndex]->setColor(unFocusEleAttr);
        drawLineByPort(currentIndex, g);

        allCheckBoxes[--currentIndex]->setColor(focusEleAttr);

        if (currentIndex < port->currentTopIndex)
        {
            port->currentTopIndex = currentIndex;
            --port->currentBottomIndex;
            port->drawAllLineFlag = true;
        }
        else
        {
            port->drawAllLineFlag = false;
        }
        break;
    case KEY_SPACE:
    case KEY_ENTER:
        allCheckBoxes[currentIndex]->reverseChecked();
        previousIndex = currentIndex;
        port->drawAllLineFlag = false;
        break;
    }
}

//append new item to port and draw
void CheckList_t::insertNewItem(const string& newItem)
{
    if (port == NULL)
        return;

    int nextTop = top;

    if (! allCheckBoxes.empty())
    {
        nextTop = allCheckBoxes.back()->getTop() + allCheckBoxes.back()->getCord().Y;
    }

    allCheckBoxes.push_back(new CheckBox_t(left, nextTop + 1, cbStyle, newItem));

    port->lineNumber = std::min((int) cord.Y - 1, allCheckBoxes.size());

    setFocusAtIndex(allCheckBoxes.size() - 1);

    setDrawAllLinePort(true);
}

void CheckList_t::insertNewItem(int id, const string& newItem)
{
    assert(0 <= id && id < allCheckBoxes.size());

    if (port == NULL)
        return;

    int nextTop = top;

    if (! allCheckBoxes.empty() && id > 1)
    {
        nextTop = allCheckBoxes[id - 1]->getTop() + allCheckBoxes[id - 1]->getCord().Y;
    }

    allCheckBoxes.insert(id, new CheckBox_t(left, nextTop + 1, cbStyle, newItem));

    port->lineNumber = std::min((int) cord.Y - 1, allCheckBoxes.size());

    setFocusAtIndex(id);

    setDrawAllLinePort(true);
}

void CheckList_t::deleteItem(int id)
{
    assert(0 <= id && id < allCheckBoxes.size());

    if (port == NULL)
        return;

    int nextTop = top;

    if (! allCheckBoxes.empty() && id > 1)
    {
        nextTop = allCheckBoxes[id - 1]->getTop() + allCheckBoxes[id - 1]->getCord().Y;
    }

    for (int i = id; i < allCheckBoxes.size() - 1; ++i)
    {
        *allCheckBoxes[i] = *allCheckBoxes[i + 1];
    }

    allCheckBoxes.pop_back();

    port->lineNumber = std::min((int) cord.Y - 1, allCheckBoxes.size());

    reset();
}

//not change check flag
void CheckList_t::replaceItem(int id, const string& newItemReplaced)
{
    assert(0 <= id && id < allCheckBoxes.size());

    allCheckBoxes[id]->setValue(newItemReplaced);

    setFocusAtIndex(id);
}

void CheckList_t::clearAll()
{
    if (allCheckBoxes.empty())
        return;

    allCheckBoxes.clear();

    if (port != NULL)
    {
        port->lineNumber = 0;
    }

    reset();
}

