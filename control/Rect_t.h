#ifndef RECT_T_H
#define RECT_T_H

#include <stdint.h>
#include <cassert>

#include "Graphics.h"
#include "Border_t.h"

#include "lib_vector.h"

using namespace std;

class Rect_t {
private:
	static Rect_t *focusObj;

protected:
  static const uint8_t IGNORE_FOCUS_ATTRIBUTES = 0xf7;
  static const uint8_t DEFAULT_RECT_ATTRIBUTES = 0xf0;

	string titleRect;
	int left;
	int top;
	COORD cord;
	Border_t *border;
  BufferInfo *buffer;
  Rect_t *pnContainer;

	COORD currentPosition;

  bool closeFlag;
  bool hideFlag;
  bool ableFocusFlag;
  bool canDrawFlag;
  bool firstDrawFlag;
  bool canUpdateBufferFlag;

  uint8_t attribute;
  uint8_t saveFocusAttribute;

  Rect_t& operator = (const Rect_t& o) {
    titleRect = o.titleRect;
    left = o.left;
    top = o.top;
    cord = o.cord;
    border = o.border;
    buffer = o.buffer;

    currentPosition = o.currentPosition;

    closeFlag = o.closeFlag;
    hideFlag = o.hideFlag;
    ableFocusFlag = o.ableFocusFlag;
    canDrawFlag = o.canDrawFlag;
    firstDrawFlag = o.firstDrawFlag;
    canUpdateBufferFlag = o.canUpdateBufferFlag;

    attribute = o.attribute;
    saveFocusAttribute = o.saveFocusAttribute;
    return *this;
	}

public:
	Rect_t();
	Rect_t(int l, int t, const string& newTitle = "");
	Rect_t(int l, int t, COORD cor, const string& newTitle = "");
	Rect_t(int borderType, int l, int t, const string& newTitle = "");
	Rect_t(int borderType, int l, int t, COORD cor, const string& newTitle = "");

	virtual ~Rect_t();

	virtual bool isFocus() { return Rect_t::getFocus() == this; }

	virtual void setTitle(const string& newTitle);
	virtual void setCoor(int l, int t, COORD cor);
	virtual void setCoor(int l, int t) {}
	virtual void setCoor() {}
  virtual void setBorder(int borderType);
  virtual void setBuffer() {}
	virtual void setPanelContainer(Rect_t *fpanel);

	Rect_t *getPanelContainer() { return pnContainer; }

	int getLeft() { return left; }
	int getTop() { return top; }
	uint8_t getAttribute() { return attribute; }
	COORD getCord() { return cord; }

	virtual void setFirstDrawFlag(bool flag);

	virtual void setCurrentPosition(int l, int t);
	COORD getCurrentPosition() { return currentPosition; }

	virtual void setCloseFlag(bool flag) { closeFlag = flag; }
  bool getCloseFlag() { return closeFlag; }

  virtual void setHideFlag(bool flag) { hideFlag = flag; }
  bool getHideFlag() { return hideFlag; }

	virtual void setAbleFocus(bool flag);

	void setColor(uint8_t attr);
  virtual void setDefaultColor() {}

	virtual void draw(Graphics& g) {}

	virtual void hide(Graphics& g) {}

	virtual void close(Graphics& g) {}

	virtual bool mouseClick(int l, int t, Graphics &g) { return false; }

	virtual bool mouseScroll(int state, Graphics &g) { return false; }

	virtual void keyPress(int keyCode, char ch, Graphics& g) {}

  virtual void getAllRects(Vector<Rect_t *> &rects) {}

	virtual bool canGetFocus() { return ableFocusFlag; }

	virtual bool IsCursorVisible() { return false; }

  virtual void releaseFocus(Graphics& g) {}

  //support timer
  virtual void getActionTimeEnding() {}

  //support panel
  virtual void setCurrentFocus(Rect_t *rect) {}
  virtual Rect_t *getCurrentFocus() { return this; }

  //only use for menubar
  virtual void setDisplayPanel(Rect_t *panel) {}
  virtual Rect_t *getDisplayPanel() { return NULL; }

  virtual void setSelectedMenu(Rect_t *menu) {}
  virtual Rect_t *getSelectedMenu() { return NULL; }

	//=====================
  static void setFocus(Rect_t& rect) { focusObj = &rect; }

	static Rect_t* getFocus() { return focusObj; }
};

#endif
