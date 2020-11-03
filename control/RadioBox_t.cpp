#include "RadioBox_t.h"

RadioBox_t::RadioBox_t()
: CheckList_t(), forceCheckFlag(false) {}

RadioBox_t::RadioBox_t(int l, int t, const Vector<string>& str)
: CheckList_t(l, t, str), forceCheckFlag(false) {}

RadioBox_t::RadioBox_t(int l, int t, CheckBox_t::_checkBoxStyle_ cbStyle, const Vector<string>& str)
: CheckList_t(l, t, cbStyle, str), forceCheckFlag(false) {}

RadioBox_t::RadioBox_t(int l, int t, int portLineNumber, const Vector<string>& str)
: CheckList_t(l, t, portLineNumber, str), forceCheckFlag(false) {}

RadioBox_t::RadioBox_t(int borderType, int l, int t, int portLineNumber, const Vector<string>& str)
: CheckList_t(borderType, l, t, portLineNumber, str), forceCheckFlag(false) {}

RadioBox_t::RadioBox_t(int l, int t, int portLineNumber, CheckBox_t::_checkBoxStyle_ cbStyle, const Vector<string>& str)
: CheckList_t(l, t, portLineNumber, cbStyle, str), forceCheckFlag(false) {}

RadioBox_t::RadioBox_t(int borderType, int l, int t, int portLineNumber, CheckBox_t::_checkBoxStyle_ cbStyle, const Vector<string>& str)
: CheckList_t(borderType, l, t, portLineNumber, cbStyle, str), forceCheckFlag(false) {}

RadioBox_t::RadioBox_t(int l, int t)
: RadioBox_t(l, t, Vector<string>()) {}

RadioBox_t::RadioBox_t(int l, int t, CheckBox_t::_checkBoxStyle_ cbStyle)
: RadioBox_t(l, t, cbStyle, Vector<string>()) {}

RadioBox_t::RadioBox_t(int l, int t, int portLineNumber)
: RadioBox_t(l, t, portLineNumber, Vector<string>()) {}

RadioBox_t::RadioBox_t(int borderType, int l, int t, int portLineNumber)
: RadioBox_t(borderType, l, t, portLineNumber, Vector<string>()) {}

RadioBox_t::RadioBox_t(int l, int t, int portLineNumber, CheckBox_t::_checkBoxStyle_ cbStyle)
: RadioBox_t(l, t, portLineNumber, cbStyle, Vector<string>()) {}

int RadioBox_t::getSelectedIndex() {
  if (empty()) return -1;
  if (allCheckBoxes[previousIndex]->getIsChecked() == false) return -1;
  return previousIndex;
}

void RadioBox_t::setFirstSelected() {
  setSelectedIndex(0);
}

//set check and focus
void RadioBox_t::setSelectedIndex(int id) {
  if (allCheckBoxes.empty()) return;
  assert(0 <= id && id < allCheckBoxes.size());

  allCheckBoxes[previousIndex]->setIsChecked(false);

  if (currentIndex != id) {
    allCheckBoxes[currentIndex]->setColor(unFocusEleAttr);
  }

  previousIndex = currentIndex = id;
  allCheckBoxes[id]->setIsChecked(true);
  allCheckBoxes[id]->setColor(focusEleAttr);

  setFocusAtIndex(id);

  if (port != NULL) {
    port->drawAllLineFlag = true;
  } else {
    firstDrawFlag = true;
  }
}

string RadioBox_t::getSelectedValue() {
  if (empty()) return "";
  if (allCheckBoxes[previousIndex]->getIsChecked() == false) return "";
  return allCheckBoxes[previousIndex]->getValue();
}

void RadioBox_t::setForceChecked() {
  forceCheckFlag = true;
  reset();
  if (! allCheckBoxes.empty()) {
    allCheckBoxes[0]->setIsChecked(true);
    allCheckBoxes[0]->setColor(focusEleAttr);
  }
}

bool RadioBox_t::mouseClick(int l, int t, Graphics &g) {
  if (allCheckBoxes.empty()) return false;

  if (port != NULL) {
    return mouseClickByPort(l, t, g);
  }

  if (g.isInside(l, t, left, top, cord.X - 1, cord.Y - 1)) {
    for (int i = 0; i < allCheckBoxes.size(); ++i) {
      if (allCheckBoxes[i]->mouseClick(l, t, g)) {
        if (forceCheckFlag) {
          allCheckBoxes[i]->setIsChecked(true);
        }

        if (previousIndex != i) {
          allCheckBoxes[previousIndex]->setIsChecked(false);
          allCheckBoxes[previousIndex]->setColor(unFocusEleAttr);
          if (previousIndex != currentIndex) {
            allCheckBoxes[previousIndex]->draw(g, maxWidthItem);
          }
        }

        if (currentIndex != i) {
          allCheckBoxes[currentIndex]->setColor(unFocusEleAttr);
          allCheckBoxes[currentIndex]->draw(g, maxWidthItem);
        }

        previousIndex = currentIndex = i;
        allCheckBoxes[currentIndex]->setColor(focusEleAttr);

        Rect_t::setFocus(*this);
        return true;
      }
    }
  }
  return false;
}

