#include "Rect_t.h"

Rect_t* Rect_t::focusObj = NULL;

Rect_t::Rect_t() : Rect_t(BorderFactory::NONE, 0, 0, {0, 0}, "") {}

Rect_t::Rect_t(int l, int t, const string& newTitle)
: Rect_t(BorderFactory::NONE, l, t, {0, 0}, newTitle) {}

Rect_t::Rect_t(int l, int t, COORD cor, const string& newTitle)
: Rect_t(BorderFactory::NONE, l, t, cor, newTitle) {}

Rect_t::Rect_t(int borderType, int l, int t, const string& newTitle)
: Rect_t(borderType, l, t, {0, 0}, newTitle) {}

Rect_t::Rect_t(int borderType, int l, int t, COORD cor, const string& newTitle)
: titleRect(newTitle),
  left(l),
	top(t),
	cord(cor),
	border(BorderFactory::makeBorder(borderType)),
	buffer(NULL),
	pnContainer(NULL),

	closeFlag(false),
	hideFlag(false),
	ableFocusFlag(true),
	canDrawFlag(true),
	firstDrawFlag(true),
	canUpdateBufferFlag(false),

	attribute(DEFAULT_RECT_ATTRIBUTES) {

}

Rect_t::~Rect_t() {}

void Rect_t::setFirstDrawFlag(bool flag) {
  firstDrawFlag = flag;
}

void Rect_t::setCurrentPosition(int l, int t) {
  currentPosition = {l, t};
}

void Rect_t::setTitle(const string& newTitle) {
  titleRect = newTitle;
}

void Rect_t::setColor(uint8_t attr) {
  attribute = attr;
}

void Rect_t::setBorder(int borderType) {
  border = BorderFactory::makeBorder(borderType);
}

void Rect_t::setAbleFocus(bool flag) {
  //set flag + redraw
  if (ableFocusFlag != flag) {
    //if rect is currently focusable, set it to unfocusable. And otherwise...
    if (ableFocusFlag) {
      saveFocusAttribute = this->attribute;
      setColor(IGNORE_FOCUS_ATTRIBUTES);
    } else {
      setColor(saveFocusAttribute);
    }
    ableFocusFlag = flag;
  }
}

void Rect_t::setCoor(int l, int t,  COORD c) {
	left = l;
	top = t;
	cord = c;
}

void Rect_t::setPanelContainer(Rect_t* fpanel) {
  pnContainer = fpanel;
}
