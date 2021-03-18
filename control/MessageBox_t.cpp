#include "MessageBox_t.h"

const uint8_t MessageBox_t::OPT_COLOR[4] =
{
    WARNING_MESSAGE_ATTRIBUTE,
    INFORMATION_MESSAGE_ATTRIBUTE,
    ERROR_MESSAGE_ATTRIBUTE,
    QUESTION_MESSAGE_ATTRIBUTE,
};

//made to equivalent to OPT_COLOR
const uint8_t MessageBox_t::BUTTON_COLOR[4][2] =
{
    { WARNING_MESSAGE_BUTTON_FOCUS_ATTRIBUTE,	WARNING_MESSAGE_BUTTON_UNFOCUS_ATTRIBUTE          },
    { INFORMATION_MESSAGE_BUTTON_FOCUS_ATTRIBUTE, INFORMATION_MESSAGE_BUTTON_UNFOCUS_ATTRIBUTE  },
    { ERROR_MESSAGE_BUTTON_FOCUS_ATTRIBUTE,	ERROR_MESSAGE_BUTTON_UNFOCUS_ATTRIBUTE              },
    { QUESTION_MESSAGE_BUTTON_FOCUS_ATTRIBUTE,	QUESTION_MESSAGE_BUTTON_UNFOCUS_ATTRIBUTE       }
};

const string MessageBox_t::MESSAGE_TITLE[4] =
{
    "Warning Message", "Infomation Message", "Error Message", "Question Message"
};

MessageBox_t::MessageBox_t() : Panel_t() {}

MessageBox_t::MessageBox_t(int l, int t, const string& message, _messageType_ msgType)
    : MessageBox_t(l, t, message, msgType, PLAIN_OPTION) {}

MessageBox_t::MessageBox_t(int l, int t, const string& message, _messageType_ msgType, _optionType_ optType)
{
    border = BorderFactory::makeBorder(BorderFactory::SINGLE);
    setMessage(message, msgType);
    setCoor(l, t);
    setButtonMode(optType);
}

MessageBox_t::MessageBox_t(int l, int t, const string& newTitle, const string& message, _messageType_ msgType, _optionType_ optType)
{
    border = BorderFactory::makeBorder(BorderFactory::SINGLE);
    setTitle(newTitle);
    setMessage(message, msgType);
    setCoor(l, t);
    setButtonMode(optType);
}

void MessageBox_t::setCoor(int l, int t)
{
    cord.X = MAX_MESSAGE_LENGTH + 3;

    int maxWid = 0;
    for (int i = 0; i < msgLines.size(); ++i)
        maxWid = std::max(maxWid, (int) msgLines[i].size());

    while (cord.X <= maxWid && cord.X < 100)
    {
        cord.X += MESSAGE_WIDTH_DIFF;
    }

    cord.Y = msgLines.size() + 4;
    Rect_t::setCoor(l, t, cord);
    setBuffer();
}

void MessageBox_t::setCoor()
{
    COORD maxCord = Graphics::getCurrentActiveGraphics()->getMaxCoord();

    cord.X = MAX_MESSAGE_LENGTH + 3;

    int maxWid = 0;
    for (int i = 0; i < msgLines.size(); ++i)
        maxWid = std::max(maxWid, (int) msgLines[i].size());

    while (cord.X <= maxWid && cord.X < 100)
    {
        cord.X += MESSAGE_WIDTH_DIFF;
    }

    cord.Y = msgLines.size() + 4;
//  Rect_t::setCoor(maxCord.X / 3 * 2 , maxCord.Y / 3 * 2 , cord);
    //set middle window
    Panel_t::setCoor();

    setBuffer();
}

void MessageBox_t::setButtonColor(uint8_t focusAttr, uint8_t unFocusAttr)
{
    focusButtonAttr = focusAttr;
    unFocusButtonAttr = unFocusAttr;
}

//Can use char '\n' to split message to each line
void MessageBox_t::setMessage(const string& newMessage, _messageType_ msgType)
{
    msgLines = StringUtility::splitStringByChar(newMessage, '\n');
    if (titleRect == "")
        titleRect = MESSAGE_TITLE[msgType];
    setColor(OPT_COLOR[msgType]);
    setButtonColor(BUTTON_COLOR[msgType][0], BUTTON_COLOR[msgType][1]);
}

void MessageBox_t::addButton(const string& btnValue)
{
    items.push_back(new Button_t(btnValue));
    int n = items.size();

    const int BTNLIST_Y = top + msgLines.size() + 2;
    const int BTNLIST_X_CENTRAL = left + cord.X / 2;

    int nextLeft;
    if (n == 1)
    {
        nextLeft = BTNLIST_X_CENTRAL - 1;
    }
    else
    {
        //? if number of button overflow, insert new row after the current one
        assert(n <= 3);
        nextLeft = items[n - 2]->getLeft() + items[n - 2]->getCord().X + BUTTON_PADDING;
    }
    items[n - 1]->setCoor(nextLeft, BTNLIST_Y);

    for (int i = 0; i < items.size(); ++i)
    {
        items[i]->setColor(unFocusButtonAttr);
    }

    items[0]->setColor(focusButtonAttr);
}

