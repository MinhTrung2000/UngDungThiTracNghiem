#include "PasswordField_t.h"

PasswordField_t::PasswordField_t() : TextField_t() {}

PasswordField_t::PasswordField_t(int l, int t, int maxWid) : TextField_t(l, t, maxWid) {}

PasswordField_t::PasswordField_t(int borderType, int l, int t, int maxWid) : TextField_t(borderType, l, t, maxWid) {}

void PasswordField_t::draw(Graphics& g) {
  g.setColor(attribute);

	string output = string(text.substr(firstPosition, std::min((size_t) cord.X - 2, text.size())).size(), '*');

  if (Border_t::isNoBorder(border)) {
    g.write(left, top, string(1, '|') + string(cord.X - 2, ' ') + string(1, '|'));
    g.write(left + 1, top, output);
  } else {
    border->draw(left, top, {cord.X - 2, cord.Y}, titleRect, g);
    g.write(left + 1, top + 1, output);
  }

	g.setCursorVisibility(true);
	g.moveTo(currentPosition.X, currentPosition.Y);
}
