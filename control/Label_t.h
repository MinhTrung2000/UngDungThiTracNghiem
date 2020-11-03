#ifndef LABEL_T_H
#define LABEL_T_H

#include <string>

#include "Rect_t.h"

using namespace std;

class Label_t : public Rect_t {
protected:
  Vector<string> valueLines;
  int padding;
  int maxWidth;

public:
	Label_t();
	Label_t(const string& val);
	Label_t(int l, int t, const string& val);
	Label_t(int borderType, int l, int t, const string& val);
	Label_t(int borderType, int l, int t, int _padding, const string& val);

	bool emptyValue();

	virtual void setMaxWidth(int wid);

	//===============================
	//deprecated
  void setTitle(const string& newTitle) {}
  void setCloseFlag(bool flag) {}
  void setHideFlag(bool flag) {}
  void setCurrentPosition(int l, int t) {}
	virtual void setAbleFocus(bool flag) {}
	void close(Graphics& g) {}
	//================================

	virtual void setCoor(int l, int t);

	//tokenize by '\n'
	virtual void setValue(const string& newVal);
  string getValue();

  virtual void setValueLines(const Vector<string> lines);
  Vector<string> getValueLines();

  virtual void clearValue();

  virtual void hide(Graphics& g);

  //================================
  //CheckBox_t
  virtual void setIsChecked(bool check) {}
	virtual bool getIsChecked() { return false; }
  virtual void reverseChecked() {}
  //================================

	virtual void draw(Graphics& g);
	virtual void draw(Graphics& g, int wid);

	Label_t& operator = (const Label_t& o) {
    Rect_t::operator=(o);
    valueLines = o.valueLines;
    padding = o.padding;
    return *this;
	}
};

#endif
