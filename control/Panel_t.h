#ifndef PANEL_T_H
#define PANEL_T_H

#include <string>

#include "Rect_t.h"
#include "RadioBox_t.h"
#include "CheckList_t.h"

#include "lib_vector.h"

using namespace std;

/*
Close panel: reset flag + reset current focus

Hide Panel: reset flag
*/

class Panel_t : public Rect_t
{
protected:
    Vector<Rect_t *> items;

    Rect_t *currentFocusItem;

public:
    Panel_t();
    Panel_t(int l, int t, COORD cord);
    Panel_t(int borderType, int l, int t, COORD cord);
    Panel_t(int borderType, int l, int t, COORD cord, const string& title);

    virtual void setFirstDrawFlag(bool flag);

    virtual void setBuffer();
    virtual void getBuffer(Graphics& g);

    virtual void setReRunFlag(bool flag) {}
    virtual bool getReRunFlag()
    {
        return false;
    }

    virtual void setCurrentFocus(Rect_t *rect);
    Rect_t *getCurrentFocus();

    virtual void setDefaultColor();

    virtual void setCoor(int l, int t, COORD cor);
    virtual void setCoor(int l, int t);
    virtual void setCoor();

    virtual void draw(Graphics& g);

    virtual void add(Rect_t* rect);

    //ignore key press
    virtual bool mouseClick(int l, int t, Graphics &g);

    void getAllRects(Vector<Rect_t *> &rects);

    Panel_t& operator = (const Panel_t& o)
    {
        Rect_t::operator=(o);

        items = o.items;

        currentFocusItem = o.currentFocusItem;

        return *this;
    }
};

#endif
