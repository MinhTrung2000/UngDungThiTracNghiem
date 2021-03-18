#ifndef CHECKLIST_T_H
#define CHECKLIST_T_H

#include <string>
#include <stdint.h>

#include "CheckBox_t.h"
#include "Label_t.h"
#include "lib_vector.h"

using namespace std;

class CheckList_t : public Rect_t
{
protected:
    static const uint8_t DEFAULT_CHECKLIST_ATTRIBUTES = 0xf0;
    static const uint8_t DEFAULT_CHECKLIST_FOCUS_ROW_ATTRIBUTES = 0xb2;
    static const uint8_t DEFAULT_CHECKLIST_UNFOCUS_ROW_ATTRIBUTES = 0xf0;

    static const int DEFAULT_MAX_WIDTH = 200;

    struct ViewPort
    {
        int lineNumber;

        int currentTopIndex;
        int currentBottomIndex;

        bool drawAllLineFlag;

        void reset()
        {
            currentTopIndex = 0;
            currentBottomIndex = lineNumber - 1;
            drawAllLineFlag = true;
        }

        virtual void setCanUpdateBufferFlag(bool flag) {}

        virtual bool getCanUpdateBufferFlag()
        {
            return false;
        }

        virtual void updateBuffer(Graphics& g) {}

        virtual void getBuffer(Graphics& g) {}

        ViewPort(int lineNum)
        {
            assert(lineNum >= 0);

            currentTopIndex = 0;
            currentBottomIndex = lineNum - 1;
            lineNumber = lineNum;

            drawAllLineFlag = true;
        }
        virtual ~ViewPort() {}

        ViewPort& operator = (const ViewPort& rhs)
        {
            lineNumber = rhs.lineNumber;
            currentTopIndex = rhs.currentTopIndex;
            currentBottomIndex = rhs.currentBottomIndex;
            drawAllLineFlag = rhs.drawAllLineFlag;

            return *this;
        }
    };

    struct BufferViewPort : ViewPort
    {
        BufferInfo buffer;
        bool canUpdateBufferFlag;

        void setCanUpdateBufferFlag(bool flag)
        {
            canUpdateBufferFlag = flag;
        }

        bool getCanUpdateBufferFlag()
        {
            return canUpdateBufferFlag;
        }

        void updateBuffer(Graphics& g)
        {
            buffer.update(g);
        }

        void getBuffer(Graphics& g)
        {
            buffer.get(g);
        }

        BufferViewPort(int lineNum) : ViewPort(lineNum), canUpdateBufferFlag(true) {}

        BufferViewPort& operator = (const BufferViewPort& rhs)
        {
            ViewPort::operator=(rhs);
            buffer = rhs.buffer;
            canUpdateBufferFlag = rhs.canUpdateBufferFlag;

            return *this;
        }
    };

    Vector<Label_t *> allCheckBoxes;

    int previousIndex;
    int currentIndex;

    int maxWidthItem;

    uint8_t focusEleAttr;
    uint8_t unFocusEleAttr;

    //support table_t
    string emptyLineFormatTable;

    short cbStyle;

    ViewPort *port;

    bool isBufferViewPort();

    void setDrawAllLinePort(bool flag);

    virtual void drawLineByPort(int id, Graphics& g);
    virtual void drawByPort(Graphics& g);

    virtual bool mouseClickByPort(int l, int t, Graphics& g);
    virtual bool mouseScrollByPort(int state, Graphics& g);
    virtual void keyPressByPort(int keyCode, char ch, Graphics& g);

    friend class Table_t;
    friend class CheckListBox_t;

public:
    CheckList_t();
    CheckList_t(int l, int t);
    CheckList_t(int l, int t, const Vector<string>& str);
    CheckList_t(int l, int t, CheckBox_t::_checkBoxStyle_ cbStyle);
    CheckList_t(int l, int t, CheckBox_t::_checkBoxStyle_ cbStyle, const Vector<string>& str);

    CheckList_t(int l, int t, int portLineNumber);
    CheckList_t(int l, int t, int portLineNumber, const Vector<string>& str);

    CheckList_t(int borderType, int l, int t, int portLineNumber);
    CheckList_t(int borderType, int l, int t, int portLineNumber, const Vector<string>& str);

    CheckList_t(int l, int t, int portLineNumber, CheckBox_t::_checkBoxStyle_ cbStyle);
    CheckList_t(int l, int t, int portLineNumber, CheckBox_t::_checkBoxStyle_ cbStyle, const Vector<string>& str);

    CheckList_t(int borderType, int l, int t, int portLineNumber, CheckBox_t::_checkBoxStyle_ cbStyle, const Vector<string>& str);

    bool empty()
    {
        return allCheckBoxes.empty();
    }

    void setEmptyLineFormatTable(const string& lineFormat);

    virtual void setFirstDrawFlag(bool flag);

    virtual void setMaxWidthItem(int wid);

    virtual void hide(Graphics& g);

    virtual void reset();

    virtual void setBuffer();

    virtual void setColorEle(uint8_t focusAttr, uint8_t unFocusAttr);

    virtual void setDefaultColor();

    virtual void setFirstSelected();

    //hight light row at id
    virtual void setFocusAtIndex(int id);

    //set check and focus to item id
    virtual void setSelectedIndex(int id);

    virtual void setToggleAll(bool drawFlag = true);

    virtual void setClearAll(bool drawFlag = true);

    virtual void setData(const Vector<string>& newData);

    //check if get at least one check
    bool havingCheck();

    int getPortLineNum()
    {
        return port->lineNumber;
    }

    Vector<int> getSelectedIndexList();

    Vector<string> getSelectedValueList();

    //?
    string getCheckBoxValueAtIndex(int id);

    void setCoor(int l, int t);

    virtual void draw(Graphics& g);

    virtual bool mouseScroll(int state, Graphics& g);

    virtual bool mouseClick(int left, int top, Graphics &g);

    virtual void keyPress(int keyCode, char ch, Graphics &g);

    void getAllRects(Vector<Rect_t *> &rects);

    //transaction
    virtual void insertNewItem(const string& newItem);

    virtual void insertNewItem(int id, const string& newItem);

    virtual void deleteItem(int id);

    virtual void replaceItem(int id, const string& newItemReplaced);

    virtual void clearAll();

    CheckList_t& operator = (const CheckList_t& o)
    {
        Rect_t::operator=(o);

        allCheckBoxes = o.allCheckBoxes;

        previousIndex = o.previousIndex;
        currentIndex = o.currentIndex;

        maxWidthItem = o.maxWidthItem;

        focusEleAttr = o.focusEleAttr;
        unFocusEleAttr = o.unFocusEleAttr;

        emptyLineFormatTable = o.emptyLineFormatTable;

        cbStyle = o.cbStyle;

        port = o.port;

        return *this;
    }
};

#endif
