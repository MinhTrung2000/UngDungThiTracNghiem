#include "Menu_t.h"

#include <cassert>

Menu_t::Menu_t()
: Rect_t(),
  menuBar(NULL),
  menuOpenFlag(false),
  oneClickMode(false) {
}

//one click mode
//Menu_t::Menu_t(const string& label, Rect_t *_mnBar)
Menu_t::Menu_t(const string& label)
: lbMenu(BorderFactory::NONE, 0, 0, 2, label),
  menuBar(NULL),
  menuOpenFlag(false),
  oneClickMode(true) {

  panelPtrs.resize(1, NULL);
  Rect_t::setCoor(lbMenu.getLeft(), lbMenu.getTop(), lbMenu.getCord());
}

//Menu_t::Menu_t(const string& label, const Vector<string>& itemList, Rect_t *_mnBar)
Menu_t::Menu_t(const string& label, const Vector<string>& itemList)
: lbMenu(BorderFactory::NONE, 0, 0, 2, label),
  menuBar(NULL),
  menuItems(left + 2, top + 1, itemList.size(), CheckBox_t::TICK_STYLE, itemList),
  menuOpenFlag(false),
  oneClickMode(false) {

  menuItems.setBuffer();
  panelPtrs.resize(itemList.size(), NULL);
  Rect_t::setCoor(lbMenu.getLeft(), lbMenu.getTop(), lbMenu.getCord());
}

void Menu_t::setCoor(int l, int t) {
  lbMenu.setCoor(l, t);
  menuItems.setCoor(l, t + 1);
  Rect_t::setCoor(lbMenu.getLeft(), lbMenu.getTop(), lbMenu.getCord());
  menuItems.setBuffer();
}

int Menu_t::getSelectedIndex() {
  return menuItems.getSelectedIndex();
}

string Menu_t::getSelectedValue() {
  return menuItems.getSelectedValue();
}

void Menu_t::setColorMenuItem(uint8_t focusAttr, uint8_t unFocusAttr) {
  menuItems.setColorEle(focusAttr, unFocusAttr);
  menuItems.setColor(unFocusAttr);
}

void Menu_t::setColorLabel(uint8_t focusAttr, uint8_t unFocusAttr) {
  focusLabelAttr = focusAttr;
  unFocusLabelAttr = unFocusAttr;
  lbMenu.setColor(unFocusLabelAttr);
}

string Menu_t::getMenuLabelValue() {
  return lbMenu.getValue();
}

void Menu_t::draw(Graphics& g) {
  lbMenu.draw(g);
  if (firstDrawFlag) {
    firstDrawFlag = false;
  } else {
    if (!oneClickMode && menuOpenFlag) {
      menuItems.draw(g);
    }
  }
}

void Menu_t::setPanelLinkAtIndex(int id, Panel_t *ptr) {
  assert(0 <= id && id < panelPtrs.size());

  panelPtrs[id] = ptr;
}

void Menu_t::collapse(Graphics& g) {
  if (! oneClickMode && menuOpenFlag) {
    menuItems.hide(g);
  }

  menuOpenFlag = false;

  lbMenu.setColor(unFocusLabelAttr);

  draw(g);
}

void Menu_t::processPanel(Panel_t* pn) {
  pn->setFirstDrawFlag(true);

  if (dynamic_cast<UIForm *>(pn) != NULL) {
    if (lbMenu.getValue() != "Dang Xuat") {
      UIForm::drawForm(((UIForm *) pn), false);
    } else {
      menuBar->getPanelContainer()->setCloseFlag(true);
    }
  } else {
    pn->draw(*Graphics::getCurrentActiveGraphics());
  }
}

void Menu_t::setFocus() {
  lbMenu.setColor(focusLabelAttr);
}

