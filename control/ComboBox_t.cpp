#include "ComboBox_t.h"

const string ComboBox_t::EMPTY_VALUE = "<Empty>   ";

ComboBox_t::ComboBox_t()
:  Rect_t(),
   menuOpenFlag(false),
   onlyDrawMenuFlag(false) {
}

ComboBox_t::ComboBox_t(int l, int t, const Vector<string>& options)
: ComboBox_t(l, t, options.size(), options) {}

ComboBox_t::ComboBox_t(int l, int t, int lineNumber, const Vector<string>& options)
: comboOptions(l, t + 3, std::min(lineNumber, options.size()), CheckBox_t::TICK_STYLE, options),
  menuOpenFlag(false),
  onlyDrawMenuFlag(false) {

  comboOptions.setBuffer();
  comboOptions.setForceChecked();
  comboOptions.setFirstSelected();
	setBorder(BorderFactory::SINGLE);
  setCoor(l, t);
	setDefaultColor();
}

ComboBox_t::ComboBox_t(int l, int t)
: ComboBox_t(l, t, 3) {}

ComboBox_t::ComboBox_t(int l, int t, int lineNumber)
: comboOptions(l, t + 3, lineNumber, CheckBox_t::TICK_STYLE),
  menuOpenFlag(false),
  onlyDrawMenuFlag(false) {

  comboOptions.setBuffer();
  comboOptions.setForceChecked();
  comboOptions.setFirstSelected();
	setBorder(BorderFactory::SINGLE);
  setCoor(l, t);
	setDefaultColor();
}

void ComboBox_t::setColorOptions(uint8_t focusAttr, uint8_t unFocusAttr) {
  comboOptions.setColorEle(focusAttr, unFocusAttr);
}

void ComboBox_t::setData(const Vector<string>& newData) {
  comboOptions.setData(newData);
  comboOptions.setFirstSelected();
  Rect_t::setCoor(left, top, {std::max((int) comboOptions.getCord().X, (int) EMPTY_VALUE.size()), 2});
  comboOptions.cord.X = cord.X;
  comboOptions.setBuffer();
}

void ComboBox_t::setDefaultColor() {
  setColor(0xf1);
  setColorOptions(0xb1, 0xf1);
}

void ComboBox_t::clear() {
  menuOpenFlag = 0;
  onlyDrawMenuFlag = 0;

  comboOptions.clearAll();
}

void ComboBox_t::setCoor(int l, int t) {
  Rect_t::setCoor(l, t, {std::max(comboOptions.getCord().X + 1, (int) EMPTY_VALUE.size()), 2});

  comboOptions.setCoor(left, top + 3);
}

void ComboBox_t::setSelectedIndex(int id) {
  comboOptions.setSelectedIndex(id);
}

int ComboBox_t::getSelectedIndex() {
  //if comboOptions is empty, it return -1
  return comboOptions.getSelectedIndex();
}

string ComboBox_t::getSelectedValue() {
  return comboOptions.getSelectedValue();
}

void ComboBox_t::setFirstSelected() {
  if (comboOptions.empty()) return;

  comboOptions.setFirstSelected();
}

void ComboBox_t::draw(Graphics& g) {
  if (! canDrawFlag) {
    canDrawFlag = true;
    return;
  }

  if (! onlyDrawMenuFlag) {
    g.setColor(attribute);
    border->draw(left, top, cord, g);
    border->draw(left + cord.X + 1, top, {1, 2}, g);

    if (comboOptions.empty()) {
      g.write(left + 1, top + 1, EMPTY_VALUE);
    } else {
      g.write(left + 1, top + 1, comboOptions.getSelectedValue());
    }

    g.write(left + cord.X + 2, top + 1, string(1, CHAR_DIRECTDOWN));
  } else {
    onlyDrawMenuFlag = false;
  }

  if (menuOpenFlag) {
    comboOptions.draw(g);
  }
}

bool ComboBox_t::mouseScroll(int state, Graphics& g) {
  if (menuOpenFlag) {
    if (comboOptions.mouseScroll(state, g)) {
      onlyDrawMenuFlag = true;

      Rect_t::setFocus(*this);
      return true;
    }
  }
  return false;
}

bool ComboBox_t::mouseClick(int l, int t, Graphics &g) {
  if (! ableFocusFlag) return false;

	if (g.isInside(l, t, left + cord.X + 1, top + 1, 2, 0)) {
    menuOpenFlag ^= 1;

    if(! menuOpenFlag) {
      comboOptions.hide(g);
    }

    Rect_t::setFocus(*this);
    return true;
	}
	if (menuOpenFlag) {
		if (comboOptions.mouseClick(l, t, g)) {
      menuOpenFlag = false;

      comboOptions.hide(g);

      Rect_t::setFocus(*this);
      return true;
		}
	}
	return false;
}

void ComboBox_t::keyPress(int keyCode, char ch, Graphics &g) {
  if (! ableFocusFlag) return;

  switch(keyCode) {
    case KEY_SPACE: case KEY_ENTER: {
      if (! menuOpenFlag) {
        menuOpenFlag = true;
      } else {
        menuOpenFlag = false;
        comboOptions.keyPress(keyCode, ch, g);
        comboOptions.hide(g);
      }
      break;
    }
    case KEY_ESCAPE: {
      if (menuOpenFlag) {
        menuOpenFlag = false;
        comboOptions.hide(g);
      }
      break;
    }
    case KEY_UP: case KEY_DOWN: {
      if (! menuOpenFlag) return;
      comboOptions.keyPress(keyCode, ch, g);
      onlyDrawMenuFlag = true;
      break;
    }
    default: {
      canDrawFlag = false;
    }
  }
}

void ComboBox_t::releaseFocus(Graphics& g) {
  if (menuOpenFlag) {
    menuOpenFlag = false;

    comboOptions.hide(g);

    draw(g);
  }
}