void MessageBox_t::setButtonMode(_optionType_ optType)
{
    items.clear();

    const int BTNLIST_Y = top + msgLines.size() + 2;
    const int BTNLIST_X_CENTRAL = left + cord.X / 2;

    if (optType == PLAIN_OPTION)
    {
        items.push_back(new Button_t("OK"));

        items[0]->setCoor(BTNLIST_X_CENTRAL - 1, BTNLIST_Y);

    }
    else if (optType == YES_NO_CANCEL_OPTION)
    {
        items.push_back(new Button_t("YES"));
        items.push_back(new Button_t("NO"));
        items.push_back(new Button_t("CANCEL"));

        items[0]->setCoor(BTNLIST_X_CENTRAL - ((Button_t *) items[1])->getValue().size() / 2 - BUTTON_PADDING - ((Button_t *) items[0])->getValue().size() - 2, BTNLIST_Y);
        items[1]->setCoor(BTNLIST_X_CENTRAL - ((Button_t *) items[1])->getValue().size() / 2, BTNLIST_Y);
        items[2]->setCoor(BTNLIST_X_CENTRAL + ((Button_t *) items[1])->getValue().size() / 2 + BUTTON_PADDING + 2, BTNLIST_Y);

    }
    else
    {
        items.push_back(new Button_t("OK"));
        items.push_back(new Button_t("CANCEL"));

        items[0]->setCoor(BTNLIST_X_CENTRAL - BUTTON_PADDING - ((Button_t *) items[0])->getValue().size() - 2, BTNLIST_Y);
        items[1]->setCoor(BTNLIST_X_CENTRAL + BUTTON_PADDING + 1, BTNLIST_Y);
    }

    for (int i = 0; i < items.size(); ++i)
    {
        items[i]->setColor(unFocusButtonAttr);
    }

    items[0]->setColor(focusButtonAttr);
}

void MessageBox_t::draw(Graphics& g)
{
    if (firstDrawFlag)
    {
        assert(buffer->update(g));

        g.setColor(attribute);
        border->draw(left, top, cord, titleRect, g);

        for (int i = 0; i < msgLines.size(); ++i)
        {
            string text = msgLines[i];
            StringUtility::adjustStringToFit(text, cord.X);
            g.write(left + 1, top + 1 + i, text);
        }
        for (int i = 0; i < items.size(); ++i)
        {
            items[i]->draw(g);
        }

//    items[currentFocusIndex]->setColor(focusButtonAttr);
        if (! items.empty())
        {
            currentFocusItem = items[0];
            currentFocusItem->setColor(focusButtonAttr);
        }

        firstDrawFlag = false;
    }

//  items[currentFocusIndex]->draw(g);
    if (currentFocusItem != NULL)
        currentFocusItem->draw(g);
    Rect_t::setFocus(*this);
}

bool MessageBox_t::mouseClick(int l, int t, Graphics &g)
{
    if (g.isInside(l, t, left, top, cord.X, cord.Y))
    {
        for (int i = 0; i < items.size(); i++)
        {
            if (items[i]->mouseClick(l, t, g))
            {
//        currentFocusIndex = i;
                currentFocusItem = items[i];
                setCloseFlag(true);
                return true;
            }
        }
    }
    return false;
}

void MessageBox_t::keyPress(int keyCode, char ch, Graphics& g)
{
    int currentFocusIndex = 0;
    for (int i = 0; i < items.size(); ++i)
    {
        if (currentFocusItem == items[i])
            currentFocusIndex = i;
    }

    switch (keyCode)
    {
    case KEY_LEFT:
        if (currentFocusIndex == 0)
            return;
        items[currentFocusIndex]->setColor(unFocusButtonAttr);
        items[currentFocusIndex]->draw(g);
        items[--currentFocusIndex]->setColor(focusButtonAttr);
        currentFocusItem = items[currentFocusIndex];
        break;
    case KEY_RIGHT:
        if (currentFocusIndex == items.size() - 1)
            return;
        items[currentFocusIndex]->setColor(unFocusButtonAttr);
        items[currentFocusIndex]->draw(g);
        items[++currentFocusIndex]->setColor(focusButtonAttr);
        currentFocusItem = items[currentFocusIndex];
        break;
    case KEY_ENTER:
        items[currentFocusIndex]->keyPress(keyCode, ch, g);
        setCloseFlag(true);
    }
}

