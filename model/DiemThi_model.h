#ifndef DIEMTHI_MODEL_H
#define DIEMTHI_MODEL_H

#include <string>
#include <cassert>

#include "MonHoc_model.h"
#include "CauHoiThi_model.h"
#include "DSCauHoiThi_t_model.h"

class DiemThi
{
private:
    string maMonHoc;
    Vector<CauHoiThi *> dsCauHoi;
    Vector<char> dsCauTraLoi;
    int diem;

    friend class DSDiemThi_t;
    friend ostream& operator << (ostream& os, const DiemThi& rhs);
    friend istream& operator >> (istream& is, DiemThi& rhs);

public:
    DiemThi() : maMonHoc(""), diem(0) {}

    DiemThi(const string& maMon, const Vector<CauHoiThi *>& _dsCauHoi, const Vector<char>& _dsCauTraLoi, int _diem)
    {
        maMonHoc = maMon;
        dsCauHoi = _dsCauHoi;
        dsCauTraLoi = _dsCauTraLoi;
        diem = _diem;
    }

    DiemThi(const DiemThi& rhs)
    {
        maMonHoc = rhs.maMonHoc;
        dsCauHoi = rhs.dsCauHoi;
        dsCauTraLoi = rhs.dsCauTraLoi;
        diem = rhs.diem;
    }

    void setMaMon(const string& newMaMon)
    {
        maMonHoc = newMaMon;
    }

    string getMaMon()
    {
        return maMonHoc;
    }

    void setDiem(int newDiem)
    {
        diem = newDiem;
    }

    int getDiem()
    {
        return diem;
    }

    void setDSCauHoiThi(const Vector<CauHoiThi *>& newDS)
    {
        dsCauHoi = newDS;
    }

    Vector<CauHoiThi *> getDSCauHoiThi()
    {
        return dsCauHoi;
    }

    void setDSCautTraLoi(const Vector<char>& newDS)
    {
        dsCauTraLoi = newDS;
    }

    Vector<char> getDSCauTraLoi()
    {
        return dsCauTraLoi;
    }

    void reset()
    {
        for (int i = 0; i < dsCauHoi.size(); ++i)
        {
            dsCauHoi[i]->setCanModifyFlag(true);
        }
    }

    void tinhDiem()
    {
        diem = 0;
        int soCauDung = 0;
        for (int i = 0; i < dsCauHoi.size(); ++i)
        {
            if (dsCauHoi[i]->getDapAn() == dsCauTraLoi[i])
            {
                ++soCauDung;
            }
        }
        diem = (int) (10.0 / (double) dsCauHoi.size() * soCauDung);
    }

#define compare(_op_) bool operator _op_ (const DiemThi& o) const { return maMonHoc _op_ o.maMonHoc; }
    compare(<) compare(<=) compare(>) compare(>=) compare(==) compare(!=)
#undef compare

    DiemThi& operator = (const DiemThi& o)
    {
        maMonHoc = o.maMonHoc;
        dsCauHoi = o.dsCauHoi;
        dsCauTraLoi = o.dsCauTraLoi;
        diem = o.diem;
        return *this;
    }
};

ostream& operator << (ostream& os, const DiemThi& rhs)
{
//  os << rhs.maMonHoc << '\n';

    int soCau = rhs.dsCauHoi.size();

    os << soCau << '\n';

    for (int i = 0; i < soCau; ++i)
        os << rhs.dsCauHoi[i]->getId() << '\n';
//    os << *rhs.dsCauHoi[i] << '\n';

    //Day cau tra loi (ie.: ABCAC...)
    for (int i = 0; i < soCau; ++i)
    {
        os << rhs.dsCauTraLoi[i];
    }

    os << '\n' << rhs.diem;

    return os;
}

istream& operator >> (istream& is, DiemThi& rhs)
{
    rhs.dsCauHoi.clear();
    rhs.dsCauTraLoi.clear();

    string line = "";

    if (getline(is, line))
    {
        int soCau = atoi(line.c_str());

        if (soCau > 0)
        {
            DSCauHoiThi_t *NganHangDeThi = DSCauHoiThi_t::getDSCauHoiThiHienHanh();

            rhs.dsCauHoi.resize(soCau);

            for (int i = 0; i < soCau; ++i)
            {
                getline(is, line);
                int cauHoiId = atoi(line.c_str());
                rhs.dsCauHoi[i] = NganHangDeThi->getCauHoi(cauHoiId);
                assert(rhs.dsCauHoi[i] != NULL);
            }

            rhs.dsCauTraLoi.resize(soCau);

            getline(is, line);

            assert(!line.empty());
            assert(line.size() == soCau);

            for (int i = 0; i < soCau; ++i)
            {
                rhs.dsCauTraLoi[i] = line[i];
            }
        }

        getline(is, line);
        rhs.diem = atoi(line.c_str());
    }

    return is;
}

#endif
