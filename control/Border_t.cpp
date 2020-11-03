#include "Border_t.h"

#include <cassert>
#include <string>
#include <iostream>

void SingleBorder_t::draw(int l, int t, COORD cord, const string& title, Graphics& g) {
  drawBorder('\xDA', '\xC4', '\xBF', '\xB3', '\xC0', '\xD9', l, t, cord, title, g);
}

void SingleBorder_t::draw(int l, int t, COORD cord, Graphics& g) {
  drawBorder('\xDA', '\xC4', '\xBF', '\xB3', '\xC0', '\xD9', l, t, cord, "", g);
}
//===============================================
void DoubleBorder_t::draw(int l, int t, COORD cord, const string& title, Graphics& g) {
  drawBorder('\xC9', '\xCD', '\xBB', '\xBA', '\xC8', '\xBC', l, t, cord, title, g);
}

void DoubleBorder_t::draw(int l, int t, COORD cord, Graphics& g) {
  drawBorder('\xC9', '\xCD', '\xBB', '\xBA', '\xC8', '\xBC', l, t, cord, "", g);
}
//===============================================
void PlainBorder_t::draw(int l, int t, COORD cord, const string& title, Graphics& g) {
  drawBorder(' ', ' ', ' ', ' ', ' ', ' ', l, t, cord, title, g);
}

void PlainBorder_t::draw(int l, int t, COORD cord, Graphics& g) {
  drawBorder(' ', ' ', ' ', ' ', ' ', ' ', l, t, cord, "", g);
}
//===============================================
void Border_t::drawBorder(char topLeft, char hrLine, char topRight, char vrLine, char bottomLeft, char bottomRight, int l, int t, COORD cord, const string& title, Graphics& g) {
  stringstream line;
  if (title == "") {
    line << topLeft << string(cord.X, hrLine) << topRight;
    g.write(l, t, line.str());
  } else {
    assert(title.size() < cord.X);

    int remaining = cord.X - (int) title.size();
    int paddingTitle = (remaining / 2 > 3 ? 3 : remaining / 2);
    line << topLeft << string(paddingTitle, hrLine) << title << string((int) cord.X - title.size() - paddingTitle, hrLine) << topRight;
    g.write(l, t, line.str());
  }

  line.str("");
  line.clear();

  line << vrLine << string(cord.X, ' ') << vrLine;
  string lineStr = line.str();
  for (int hor = 1; hor < cord.Y; ++hor) {
    g.write(l, t + hor, lineStr);
  }

  line.str("");
  line.clear();

  line << bottomLeft << string(cord.X, hrLine) << bottomRight;
  g.write(l, t + cord.Y, line.str());
}

bool Border_t::isNoBorder(Border_t *border) {
  return (border != NULL && dynamic_cast<NoBorder_t *>(border) != NULL);
}

bool Border_t::isSingleaBorder(Border_t *border) {
  return (border != NULL && dynamic_cast<SingleBorder_t *>(border) != NULL);
}

bool Border_t::isDoubleBorder(Border_t *border) {
  return (border != NULL && dynamic_cast<DoubleBorder_t *>(border) != NULL);
}

bool Border_t::isPlainBorder(Border_t *border) {
  return (border != NULL && dynamic_cast<PlainBorder_t *>(border) != NULL);
}
