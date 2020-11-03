#include "Panel_t.h"

Panel_t::Panel_t() : Rect_t(), currentFocusItem(NULL) {
  setDefaultColor();
}

Panel_t::Panel_t(int l, int t, COORD cor)
: Panel_t(BorderFactory::DOUBLE, l, t, cor, "") {}

Panel_t::Panel_t(int borderType, int l, int t, COORD cor)
: Panel_t(borderType, l, t, cor, "") {}

Panel_t::Panel_t(int borderType, int l, int t, COORD cor, const string& title)
: Rect_t(borderType, l, t, cor, title),
	currentFocusItem(NULL) {
  setDefaultColor();
}

void Panel_t::setFirstDrawFlag(bool flag) {
  firstDrawFlag = flag;

  for (int i = 0; i < items.size(); ++i) {
    items[i]->setFirstDrawFlag(flag);
  }
}

void Panel_t::setBuffer() {
  canUpdateBufferFlag = true;

  buffer = new BufferInfo(left, top, cord);

  buffer->update(*Graphics::getCurrentActiveGraphics());
}

void Panel_t::getBuffer(Graphics& g) {
  buffer->get(g);

  canUpdateBufferFlag = true;
  closeFlag = false;
  firstDrawFlag = true;
}

void Panel_t::setCurrentFocus(Rect_t *rect) {
  currentFocusItem = rect;

  Rect_t *fpanel = getPanelContainer();

  if (fpanel != NULL) {
    fpanel->setCurrentFocus(this);
  }
}

void Panel_t::setDefaultColor() {
  setColor(0x3f);
}

Rect_t* Panel_t::getCurrentFocus() {
  if (currentFocusItem == NULL) return this;

  if (!currentFocusItem->canGetFocus()) {
    for (int i = 0; i < items.size(); ++i) {
      if (items[i]->canGetFocus()) {
        currentFocusItem = items[i];
        break;
      }
    }
  }

	return currentFocusItem->getCurrentFocus();
}

void Panel_t::draw(Graphics& g) {
	if (canUpdateBufferFlag) {
    assert(buffer != NULL);

    buffer->update(g);
    canUpdateBufferFlag = false;
	}

  if (firstDrawFlag) {
    g.setColor(attribute);
    border->draw(left, top, cord, titleRect, g);

    for (int i = 0; i < items.size(); ++i) {
      if (items[i] != NULL) {
        items[i]->draw(g);
      }
    }

    if (currentFocusItem != NULL && !currentFocusItem->canGetFocus()) {
      for (int i = 0; i < items.size(); ++i) {
        if (items[i]->canGetFocus()) {
          currentFocusItem = items[i];
          break;
        }
      }
    }

    firstDrawFlag = false;
  }

  if (currentFocusItem != NULL) {
    currentFocusItem->draw(g);
  }
}

void Panel_t::add(Rect_t* rect) {
  assert(rect != NULL);

  items.push_back(rect);

  rect->setPanelContainer(this);

  currentFocusItem = items[0];
}

void Panel_t::setCoor(int l, int t, COORD cor) {
  Rect_t::setCoor(l, t, cor);

  if (buffer != NULL) {
    buffer->setCoor(left, top, cord);
    canUpdateBufferFlag = true;
  }
}

void Panel_t::setCoor(int l, int t) {
  setCoor(l, t, this->cord);

  if (buffer != NULL) {
    buffer->setCoor(left, top, cord);
    buffer->update(*Graphics::getCurrentActiveGraphics());
  }
}

void Panel_t::setCoor() {
  COORD maxWd = Graphics::getCurrentActiveGraphics()->getMaxCoord();

  Rect_t::setCoor(maxWd.X / 2 - cord.X / 2, maxWd.Y / 2 - cord.Y / 2, cord);

  if (buffer != NULL) {
    buffer->setCoor(left, top, cord);
    buffer->update(*Graphics::getCurrentActiveGraphics());
    canUpdateBufferFlag = true;
  }
}

bool Panel_t::mouseClick(int l, int t, Graphics &g) {
  if (items.empty()) return false;

	for (int i = 0; i < items.size(); ++i) {
    Rect_t *it = items[i];

    if (it != NULL) {
      if (it->mouseClick(l, t, g)) {
        setCurrentFocus(it);
        return true;
      }
    }
	}

	return false;
}

void Panel_t::getAllRects(Vector<Rect_t *> &rects) {
  rects = items;
}