string MessageBox_t::getSelectedButtonValue()
{
    return ((Button_t *) currentFocusItem)->getValue();
}

Button_t *MessageBox_t::getButtonAtId(int id)
{
    if (!(0 <= id && id < items.size()))
        return NULL;
    return ((Button_t *) items[id]);
}

//Message dialog
string showMessageDialog(const string& message, MessageBox_t::_messageType_ msgType)
{
    assert(msgType != MessageBox_t::QUESTION_MESSAGE);

    MessageBox_t dialog(0, 0, message, msgType);
    dialog.setCoor();
    dialog.setButtonMode(MessageBox_t::PLAIN_OPTION);

    EventEngine tmpProcess(EventEngine::getCurrentProcess());
    tmpProcess.setMainFrame(dialog);
    tmpProcess.run(false);

    return dialog.getSelectedButtonValue();
}

string showMessageDialog(int l, int t, const string& message, MessageBox_t::_messageType_ msgType)
{
    assert(msgType != MessageBox_t::QUESTION_MESSAGE);

    MessageBox_t dialog(l, t, message, msgType);
    dialog.setButtonMode(MessageBox_t::PLAIN_OPTION);

    EventEngine tmpProcess(EventEngine::getCurrentProcess());
    tmpProcess.setMainFrame(dialog);
    tmpProcess.run(false);

    return dialog.getSelectedButtonValue();
}

string showMessageDialog(const string& title, const string& message, MessageBox_t::_messageType_ msgType)
{
    assert(msgType != MessageBox_t::QUESTION_MESSAGE);

    MessageBox_t dialog(0, 0, title, message, msgType, MessageBox_t::PLAIN_OPTION);
    dialog.setCoor();
    dialog.setButtonMode(MessageBox_t::PLAIN_OPTION);

    EventEngine tmpProcess(EventEngine::getCurrentProcess());
    tmpProcess.setMainFrame(dialog);
    tmpProcess.run(false);

    return dialog.getSelectedButtonValue();
}

string showMessageDialog(int l, int t, const string& title, const string& message, MessageBox_t::_messageType_ msgType)
{
    assert(msgType != MessageBox_t::QUESTION_MESSAGE);

    MessageBox_t dialog(l, t, title, message, msgType, MessageBox_t::PLAIN_OPTION);
    dialog.setButtonMode(MessageBox_t::PLAIN_OPTION);

    EventEngine tmpProcess(EventEngine::getCurrentProcess());
    tmpProcess.setMainFrame(dialog);
    tmpProcess.run(false);

    return dialog.getSelectedButtonValue();
}

//Option dialog
string showOptionDialog(const string& message, MessageBox_t::_messageType_ msgType, MessageBox_t::_optionType_ optType)
{
    assert(msgType != MessageBox_t::ERROR_MESSAGE);

    MessageBox_t dialog(0, 0, message, msgType);
    dialog.setCoor();
    dialog.setButtonMode(optType);

    EventEngine tmpProcess(EventEngine::getCurrentProcess());
    tmpProcess.setMainFrame(dialog);
    tmpProcess.run(false);

    return dialog.getSelectedButtonValue();
}

string showOptionDialog(int l, int t, const string& message, MessageBox_t::_messageType_ msgType, MessageBox_t::_optionType_ optType)
{
    assert(msgType != MessageBox_t::ERROR_MESSAGE);

    MessageBox_t dialog(l, t, message, msgType);
    dialog.setButtonMode(optType);

    EventEngine tmpProcess(EventEngine::getCurrentProcess());
    tmpProcess.setMainFrame(dialog);
    tmpProcess.run(false);

    return dialog.getSelectedButtonValue();
}

string showOptionDialog(const string& title, const string& message, MessageBox_t::_messageType_ msgType, MessageBox_t::_optionType_ optType)
{
    assert(msgType != MessageBox_t::ERROR_MESSAGE);

    MessageBox_t dialog(0, 0, title, message, msgType, optType);
    dialog.setCoor();
    dialog.setButtonMode(optType);

    EventEngine tmpProcess(EventEngine::getCurrentProcess());
    tmpProcess.setMainFrame(dialog);
    tmpProcess.run(false);

    return dialog.getSelectedButtonValue();
}

string showOptionDialog(int l, int t, const string& title, const string& message, MessageBox_t::_messageType_ msgType, MessageBox_t::_optionType_ optType)
{
    assert(msgType != MessageBox_t::ERROR_MESSAGE);

    MessageBox_t dialog(l, t, title, message, msgType, optType);
    dialog.setButtonMode(optType);

    EventEngine tmpProcess(EventEngine::getCurrentProcess());
    tmpProcess.setMainFrame(dialog);
    tmpProcess.run(false);

    return dialog.getSelectedButtonValue();
}
