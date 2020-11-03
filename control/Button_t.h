#ifndef BUTTON_T_H
#define BUTTON_T_H

#include <string>
#include <windows.h>
#include <iostream>

#include "Graphics.h"
#include "Rect_t.h"

class Button_t : public Rect_t {
protected:
	string value;

  friend class ButtonList_t;
  friend class MessageBox_t;

public:
	Button_t();
	Button_t(const string& val);
	Button_t(int l, int t, const string& val);

	void setCoor(int l, int t);

  void setDefaultColor();

	void setValue(const string& newVal);

	string getValue();

	void draw(Graphics& g);

	bool mouseClick(int l, int t, Graphics &g);

	Button_t& operator = (const Button_t& o) {
    Rect_t::operator=(o);
    value = o.value;
    return *this;
	}
};

class ButtonList_t {
private:
  Vector<Button_t *> btnList;
  int currentIndex;

public:
  ButtonList_t() : currentIndex(-1) {}

  ~ButtonList_t() {}

  void setCurrentIndex(Button_t *btn) {
    for (int i = 0; i < btnList.size(); ++i) {
      if (btnList[i] == btn) {
        if (currentIndex != i) {
          btnList[currentIndex]->setColor(DEFAULT_BUTTON_UNFOCUS_ATTRIBUTES);
          btnList[currentIndex]->draw(*Graphics::getCurrentActiveGraphics());
          currentIndex = i;
          btnList[currentIndex]->setColor(DEFAULT_BUTTON_FOCUS_ATTRIBUTES);
          btnList[currentIndex]->draw(*Graphics::getCurrentActiveGraphics());
        }
        return;
      }
    }
    assert(false);
  }

  void push_back(Button_t *btnPtr) {
    btnList.push_back(btnPtr);
    if (currentIndex == -1)
      currentIndex = 0;
  }

  bool movePrev(Graphics& g) {
    if (currentIndex <= 0) return false;

    for (int i = currentIndex - 1; i >= 0; --i) {
      if (btnList[i]->canGetFocus()) {
        btnList[currentIndex]->setColor(DEFAULT_BUTTON_UNFOCUS_ATTRIBUTES);
        btnList[currentIndex]->draw(g);

        currentIndex = i;
        btnList[currentIndex]->setColor(DEFAULT_BUTTON_FOCUS_ATTRIBUTES);
        Rect_t::setFocus(*btnList[currentIndex]);
        return true;
      }
    }
    return false;
  }

  bool moveNext(Graphics& g) {
    if (currentIndex <= 0) return false;

    for (int i = currentIndex + 1; i < btnList.size(); ++i) {
      if (btnList[i]->canGetFocus()) {
        btnList[currentIndex]->setColor(DEFAULT_BUTTON_UNFOCUS_ATTRIBUTES);
        btnList[currentIndex]->draw(g);

        currentIndex = i;
        btnList[currentIndex]->setColor(DEFAULT_BUTTON_FOCUS_ATTRIBUTES);
        Rect_t::setFocus(*btnList[currentIndex]);
        return true;
      }
    }
    return false;
  }

  ButtonList_t& operator = (const ButtonList_t& rhs) {
    btnList = rhs.btnList;
    currentIndex = rhs.currentIndex;
    return *this;
  }
};

#endif
