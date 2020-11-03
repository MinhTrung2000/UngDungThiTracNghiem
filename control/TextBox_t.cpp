#include "TextBox_t.h"

TextBox_t::TextBox_t() : RadioBox_t() {}

TextBox_t::TextBox_t(int l, int t, const Vector<string>& str) {
  previousIndex = 0;
  currentIndex = 0;
  firstDrawFlag = true;
  port = NULL;

  int tmp = 0;
  int wid = 0;

  if (!str.empty()) {
    allCheckBoxes.push_back(new Label2_t(l, t, str[0]));

    tmp = t + allCheckBoxes[0]->getCord().Y - 1;
    wid = allCheckBoxes[0]->getCord().X;

    for (int i = 1; i < str.size() ; i++) {
      allCheckBoxes.push_back(new Label2_t(l, tmp + 1, str[i]));
      tmp += allCheckBoxes[i]->getCord().Y;
      wid = std::max(wid, (int) allCheckBoxes[i]->getCord().X);
    }
  }

  Rect_t::setCoor(l, t, {std::min(wid, maxWidthItem), tmp - t + 1});
  setDefaultColor();
}

TextBox_t::TextBox_t(int l, int t, int portLineNumber, const Vector<string>& str)
: TextBox_t(BorderFactory::SINGLE, l, t, portLineNumber, str) {}

TextBox_t::TextBox_t(int borderType, int l, int t, int portLineNumber, const Vector<string>& str)
: TextBox_t(l, t, str) {
  port = new ViewPort(std::min(portLineNumber, str.size()));
  setBorder(borderType);
  Rect_t::setCoor(left, top, {cord.X, portLineNumber + 1});

//  setForceChecked();
}

TextBox_t::TextBox_t(int l, int t)
: TextBox_t(l, t, Vector<string>()) {}

TextBox_t::TextBox_t(int l, int t, int portLineNumber)
: TextBox_t(l, t, portLineNumber, Vector<string>()) {}

TextBox_t::TextBox_t(int borderType, int l, int t, int portLineNumber)
: TextBox_t(borderType, l, t, portLineNumber, Vector<string>()) {}

int TextBox_t::getSelectedIndex() {
  if (empty()) return -1;
  return currentIndex;
}

string TextBox_t::getSelectedValue() {
  if (empty()) return string();
  return allCheckBoxes[currentIndex]->getValue();
}

void TextBox_t::setData(const Vector<string>& newData) {
  allCheckBoxes.clear();

  int nextTop = top;
  int wid = cord.X;

  if (! newData.empty()) {
    allCheckBoxes.push_back(new Label2_t(left, nextTop + 1, newData[0]));

    wid = std::max(wid, (int) allCheckBoxes[0]->getCord().X);

    for (int i = 1; i < newData.size() ; i++) {
      nextTop += allCheckBoxes[i - 1]->getCord().Y;
      allCheckBoxes.push_back(new Label2_t(left, nextTop + 1, newData[i]));
      wid = std::max(wid, (int) allCheckBoxes[i]->getCord().X);
    }

    nextTop += allCheckBoxes.back()->getCord().Y;
  }

  if (port != NULL) {
    port->lineNumber = std::min((int) cord.Y - 1, allCheckBoxes.size());
//    Rect_t::setCoor(left, top, {std::min(wid, maxWidthItem), port->lineNumber + 1});
    Rect_t::setCoor(left, top, {std::min(wid, maxWidthItem), cord.Y});
  } else {
    //clear old bufferscreen
    Graphics::getCurrentActiveGraphics()->clearRect(left, top, cord, unFocusEleAttr);
    Rect_t::setCoor(left, top, {std::min(wid, maxWidthItem), nextTop - top + 1});
  }

  firstDrawFlag = true;
  reset();
}

void TextBox_t::insertNewItem(const string& newItem) {
  if (port == NULL) return;

  int nextTop = top;

  if (! allCheckBoxes.empty()) {
    nextTop = allCheckBoxes.back()->getTop() + allCheckBoxes.back()->getCord().Y;
  }

  allCheckBoxes.push_back(new Label2_t(left, nextTop + 1, newItem));

  port->lineNumber = std::min((int) cord.Y - 1, allCheckBoxes.size());

  setFocusAtIndex(allCheckBoxes.size() - 1);
  setDrawAllLinePort(true);
}

void TextBox_t::insertNewItem(int id, const string& newItem) {
  assert(0 <= id && id < allCheckBoxes.size());

  if (port == NULL) return;

  int nextTop = top;

  if (! allCheckBoxes.empty() && id > 1) {
    nextTop = allCheckBoxes[id - 1]->getTop() + allCheckBoxes[id - 1]->getCord().Y;
  }

  allCheckBoxes.insert(id, new Label2_t(left, nextTop + 1, newItem));

  port->lineNumber = std::min((int) cord.Y - 1, allCheckBoxes.size());

  setFocusAtIndex(id);
  setDrawAllLinePort(true);
}


