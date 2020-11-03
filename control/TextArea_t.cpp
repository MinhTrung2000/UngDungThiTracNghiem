#include "TextArea_t.h"

TextArea_t::TextArea_t() : Rect_t() {
  setCurrentPosition(0, 0);
}

TextArea_t::TextArea_t(int l, int t, int maxWid, int maxLine)
  : Rect_t(BorderFactory::SINGLE, l, t, {maxWid + 2, maxLine + 1}, "") {
  textLines.resize(maxLine);
  setCurrentPosition(l + 1, t + 1);
  setDefaultColor();
}

//process at currentPosition
void TextArea_t::deleteCharStandardize(int lineId) {
  if (lineId == textLines.size() - 1 || textLines[lineId + 1].empty()) return;

  int remaining = cord.X - textLines[lineId].size();
  remaining = std::min(remaining, (int) textLines[lineId + 1].size());
  if (remaining == 0) return;

  int firstSpaceId = remaining - 1;
  while (firstSpaceId >= 0 && textLines[lineId + 1][firstSpaceId] != ' ')
    --firstSpaceId;

  if (firstSpaceId == -1) {
    if (remaining != textLines[lineId + 1].size()) return;
    firstSpaceId = remaining - 1;
  }

  textLines[lineId] += textLines[lineId + 1].substr(0, firstSpaceId + 1);
  textLines[lineId + 1].erase(0, firstSpaceId + 1);

  deleteCharStandardize(lineId + 1);
}

bool TextArea_t::insertCharStandardize(int lineId) {
  COORD backUpPosition = currentPosition;
  ++currentPosition.X;
  for (int i = lineId; i < textLines.size(); ++i) {
    if (i == textLines.size() - 1 && textLines[i].size() > cord.X) {
      currentPosition = backUpPosition;
      return false;
    }
    if (textLines[i].size() <= cord.X) return true;

    bool noSpaceFound = false;
    int firstSpaceRev = cord.X;

    if (textLines[i][firstSpaceRev] != ' ') {
      while (firstSpaceRev >= 0 && textLines[i][firstSpaceRev] != ' ')
        --firstSpaceRev;
      ++firstSpaceRev;
      if (firstSpaceRev == 0) {
        noSpaceFound = true;
        firstSpaceRev = cord.X;
      }
    }

    if (i != textLines.size() - 1) {
      string word = textLines[i].substr(firstSpaceRev, string::npos);
      textLines[i].erase(firstSpaceRev, string::npos);
      textLines[i + 1] = word + textLines[i + 1];
    }

    if (firstSpaceRev < currentPosition.X - left - 1) {
      currentPosition.X = currentPosition.X - firstSpaceRev;
      currentPosition.Y = top + 1 + lineId + 1;
    }
  }
  return true;
}

void TextArea_t::draw(Graphics& g) {
  g.setColor(attribute);
  border->draw(left, top, cord, titleRect, g);
  for (int i = 0; i < textLines.size(); ++i) {
    g.write(left + 1, top + 1 + i, textLines[i]);
  }
  g.setCursorVisibility(true);
  g.moveTo(currentPosition.X, currentPosition.Y);

//  Graphics::getCurrentActiveGraphics()->setColor(0x4e);
//  Graphics::getCurrentActiveGraphics()->write(currentPosition.X, currentPosition.Y, "#");
}

void TextArea_t::setDefaultColor() {
  setColor(0xe5);
}

void TextArea_t::addCheckFunction(bool (*newFunc)(const string&)) {
  checkInputValid.push_back(newFunc);
}

void TextArea_t::clear() {
  for (int i = 0; i < textLines.size(); ++i) {
    textLines[i] = "";
  }
  setCurrentPosition(left + 1, top + 1);
}
//
//void TextArea_t::setValue(const string& value) {
//  //clear old data and reset initial cursor
//  clear();
//
//  for (int i = 0; i < value.size(); ++i) {
//    int currY = currentPosition.Y - top - 1;
//    //is full
//    if (currY == textLines.size() - 1 && textLines[currY].size() == cord.X) return;
//
//    //back up
//    Vector<string> backUpText;
//    for (int i = currY; i < textLines.size(); ++i)
//      backUpText.push_back(textLines[i]);
//
//    //insert one char
//    textLines[currY].insert(currentPosition.X - left - 1, 1, value[i]);
//
//    if (insertCharStandardize(currY) == false) {
//      for (int i = currY; i < textLines.size(); ++i) {
//        textLines[i] = backUpText[i - currY];
//      }
//    }
//  }
//}
void TextArea_t::setValue(const string& value) {
  //clear old data and reset initial cursor
  clear();

  for (int i = 0; i < value.size(); ++i) {
    int currY = currentPosition.Y - top - 1;
    //is full
    if (currY == textLines.size() - 1 && textLines[currY].size() == cord.X) return;

    //back up
    Vector<string> backUpText;
    for (int i = currY; i < textLines.size(); ++i)
      backUpText.push_back(textLines[i]);

    //insert one char
    textLines[currY].insert(currentPosition.X - left - 1, 1, value[i]);

    if (insertCharStandardize(currY) == false) {
      for (int i = currY; i < textLines.size(); ++i) {
        textLines[i] = backUpText[i - currY];
      }
      for (int i = 0; i < 3; ++i) {
        textLines.back()[cord.X - 1 - i] = '.';
      }
      break;
    }
  }
}

