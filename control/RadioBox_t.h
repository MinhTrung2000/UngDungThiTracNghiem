#ifndef RADIOBOX_T_H
#define RADIOBOX_T_H

#include "CheckList_t.h"
#include "lib_vector.h"

using namespace std;

class RadioBox_t : public CheckList_t
{
protected:
    //force to select at least one choice
    bool forceCheckFlag;

    virtual bool mouseClickByPort(int l, int t, Graphics& g);
    virtual void keyPressByPort(int keyCode, char ch, Graphics& g);

    friend class ComboBox_t;
    friend class Menu_t;

public:
    RadioBox_t();
    RadioBox_t(int l, int t);
    RadioBox_t(int l, int t, const Vector<string>& str);
    RadioBox_t(int l, int t, CheckBox_t::_checkBoxStyle_ cbStyle);
    RadioBox_t(int l, int t, CheckBox_t::_checkBoxStyle_ cbStyle, const Vector<string>& str);
    RadioBox_t(int l, int t, int portLineNumber);
    RadioBox_t(int l, int t, int portLineNumber, const Vector<string>& str);
    RadioBox_t(int borderType, int l, int t, int portLineNumber);
    RadioBox_t(int borderType, int l, int t, int portLineNumber, const Vector<string>& str);
    RadioBox_t(int l, int t, int portLineNumber, CheckBox_t::_checkBoxStyle_ cbStyle);
    RadioBox_t(int l, int t, int portLineNumber, CheckBox_t::_checkBoxStyle_ cbStyle, const Vector<string>& str);
    RadioBox_t(int borderType, int l, int t, int portLineNumber, CheckBox_t::_checkBoxStyle_ cbStyle, const Vector<string>& str);

    virtual int getSelectedIndex();

    virtual string getSelectedValue();

    //force to have at least one check
    void setForceChecked();

    void setFirstSelected();

    void setSelectedIndex(int id);

    //deprecated
    void setToggleAll() {}

    //clear selected item if forceCheckFlag is off
    void setClearAll();

    virtual bool mouseClick(int l, int t, Graphics &g);

    virtual void keyPress(int keyCode, char charecter, Graphics &g);

    RadioBox_t& operator = (const RadioBox_t& o)
    {
        CheckList_t::operator=(o);

        forceCheckFlag = o.forceCheckFlag;

        return *this;
    }
};

#endif
