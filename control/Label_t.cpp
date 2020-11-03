#include "Label_t.h"

Label_t::Label_t() : Rect_t(), padding(0), maxWidth(150) {
  ableFocusFlag = false;
}

Label_t::Label_t(const string& val)
: Label_t(BorderFactory::NONE, 0, 0, 0, val) {}

Label_t::Label_t(int l, int t, const string& val)
: Label_t(BorderFactory::NONE, l, t, 0, val) {}

Label_t::Label_t(int borderType, int l, int t, const string& val)
: Label_t(borderType, l, t, 0, val) {}

Label_t::Label_t(int borderType, int l, int t, int _padding, const string& val)
: Rect_t(borderType, l, t),
  padding(_padding),
  maxWidth(150) {

  setValue(val);
  ableFocusFlag = false;
}

void Label_t::setMaxWidth(int wid) {
  maxWidth = wid;
  cord.X = std::min((int) cord.X, maxWidth);
}

bool Label_t::emptyValue() {
  bool emptyFlag = true;
  for (int i = 0; i < valueLines.size(); ++i) {
    emptyFlag &= valueLines.empty();
    if (!emptyFlag) break;
  }
  return emptyFlag;
}

void Label_t::clearValue() {
  valueLines.clear();
  Graphics::getCurrentActiveGraphics()->clearRect(left, top, cord, this->attribute);
}

void Label_t::hide(Graphics& g) {
	if (Border_t::isNoBorder(border)) {
    g.clearRect(left, top, cord, this->attribute);
  } else {
    g.clearRect(left, top, {cord.X + 2, cord.Y + 2}, this->attribute);
  }
}

//tokenize by '\n'
void Label_t::setValue(const string& newVal) {
  if (newVal == "") return;

  valueLines.clear();

  stringstream strm(newVal);
  string ele = "";

  while (getline(strm, ele, '\n')) {
    valueLines.push_back (ele);
  }

  setCoor(left, top);
}

void Label_t::setCoor(int l, int t) {
  left = l;
  top = t;

  cord.X = 0;

  for (int i = 0; i < valueLines.size(); ++i) {
    cord.X = std::max(cord.X, (short) valueLines[i].size());
  }

  cord.X  += 2 * padding;
  cord.X = std::min((int) cord.X, maxWidth);

  cord.Y = valueLines.size() + 1;
}

void Label_t::setValueLines(const Vector<string>lines) {
  valueLines = lines;
}

Vector<string> Label_t::getValueLines() {
  return valueLines;
}

/*
  If want to retrieve token string, use getValueLines()
*/
string Label_t::getValue() {
  string value = "";
  for (int i = 0; i < valueLines.size(); ++i)
    value += valueLines[i];
  return value;
}

void Label_t::draw(Graphics& g) {
	g.setColor(attribute);
	if (Border_t::isNoBorder(border)) {
    for (int i = 0; i  < valueLines.size(); ++i) {
      string text = valueLines[i];
      StringUtility::adjustStringToFit(text, maxWidth);
      g.write(left, top + i, string(padding, ' ') + text + string(padding, ' '));
    }
  } else {
    if (emptyValue()) return;
    border->draw(left, top, cord, g);
    for (int i = 0; i  < valueLines.size(); ++i) {
      string text = valueLines[i];
      StringUtility::adjustStringToFit(text, maxWidth);
      g.write(left + 1, top + 1 + i, string(padding, ' ') + text + string(padding, ' '));
    }
  }
}

void Label_t::draw(Graphics& g, int wid) {
	g.setColor(attribute);
	if (Border_t::isNoBorder(border)) {
    for (int i = 0; i  < valueLines.size(); ++i) {
      string text = valueLines[i];
      StringUtility::adjustStringToFit(text, wid);
      g.write(left, top + i, string(padding, ' ') + text + string(padding, ' '));
    }
  } else {
    if (emptyValue()) return;
    border->draw(left, top, cord, g);
    for (int i = 0; i  < valueLines.size(); ++i) {
      string text = valueLines[i];
      StringUtility::adjustStringToFit(text, wid);
      g.write(left + 1, top + 1 + i, string(padding, ' ') + text + string(padding, ' '));
    }
  }
}