string TextArea_t::getValue() {
  string value;
  for (int i = 0; i < textLines.size(); ++i)
    value += textLines[i];
  return value;
}

bool TextArea_t::mouseClick(int l, int t, Graphics &g) {
  if (g.isInside(l, t, left + 1, top + 1, cord.X - 1, cord.Y - 2)) {
    if (! ableFocusFlag) return false;

    for (int i = 0; i < textLines.size(); ++i) {
      if (textLines[i].empty()) {
        if (i == 0) {
          currentPosition = {left + 1, top + 1};
        } else {
          currentPosition = {left + 1 + textLines[i - 1].size(), top + i};
        }
        g.moveTo(currentPosition.X, currentPosition.Y);
        Rect_t::setFocus(*this);
        return true;
      }
      if (t == top + i + 1) {
        currentPosition = {std::min(l, left + 1 + (int) textLines[i].size()), t};
        g.moveTo(currentPosition.X, currentPosition.Y);
        Rect_t::setFocus(*this);
        return true;
      }
    }
  }
  return false;
}

void TextArea_t::keyPress(int keycode, char ch, Graphics& g) {
  if (! ableFocusFlag) return;

  switch (keycode) {
    case KEY_BACKSPACE:	{	// backspace
      if (currentPosition.X == left + 1) {
        if (currentPosition.Y == top + 1) return;

        --currentPosition.Y;
        currentPosition.X = left + 2 + textLines[currentPosition.Y - top - 1].size();
        keyPress(keycode, ch, g);
      }
      int currY = currentPosition.Y - top - 1;
      textLines[currY].erase(currentPosition.X - 1 - left - 1, 1);
      --currentPosition.X;
      deleteCharStandardize(currY);
      break;
    }
    case KEY_DELETE: {	// del key
      int currY = currentPosition.Y - top - 1;
      if (currentPosition.X == left + 1 + textLines[currY].size()) return;
      textLines[currY].erase(currentPosition.X - left - 1, 1);
      deleteCharStandardize(currY);
      break;
    }
    case KEY_LEFT:  {  // left arrow
      if (currentPosition.X > left + 1)   // checking if we are not moving outside the string
        --currentPosition.X;
      break;
    }
    case KEY_RIGHT:	{ // right arrow
      if (currentPosition.X < left + 1 + textLines[currentPosition.Y - top - 1].size())
        ++currentPosition.X;
      break;
    }
    case KEY_UP: {
      if (currentPosition.Y == top + 1) return;
      --currentPosition.Y;
      currentPosition.X = std::min((int) currentPosition.X, left + 1 + (int) textLines[currentPosition.Y - top - 1].size());
      break;
    }
    case KEY_DOWN: {
      if (currentPosition.Y - top - 1 == textLines.size() - 1) return;
      if (textLines[currentPosition.Y - top].empty()) return;
      ++currentPosition.Y;
      currentPosition.X = std::min((int) currentPosition.X, left + 1 + (int) textLines[currentPosition.Y - top - 1].size());
      break;
    }
    case KEY_END: {
      currentPosition.X = left + 1 + (int) textLines[currentPosition.Y - top - 1].size();
      break;
    }
    case KEY_HOME: {
      currentPosition.X = left + 1;
      break;
    }
    case KEY_ENTER: {
      int currY = currentPosition.Y - top - 1;
      if (!textLines[textLines.size() -1].empty() || (currY == textLines.size() - 1)) return;

      for (int i = textLines.size() - 1; i > currY + 1; --i) {
        textLines[i] = textLines[i - 1];
      }

      textLines[currY + 1] = textLines[currY].substr(currentPosition.X - left - 1, string::npos);
      textLines[currY].erase(currentPosition.X - left - 1, string::npos);

      currentPosition.X = left + 1 + textLines[currY + 1].size();
      ++currentPosition.Y;
      break;
    }
    default: {
      if (iscntrl(ch)) return;
      int currY = currentPosition.Y - top - 1;
      //is full
      if (currY == textLines.size() - 1 && textLines[currY].size() == cord.X) return;

      //back up
      Vector<string> backUpText;
      for (int i = currY; i < textLines.size(); ++i)
        backUpText.push_back(textLines[i]);

      //insert one char
      textLines[currY].insert(currentPosition.X - left - 1, 1, ch);

      if (insertCharStandardize(currY) == false) {
        for (int i = currY; i < textLines.size(); ++i) {
          textLines[i] = backUpText[i - currY];
        }
      }
    }
  }
}

COORD TextArea_t::getCurrentPosition() { return currentPosition; }

bool TextArea_t::IsCursorVisible() { return true; }












