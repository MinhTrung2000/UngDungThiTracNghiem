#ifndef MENU_T_H_INCLUDED
#define MENU_T_H_INCLUDED

#include "Graphics.h"
#include "Panel_t.h"
#include "Label_t.h"
#include "MessageBox_t.h"
#include "Panel_t.h"
#include "../view/UIForm.h"

class Menu_t : public Rect_t
{
protected:
    Label_t lbMenu;

    RadioBox_t menuItems;

    Rect_t *menuBar;

    Vector<Panel_t *> panelPtrs;

    bool menuOpenFlag;
    bool oneClickMode;

    uint8_t focusLabelAttr;
    uint8_t unFocusLabelAttr;

    virtual void processPanel(Panel_t *pn);

    friend class MenuBar_t;

public:
    Menu_t();

    //one click mode
    Menu_t(const string& label);

    Menu_t(const string& label, const Vector<string>& itemList);

    void collapse(Graphics& g);

    void setPanelLinkAtIndex(int id, Panel_t *ptr);

    void setColorLabel(uint8_t focusAttr, uint8_t unFocusAttr);

    void setColorMenuItem(uint8_t focusAttr, uint8_t unFocusAttr);

    void setFocus();

    void setCoor(int l, int t);

    int getSelectedIndex();

    string getSelectedValue();

    string getMenuLabelValue();

    void draw(Graphics& g);

    void releaseFocus(Graphics& g);

    bool mouseClick(int l, int t, Graphics& g);

    bool mouseScroll(int state, Graphics& g);

    void keyPress(int keyCode, char ch, Graphics& g);

    bool canGetFocus()
    {
        return false;
    }

    Menu_t& operator = (const Menu_t& o)
    {
        Rect_t::operator=(o);

        lbMenu = o.lbMenu;

        menuItems = o.menuItems;

        menuBar = o.menuBar;

        panelPtrs = o.panelPtrs;

        menuOpenFlag = o.menuOpenFlag;
        oneClickMode = o.oneClickMode;

        focusLabelAttr = o.focusLabelAttr;
        unFocusLabelAttr = o.unFocusLabelAttr;

        return *this;
    }
};

#endif // MENU_T_H_INCLUDED