void RadioBox_t::keyPress(int keyCode, char ch, Graphics &g) {
  if (allCheckBoxes.empty()) return;

  if (port != NULL) {
    keyPressByPort(keyCode, ch, g);
    return;
  }

  switch (keyCode) {
		case KEY_DOWN: {	//down arrow
		  if (currentIndex == allCheckBoxes.size() - 1) {
        canDrawFlag = false;
        break;
      }
			allCheckBoxes[currentIndex]->setColor(unFocusEleAttr);
      allCheckBoxes[currentIndex]->draw(g);

			allCheckBoxes[++currentIndex]->setColor(focusEleAttr);
			break;
		}
		case KEY_UP: {	// up arrow key
      if (currentIndex == 0) {
        canDrawFlag = false;
        break;
      }
      allCheckBoxes[currentIndex]->setColor(unFocusEleAttr);
      allCheckBoxes[currentIndex]->draw(g);

      allCheckBoxes[--currentIndex]->setColor(focusEleAttr);
			break;
		}
		case KEY_SPACE:	case KEY_ENTER: {
       if (forceCheckFlag) {
        allCheckBoxes[currentIndex]->setIsChecked(true);
      } else {
        allCheckBoxes[currentIndex]->reverseChecked();
      }

      if (previousIndex != currentIndex) {
        allCheckBoxes[previousIndex]->setIsChecked(false);
        allCheckBoxes[previousIndex]->setColor(unFocusEleAttr);
        allCheckBoxes[previousIndex]->draw(g, maxWidthItem);
      }

      previousIndex = currentIndex;
		}
	}
}

bool RadioBox_t::mouseClickByPort(int l, int t, Graphics& g) {
  if (g.isInside(l, t, left + 1, top + 1, cord.X - 1, cord.Y - 1)) {
    for (int i = port->currentTopIndex; i <= port->currentBottomIndex; ++i) {
      if (t == top + i - port->currentTopIndex + 1) {
        if (forceCheckFlag) {
          allCheckBoxes[i]->setIsChecked(true);
        } else {
          allCheckBoxes[i]->reverseChecked();
        }

        if (previousIndex != i) {
          allCheckBoxes[previousIndex]->setIsChecked(false);
          allCheckBoxes[previousIndex]->setColor(unFocusEleAttr);
          if (previousIndex != currentIndex && port->currentTopIndex <= previousIndex && previousIndex <= port->currentBottomIndex)
            drawLineByPort(previousIndex, g);
        }

        if (currentIndex != i) {
          allCheckBoxes[currentIndex]->setColor(unFocusEleAttr);
          drawLineByPort(currentIndex, g);
        }

        previousIndex = currentIndex = i;
        allCheckBoxes[currentIndex]->setColor(focusEleAttr);
        port->drawAllLineFlag = false;
        Rect_t::setFocus(*this);
        return true;
      }
    }
  }
  return false;
}

void RadioBox_t::keyPressByPort(int keyCode, char ch, Graphics& g) {
  switch (keyCode) {
    case KEY_DOWN:	//down arrow
      if (currentIndex == allCheckBoxes.size() - 1) {
        canDrawFlag = false;
        break;
      }
      allCheckBoxes[currentIndex]->setColor(unFocusEleAttr);
      drawLineByPort(currentIndex, g);

      allCheckBoxes[++currentIndex]->setColor(focusEleAttr);

      if (currentIndex > port->currentBottomIndex) {
        port->currentBottomIndex = currentIndex;
        ++port->currentTopIndex;
        port->drawAllLineFlag = true;
      } else {
        port->drawAllLineFlag = false;
      }
      break;
    case KEY_UP:	// up arrow key
      if (currentIndex == 0) {
        canDrawFlag = false;
        break;
      }
      allCheckBoxes[currentIndex]->setColor(unFocusEleAttr);
      drawLineByPort(currentIndex, g);

      allCheckBoxes[--currentIndex]->setColor(focusEleAttr);

      if (currentIndex < port->currentTopIndex) {
        port->currentTopIndex = currentIndex;
        --port->currentBottomIndex;
        port->drawAllLineFlag = true;
      } else {
        port->drawAllLineFlag = false;
      }
      break;
    case KEY_SPACE:	// Space key
    case KEY_ENTER:	// Enter key
      if (forceCheckFlag) {
        allCheckBoxes[currentIndex]->setIsChecked(true);
      } else {
        allCheckBoxes[currentIndex]->reverseChecked();
      }

      if (previousIndex != currentIndex) {
        allCheckBoxes[previousIndex]->setIsChecked(false);
        allCheckBoxes[previousIndex]->setColor(unFocusEleAttr);
        if (previousIndex != currentIndex && port->currentTopIndex <= previousIndex && previousIndex <= port->currentBottomIndex)
          drawLineByPort(previousIndex, g);
      }

      previousIndex = currentIndex;
      port->drawAllLineFlag = false;
      break;
  }
}

void RadioBox_t::setClearAll() {
  if (allCheckBoxes.empty()) return;

  if (!forceCheckFlag) {
    allCheckBoxes[previousIndex]->setIsChecked(false);
    allCheckBoxes[previousIndex]->setColor(unFocusEleAttr);

    setFocusAtIndex(0);
  }
}
