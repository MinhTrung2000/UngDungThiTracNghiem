#ifndef MONHOC_MODEL_H
#define MONHOC_MODEL_H

#include <string>
#include <cassert>

#include "../control/MessageBox_t.h"

#include "Lop_model.h"
#include "CauHoiThi_model.h"

class MonHoc
{
private:
    static const int MAX_MAMON_LENGTH = 15;

    string maMon;	//key
    string tenMon;

    friend class DSMonHoc_t;
    friend ostream& operator << (ostream& os, const MonHoc& rhs);
    friend istream& operator >> (istream& is, MonHoc& rhs);

public:
    MonHoc() {}
    MonHoc(const string& _maMon, const string& _tenMon)
        : maMon(_maMon),
          tenMon(_tenMon)
    {
    }
    MonHoc(const MonHoc& rhs) : maMon(rhs.maMon), tenMon(rhs.tenMon) {}

    static int getMaxMaMonLength()
    {
        return MAX_MAMON_LENGTH;
    }

    void setMaMon(const string& newMaMon)
    {
        assert(newMaMon.size() <= MAX_MAMON_LENGTH);
        maMon = newMaMon;
    }

    string getMaMon()
    {
        return maMon;
    }

    void setTenMon(const string& newTenMon)
    {
        tenMon = newTenMon;
    }

    string getTenMon()
    {
        return tenMon;
    }

#define compare(_op_) bool operator _op_ (const MonHoc& o) { return maMon _op_ o.maMon; }
    compare(<) compare(<=) compare(>) compare(>=) compare(==) compare(!=)
#undef compare

    MonHoc& operator = (const MonHoc& rhs)
    {
        maMon = rhs.maMon;
        tenMon = rhs.tenMon;
        return *this;
    }
};

ostream& operator << (ostream& os, const MonHoc& rhs)
{
    os << rhs.maMon << '\n';
    os << rhs.tenMon;
    return os;
}

istream& operator >> (istream& is, MonHoc& rhs)
{
    getline(is, rhs.maMon);
    getline(is, rhs.tenMon);
    return is;
}

#endif