bool Menu_t::mouseClick(int l, int t, Graphics& g) {
  bool clickAtLabel = g.isInside(l, t, left, top, cord.X - 1, 0);

  if (!(clickAtLabel || menuOpenFlag)) return false;

  //if mouse click at label, not item list
  Menu_t *preSelectedMenu = dynamic_cast<Menu_t *>(menuBar->getSelectedMenu());
  assert(preSelectedMenu != NULL);

  Rect_t *currentFocusRect = Rect_t::getFocus();
  bool isMenuCurrFocusRect = dynamic_cast<Menu_t *>(currentFocusRect);

  if (clickAtLabel) {

  //Truong hop khi click 1 menu label X ma X co oneClickMode = true hoac co menuOpenFlag = false
  //Neu X khac preSelectedMenu
  //1. Tu preSelectedMenu den X (hoac den rect(khong la menu) den X)
  //2. Tu preSelectedMenu den menu khac den X
  //Neu x la preSelectedMenu (tuong tu)
  //1. Tu preSelectedMenu den X (hoac den rect(khong la menu) den X)
  //2. Tu preSelectedMenu den menu khac den X

    //one click menu type
    if (oneClickMode) {
      Panel_t *pn = panelPtrs[0];
      assert(pn != NULL);

      if (preSelectedMenu != this) {
        if (isMenuCurrFocusRect && currentFocusRect != preSelectedMenu) {
          ((Menu_t *) currentFocusRect)->collapse(g);
        }

        preSelectedMenu->menuItems.setClearAll();
        preSelectedMenu->collapse(g);

        menuBar->setSelectedMenu(this);
      } else if (isMenuCurrFocusRect && currentFocusRect != this) {
        ((Menu_t *) currentFocusRect)->collapse(g);
      }

      lbMenu.setColor(focusLabelAttr);

      if (dynamic_cast<Panel_t *> (menuBar->getDisplayPanel()) != pn) {
        menuBar->setDisplayPanel(pn);
        processPanel(pn);
      }

      draw(g);
      Rect_t::setFocus(*pn->getCurrentFocus());
      return true;
    }

    if(menuOpenFlag) {
      //menu close next time
      menuOpenFlag = false;
      if (! oneClickMode) {
        menuItems.hide(g);
      }

      if (preSelectedMenu != this) {
        lbMenu.setColor(unFocusLabelAttr);
        draw(g);

        preSelectedMenu->lbMenu.setColor(focusLabelAttr);
        Rect_t::setFocus(*preSelectedMenu);
        return true;
      }

      Rect_t::setFocus(*this);
      return true;
    } else {
      //menu open next time
      menuOpenFlag = true;

      if (preSelectedMenu != this) {
        if (isMenuCurrFocusRect && currentFocusRect != preSelectedMenu) {
          ((Menu_t *) currentFocusRect)->collapse(g);
        }

        preSelectedMenu->collapse(g);

      } else if (isMenuCurrFocusRect && currentFocusRect != this) {
        ((Menu_t *) currentFocusRect)->collapse(g);
      }

      lbMenu.setColor(focusLabelAttr);

      Rect_t::setFocus(*this);
      return true;
    }
  }

  //mouse click at item list
  if (menuOpenFlag) {
    int preSelectedMenuItemIndex = menuItems.getSelectedIndex();

    if (menuItems.mouseClick(l, t, g)) {
      menuOpenFlag = false;
      menuItems.hide(g);

      draw(g);

      int newSelectedMenuItemIndex = menuItems.getSelectedIndex();

      if (newSelectedMenuItemIndex != -1 && preSelectedMenuItemIndex != newSelectedMenuItemIndex) {
        Panel_t *pn = panelPtrs[getSelectedIndex()];
        assert(pn != NULL);

        if (preSelectedMenu != this) {
          preSelectedMenu->menuItems.setClearAll();
          preSelectedMenu->collapse(g);

          menuBar->setSelectedMenu(this);
        }

        if (dynamic_cast<Panel_t *> (menuBar->getDisplayPanel()) != pn) {
          menuBar->setDisplayPanel(pn);
          processPanel(pn);
          Rect_t::setFocus(*pn->getCurrentFocus());
        }

      } else {
        //if click at current selected menu item, recover checked
        menuItems.setSelectedIndex(preSelectedMenuItemIndex);
        Rect_t::setFocus(*this);
      }

      return true;
    }
  }

  return false;
}

