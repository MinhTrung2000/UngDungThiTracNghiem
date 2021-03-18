#ifndef PASSWORDFIELD_T_H_INCLUDED
#define PASSWORDFIELD_T_H_INCLUDED

#include "TextField_t.h"

class PasswordField_t : public TextField_t
{
public:
    PasswordField_t();
    PasswordField_t(int l, int t, int maxWid);
    PasswordField_t(int borderType, int l, int t, int maxWid);

    void draw(Graphics& g);

    PasswordField_t& operator = (const PasswordField_t& o)
    {
        TextField_t::operator=(o);
        return *this;
    }
};

#endif // PASSWORDFIELD_T_H_INCLUDED
