#ifndef BORDER_T_H
#define BORDER_T_H

#include <windows.h>

#include "Graphics.h"

class Border_t {
protected:
	void drawBorder(char topLeft, char hrLine, char topRight, char vrLine, char botoomLeft, char bottomRight, int left, int top, COORD cord, const string& title, Graphics& g);

public:
	virtual void draw(int l, int t, COORD cord, const string& title, Graphics& g) = 0;
	virtual void draw(int l, int t, COORD cord, Graphics& g) = 0;

	static bool isNoBorder(Border_t *border);
	static bool isSingleaBorder(Border_t *border);
	static bool isDoubleBorder(Border_t *border);
	static bool isPlainBorder(Border_t *border);
};

class NoBorder_t : public Border_t {
	virtual void draw(int l, int t, COORD cord, const string& title, Graphics& g) {}
	virtual void draw(int l, int t, COORD cord, Graphics& g) {}
};

class SingleBorder_t : public Border_t {
	virtual void draw(int l, int t, COORD cord, const string& title, Graphics& g);
	virtual void draw(int l, int t, COORD cord, Graphics& g);
};

class DoubleBorder_t : public Border_t {
	virtual void draw(int l, int t, COORD cord, const string& title, Graphics& g);
	virtual void draw(int l, int t, COORD cord, Graphics& g);
};

class PlainBorder_t : public Border_t {
	virtual void draw(int l, int t, COORD cord, const string& title, Graphics& g);
	virtual void draw(int l, int t, COORD cord, Graphics& g);
};

class BorderFactory {
public:
	static const int NONE = 0;
	static const int SINGLE = 1;
	static const int DOUBLE = 2;
	static const int PLAIN = 3;

	static Border_t *makeBorder(const int type) {
		if (type == NONE) return new NoBorder_t;
		if (type == SINGLE) return new SingleBorder_t;
		if (type == DOUBLE) return new DoubleBorder_t;
		if (type == PLAIN) return new PlainBorder_t;
		return NULL;
	}
};

#endif
