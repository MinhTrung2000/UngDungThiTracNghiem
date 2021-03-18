#ifndef MENUBAR_T_H_INCLUDED
#define MENUBAR_T_H_INCLUDED

#include "Menu_t.h"

class MenuBar_t : public Rect_t
{
private:
    static const uint8_t DEFAULT_MENUBAR_ATTRIBUTES = 0x9e;
    static const uint8_t DEFAULT_FOCUS_MENU_ATTRIBUTES = 0xe9;
    static const uint8_t DEFAULT_FOCUS_MENUITEM_ATTRIBUTES = 0xf1;

    Vector<Menu_t> menuList;

    //one selected panel to displayed
    Rect_t *displayPanel;

    Menu_t *selectedMenu;

public:
    MenuBar_t();

    void setSelectedMenu(Rect_t *menu)
    {
        selectedMenu = dynamic_cast<Menu_t *> (menu);
    }
    Rect_t *getSelectedMenu()
    {
        return selectedMenu;
    }

    void setDisplayPanel(Rect_t *pn);
    Rect_t *getDisplayPanel()
    {
        return displayPanel;
    }

    void setDefaultColor();

    //if autoArrang = true, menuBar append newMenu to right of the last menu
    void addMenu(Menu_t& newMenu, bool autoArrange = true);

    void draw(Graphics& g);

    bool mouseClick(int l, int t, Graphics& g);

    bool canGetFocus()
    {
        return false;
    }

    MenuBar_t& operator = (const MenuBar_t& o)
    {
        Rect_t::operator=(o);

        menuList = o.menuList;

        displayPanel = o.displayPanel;

        selectedMenu = o.selectedMenu;

        return *this;
    }
};

#endif // MENUBAR_T_H_INCLUDED
