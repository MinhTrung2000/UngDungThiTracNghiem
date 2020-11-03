#ifndef TEXTAREA_T_H
#define TEXTAREA_T_H

#include <string>
#include <cctype>
#include <windows.h>
#include "Rect_t.h"

using namespace std;

class TextArea_t : public Rect_t {
private:
  Vector<string> textLines;

  Vector<bool (*)(const string&)> checkInputValid;

  //typing at line id, if this line is overflow,
  //insert the last word of it to the begining of
  //next line.
  //this method can change state of currentPosition, input line
  bool insertCharStandardize(int lineId);

  void deleteCharStandardize(int lineId);

public:
	TextArea_t();
	TextArea_t(int l, int t, int maxWid, int maxLine);

	void clear();

	void setValue(const string& value);

	string getValue();

	Vector<string> getValueLines() { return textLines; }

	void addCheckFunction(bool (*)(const string&));

	void setDefaultColor();

	virtual void draw(Graphics& g);

	virtual bool mouseClick(int l, int t, Graphics &g);

	virtual void keyPress(int keyCode, char ch, Graphics& g);

	COORD getCurrentPosition();

	bool IsCursorVisible();

	TextArea_t& operator = (const TextArea_t& o) {
    Rect_t::operator=(o);

    textLines = o.textLines;

    checkInputValid = o.checkInputValid;

    return *this;

	}
};

inline string getTextToken(const string& value, int nline, int wid) {
  string res = "";

  TextArea_t taTmp(0, 0, wid, nline);
  taTmp.setValue(value);

  Vector<string> vlines = taTmp.getValueLines();
  for (int i = 0; i< vlines.size(); ++i) {
    res += vlines[i] + "\n";
  }

  return res;
}

#endif // TEXTAREA_T_H
