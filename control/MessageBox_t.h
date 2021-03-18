#ifndef MESSAGEBOX_T_H
#define MESSAGEBOX_T_H

#include <cassert>
#include <stdint.h>
#include <string>
#include <sstream>

#include "Button_t.h"

#include "EventEngine.h"
#include "Panel_t.h"

#include "lib_vector.h"

#define WARNING_MESSAGE_ATTRIBUTE 												0xe0

#define WARNING_MESSAGE_BUTTON_FOCUS_ATTRIBUTE 						0xb9
#define WARNING_MESSAGE_BUTTON_UNFOCUS_ATTRIBUTE 					0xf9
//============
#define INFORMATION_MESSAGE_ATTRIBUTE										  0x97

#define INFORMATION_MESSAGE_BUTTON_FOCUS_ATTRIBUTE 			  0xec
#define INFORMATION_MESSAGE_BUTTON_UNFOCUS_ATTRIBUTE			0xfc
//============
#define ERROR_MESSAGE_ATTRIBUTE 													0xce

#define ERROR_MESSAGE_BUTTON_FOCUS_ATTRIBUTE							0xec
#define ERROR_MESSAGE_BUTTON_UNFOCUS_ATTRIBUTE 					  0xfc
//============
#define QUESTION_MESSAGE_ATTRIBUTE 											  0x3f

#define QUESTION_MESSAGE_BUTTON_FOCUS_ATTRIBUTE					  0x5f
#define QUESTION_MESSAGE_BUTTON_UNFOCUS_ATTRIBUTE 				0xfd

class MessageBox_t : public Panel_t
{
    typedef uint8_t _messageType_;
    typedef uint8_t _optionType_;
private:
    static const int MESSAGE_WIDTH_DIFF = 3;

    static const int MAX_MESSAGE_LENGTH = 40;
    static const int BUTTON_PADDING = 5;

    static const uint8_t OPT_COLOR[4];
    static const uint8_t BUTTON_COLOR[4][2];

    static const string MESSAGE_TITLE[4];

    Vector<string> msgLines;

    //color format for button
    uint8_t focusButtonAttr;
    uint8_t unFocusButtonAttr;

    //Message dialog
    friend string showMessageDialog(const string& message, _messageType_ msgType);
    friend string showMessageDialog(int l, int t, const string& message, _messageType_ msgType);

    friend string showMessageDialog(const string& title, const string& message, _messageType_ msgType);
    friend string showMessageDialog(int l, int t, const string& title, const string& message, _messageType_ msgType);

    //Option dialog
    friend string showOptionDialog(const string& message, _messageType_ msgType, _optionType_ optType);
    friend string showOptionDialog(int l, int t, const string& message, _messageType_ msgType, _optionType_ optType);

    friend string showOptionDialog(const string& title, const string& message, _messageType_ msgType, _optionType_ optType);
    friend string showOptionDialog(int l, int t, const string& title, const string& message, _messageType_ msgType, _optionType_ optType);

public:
    static const _messageType_ WARNING_MESSAGE = 0;
    static const _messageType_ INFORMATION_MESSAGE = 1;
    static const _messageType_ ERROR_MESSAGE = 2;
    static const _messageType_ QUESTION_MESSAGE = 3;

    static const _optionType_ YES_NO_CANCEL_OPTION = 0;
    static const _optionType_ OK_CANCEL_OPTION = 1;
    static const _optionType_ PLAIN_OPTION = 2;

    MessageBox_t();
    MessageBox_t(int l, int t, const string& message, _messageType_ msgType);
    MessageBox_t(int l, int t, const string& message, _messageType_ msgType, _optionType_ optType);
    MessageBox_t(int l, int t, const string& title, const string& message, _messageType_ msgType, _optionType_ optType);

    void setCoor(int l, int t);

    void setCoor();

    void setButtonColor(uint8_t focusAttr, uint8_t unFocusAttr);

    //Can use char '\n' to split message to each line
    void setMessage(const string& newMessage, _messageType_ msgType);

    void addButton(const string& btnValue);

    void setButtonMode(_optionType_ optType);

    string getSelectedButtonValue();

    Button_t *getButtonAtId(int id);

    void draw(Graphics& g);

    bool mouseClick(int l, int t, Graphics& g);

    void keyPress(int keyCode, char ch, Graphics& g);

    MessageBox_t& operator = (const MessageBox_t& o)
    {
        Panel_t::operator=(o);

        msgLines = o.msgLines;

        focusButtonAttr = o.focusButtonAttr;
        unFocusButtonAttr = o.unFocusButtonAttr;

        return *this;
    }
};

//Message dialog
string showMessageDialog(const string& message, MessageBox_t::_messageType_ msgType);
string showMessageDialog(int l, int t, const string& message, MessageBox_t::_messageType_ msgType);

string showMessageDialog(const string& title, const string& message, MessageBox_t::_messageType_ msgType);
string showMessageDialog(int l, int t, const string& title, const string& message, MessageBox_t::_messageType_ msgType);

//Option dialog
string showOptionDialog(const string& message, MessageBox_t::_messageType_ msgType, MessageBox_t::_optionType_ optType);
string showOptionDialog(int l, int t, const string& message, MessageBox_t::_messageType_ msgType, MessageBox_t::_optionType_ optType);

string showOptionDialog(const string& title, const string& message, MessageBox_t::_messageType_ msgType, MessageBox_t::_optionType_ optType);
string showOptionDialog(int l, int t, const string& title, const string& message, MessageBox_t::_messageType_ msgType, MessageBox_t::_optionType_ optType);

#endif
