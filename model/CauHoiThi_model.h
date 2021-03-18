#ifndef CAUHOITHI_MODEL_H
#define CAUHOITHI_MODEL_H

#include <string>

#include "MonHoc_model.h"
//#include "../control/lib_avltree.h"
#include "../control/lib_vector.h"
#include "../control/lib_utility.h"

#include "../control/MessageBox_t.h"

class CauHoiThi
{
private:
    static int IdTrack;

    //key
    int id;

    string maMon;
    string cauHoi;
    //4 chon lua
    Vector<string> luaChon;
    char dapAn;
    //cau hoi chua duoc thi (flag off) co the xoa, nguoc lai thi khong (flag on)
    bool canModifyFlag;

    void setId(int _id)
    {
        id = _id;
    }

    friend class DSCauHoiThi_t;
    friend ostream& operator << (ostream& os, const CauHoiThi& rhs);
    friend istream& operator >> (istream& is, CauHoiThi& rhs);

public:
    CauHoiThi()
        : id(IdTrack++),
          canModifyFlag(true)
    {
    }

    CauHoiThi(const string& _maMon, const string& _cauHoi, const Vector<string> &_luaChon, char _dapAn)
        : id(IdTrack++),
          maMon(_maMon),
          cauHoi(_cauHoi),
          luaChon(_luaChon),
          dapAn(_dapAn),
          canModifyFlag(true)
    {
    }

    CauHoiThi(const CauHoiThi& rhs)
    {
        id = rhs.id;
        maMon = rhs.maMon;
        cauHoi = rhs.cauHoi;
        luaChon = rhs.luaChon;
        dapAn = rhs.dapAn;
        canModifyFlag = rhs.canModifyFlag;
    }

    int getId()
    {
        return id;
    }

    void setMaMon(const string& newMaMon)
    {
        maMon = newMaMon;
    }

    string getMaMon() const
    {
        return maMon;
    }

    void setCauHoi(const string& newCauHoi)
    {
        cauHoi = newCauHoi;
    }

    string getCauHoi() const
    {
        return cauHoi;
    }

    void setLuaChon(const Vector<string> &newLuaChon)
    {
        luaChon = newLuaChon;
    }

    Vector<string> getLuaChon() const
    {
        return luaChon;
    }

    void setDapAn(char newDapAn)
    {
        dapAn = newDapAn;
    }

    char getDapAn() const
    {
        return dapAn;
    }

    void setCanModifyFlag(bool flag)
    {
        canModifyFlag = flag;
    }

    bool getCanModifyFlag() const
    {
        return canModifyFlag;
    }

#define compare(_op_) bool operator _op_ (const CauHoiThi& o) const { return id _op_ o.id; }
    compare(<) compare(<=) compare(>) compare(>=) compare(==) compare(!=)
#undef compare

    CauHoiThi& operator = (const CauHoiThi& o)
    {
        id = o.id;
        maMon = o.maMon;
        cauHoi = o.cauHoi;
        luaChon = o.luaChon;
        dapAn = o.dapAn;
        canModifyFlag = o.canModifyFlag;
        return *this;
    }
};

int CauHoiThi::IdTrack = 1;

ostream& operator << (ostream& os, const CauHoiThi& rhs)
{
    os << rhs.id << '\n';
    os << rhs.maMon << '\n';
    os << rhs.cauHoi << '\n';
    for (int i = 0; i < 4; ++i)
        os << rhs.luaChon[i] << '\n';
    os << rhs.dapAn << '\n';
    os << rhs.canModifyFlag;

    return os;
}

istream& operator >> (istream& is, CauHoiThi& rhs)
{
    string line = "";

    if (getline(is, line))
    {
        rhs.id = atoi(line.c_str());

        getline(is, rhs.maMon);
        getline(is, rhs.cauHoi);

        rhs.luaChon.resize(4);

        for (int i = 0; i < 4; ++i)
            getline(is, rhs.luaChon[i]);

        getline(is, line);
        rhs.dapAn = line[0];

        getline(is, line);
        rhs.canModifyFlag = line[0] - '0';
    }
    return is;
}

#endif
