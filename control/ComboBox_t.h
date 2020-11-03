#ifndef COMBOBOX_T_H
#define COMBOBOX_T_H

#include <windows.h>
#include <string>
#include <cassert>

#include "Label_t.h"
#include "Rect_t.h"
#include "RadioBox_t.h"
#include "lib_vector.h"

class ComboBox_t : public Rect_t {
private:
  static const string EMPTY_VALUE;

	bool menuOpenFlag;
  bool onlyDrawMenuFlag;

  RadioBox_t comboOptions;

  friend class Table_t;

public:
	ComboBox_t();
	ComboBox_t(int l, int t);
	ComboBox_t(int l, int t, const Vector<string>& options);
	ComboBox_t(int l, int t, int lineNumber);
	ComboBox_t(int l, int t, int lineNumber, const Vector<string>& options);

	void clear();

	bool empty() { return comboOptions.empty(); }

	bool getOpenFlag() { return menuOpenFlag; }

  void setColorOptions(uint8_t focusAttrr, uint8_t unFocusAttr);
  void setDefaultColor();

  void setData(const Vector<string>& newData);

  void setSelectedIndex(int id);

  void setFirstSelected();

  int getSelectedIndex();

	string getSelectedValue();

	void setCoor(int l, int t);

	void draw(Graphics& g);

	bool mouseClick(int left, int top, Graphics& g);

	bool mouseScroll(int state, Graphics& g);

	void keyPress(int keyCode, char ch, Graphics& g);

	void releaseFocus(Graphics& g);

  ComboBox_t& operator = (const ComboBox_t& o) {
    Rect_t::operator=(o);

    menuOpenFlag = o.menuOpenFlag;
    onlyDrawMenuFlag = o.onlyDrawMenuFlag;

    comboOptions = o.comboOptions;

    return *this;
	}
};

#endif
