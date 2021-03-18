#include "MenuBar_t.h"

MenuBar_t::MenuBar_t()
    : selectedMenu(NULL),
      displayPanel(NULL)
{

    Rect_t::setCoor(0, 0, {170, 2});
    border = BorderFactory::makeBorder(BorderFactory::NONE);
    setDefaultColor();
}

void MenuBar_t::setDisplayPanel(Rect_t* pn)
{
    assert(pn != NULL);
    displayPanel = pn;
}

void MenuBar_t::setDefaultColor()
{
    setColor(DEFAULT_MENUBAR_ATTRIBUTES);
}

void MenuBar_t::addMenu(Menu_t& newMenu, bool autoArrange)
{
    menuList.push_back(newMenu);

    int n = menuList.size();

    if (autoArrange)
    {
        if (n == 1)
        {
            menuList[n - 1].setCoor(left + 1, top);
        }
        else
        {
            menuList[n - 1].setCoor(menuList[n - 2].getLeft()
                                    + menuList[n - 2].getCord().X + 1, top);
        }
    }

    menuList[n - 1].menuBar = this;
    menuList[n - 1].setColorLabel(DEFAULT_FOCUS_MENU_ATTRIBUTES, DEFAULT_MENUBAR_ATTRIBUTES);
    menuList[n - 1].setColorMenuItem(DEFAULT_FOCUS_MENUITEM_ATTRIBUTES, DEFAULT_MENUBAR_ATTRIBUTES);

    selectedMenu = &menuList[0];
}

void MenuBar_t::draw(Graphics& g)
{
    if (selectedMenu != NULL)
    {
        selectedMenu->setFocus();
    }

    g.setColor(attribute);
    g.write(left, top, string(cord.X + 2, ' '));
    g.write(left, top, "|");

    for (int i = 0; i < menuList.size(); ++i)
    {
        menuList[i].draw(g);
        g.setColor(attribute);
        g.write(menuList[i].getLeft() + menuList[i].getCord().X, top, "|");
    }

    if (displayPanel != NULL)
        displayPanel->draw(g);
}

bool MenuBar_t::mouseClick(int l, int t, Graphics& g)
{
    for (int i = 0; i < menuList.size(); ++i)
    {
        if (menuList[i].mouseClick(l, t, g))
        {
            return true;
        }
    }
    return false;
}