bool Menu_t::mouseScroll(int state, Graphics& g) {
  if (menuOpenFlag){
    if (menuItems.mouseScroll(state, g))
      return true;
  }

  return false;
}

void Menu_t::releaseFocus(Graphics& g) {
  if (dynamic_cast<Menu_t *>(Rect_t::getFocus())) return;

  if (!menuOpenFlag) return;

  menuOpenFlag = false;

  if (! oneClickMode) {
    menuItems.hide(g);
  }

  draw(g);

  Menu_t *preSelectedMenu = dynamic_cast<Menu_t *>(menuBar->getSelectedMenu());
  assert(preSelectedMenu != NULL);

  if (preSelectedMenu != this) {
    lbMenu.setColor(unFocusLabelAttr);
    draw(g);

    preSelectedMenu->lbMenu.setColor(focusLabelAttr);
    preSelectedMenu->draw(g);
  }
}

void Menu_t::keyPress(int keyCode, char ch, Graphics& g) {
  Menu_t *preSelectedMenu = dynamic_cast<Menu_t *>(menuBar->getSelectedMenu());
  assert(preSelectedMenu != NULL);

  Rect_t *currentFocusRect = Rect_t::getFocus();
  bool isMenuCurrFocusRect = dynamic_cast<Menu_t *>(currentFocusRect);

  int preSelectedMenuItemIndex = menuItems.getSelectedIndex();

  switch(keyCode) {
    //press enter at label or menu item
    case KEY_SPACE: case KEY_ENTER: {
      //at label
      if (!menuOpenFlag) {
        menuOpenFlag = true;

        if (preSelectedMenu != this) {
          if (isMenuCurrFocusRect && currentFocusRect != preSelectedMenu) {
            ((Menu_t *) currentFocusRect)->collapse(g);
          }

          preSelectedMenu->collapse(g);

        } else if (isMenuCurrFocusRect && currentFocusRect != this) {
          ((Menu_t *) currentFocusRect)->collapse(g);
        }

        lbMenu.setColor(focusLabelAttr);
      } else {
        //at menu item
        menuOpenFlag = false;
        menuItems.hide(g);

        int newSelectedMenuItemIndex = menuItems.getSelectedIndex();

        if (newSelectedMenuItemIndex != -1 && preSelectedMenuItemIndex != newSelectedMenuItemIndex) {
          Panel_t *pn = panelPtrs[getSelectedIndex()];
          assert(pn != NULL);

          if (preSelectedMenu != this) {
            preSelectedMenu->menuItems.setClearAll();
            preSelectedMenu->collapse(g);

            menuBar->setSelectedMenu(this);
          }

          if (dynamic_cast<Panel_t *> (menuBar->getDisplayPanel()) != pn) {
            menuBar->setDisplayPanel(pn);
            processPanel(pn);
          }

        } else {
          //if click at current selected menu item, recover checked
          menuItems.setSelectedIndex(preSelectedMenuItemIndex);
        }
      }
      break;
    }
    case KEY_ESCAPE: {
      if (menuOpenFlag) {
        menuOpenFlag = false;
        if (! oneClickMode) {
          menuItems.hide(g);
        }

        if (preSelectedMenu != this) {
          lbMenu.setColor(unFocusLabelAttr);
          draw(g);

          preSelectedMenu->lbMenu.setColor(focusLabelAttr);
          Rect_t::setFocus(*preSelectedMenu);
        }
      }
      break;
    }
    default: {
      menuItems.keyPress(keyCode, ch, g);
    }
  }
}

