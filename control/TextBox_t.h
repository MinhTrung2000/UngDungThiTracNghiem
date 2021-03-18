#ifndef TEXTBOX_T_H
#define TEXTBOX_T_H

#include <string>
#include "RadioBox_t.h"

using namespace std;

class TextBox_t : public RadioBox_t
{
private:
    struct Label2_t : public Label_t
    {

        Label2_t() : Label_t() {}
        Label2_t(const string& val) : Label_t(val) {}
        Label2_t(int l, int t, const string& val) : Label_t(l, t, val) {}
        Label2_t(int borderType, int l, int t, const string& val)
            : Label_t(borderType, l, t, val) {}
        Label2_t(int borderType, int l, int t, int _padding, const string& val)
            : Label_t(borderType, l, t, _padding, val) {}


        bool mouseClick(int l, int t, Graphics &g)
        {
            if (g.isInside(l, t, left, top, cord.X - 1, 0))
            {
                return true;
            }
            return false;
        }

        bool canGetFocus()
        {
            return true;
        }

        Label2_t& operator = (const Label2_t& rhs)
        {
            Label_t::operator=(rhs);
            return *this;
        }
    };

public:
    TextBox_t();
    TextBox_t(int l, int t);
    TextBox_t(int l, int t, const Vector<string>& str);
    TextBox_t(int l, int t, int portLineNumber);
    TextBox_t(int l, int t, int portLineNumber, const Vector<string>& str);
    TextBox_t(int borderType, int l, int t, int portLineNumber);
    TextBox_t(int borderType, int l, int t, int portLineNumber, const Vector<string>& str);

    int getSelectedIndex();

    string getSelectedValue();

    void setData(const Vector<string>& newData);

    void insertNewItem(const string& newItem);
    void insertNewItem(int id, const string& newItem);

    //set selection and focus
//	void setFocusAtIndex(int id);

    TextBox_t& operator = (const TextBox_t& o)
    {
        RadioBox_t::operator=(o);
        return *this;
    }
};

#endif
