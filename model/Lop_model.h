#ifndef LOP_MODEL_H
#define LOP_MODEL_H

#include <string>
#include <cstdio>
#include <direct.h>

#include "../control/lib_vector.h"

#include "SinhVien_model.h"
#include "DSSinhVien_t_model.h"

class Lop
{
private:
    string maLop;	  //key
    string tenLop;
    DSSinhVien_t *danhSachSinhVien;

    friend class DSLop_t;
    friend ostream& operator << (ostream& os, const Lop& rhs);
    friend istream& operator >> (istream& is, Lop& rhs);

public:
    Lop() : danhSachSinhVien(new DSSinhVien_t) {}

    Lop(const string& _maLop, const string& _tenLop)
        : maLop(_maLop),
          tenLop(_tenLop),
          danhSachSinhVien(new DSSinhVien_t)
    {
    }

    Lop(const Lop& rhs)
    {
        maLop = rhs.maLop;
        tenLop = rhs.tenLop;
        delete danhSachSinhVien;
        danhSachSinhVien = new DSSinhVien_t;
        *danhSachSinhVien = *rhs.danhSachSinhVien;
    }

    ~Lop()
    {
        delete danhSachSinhVien;
    }

//	//Lay ten file luu ds sinh vien tuong ung voi ma lop
//	string getDSSVFileName() const {
//    return "data//Lop//dssv_lop" + maLop + ".data";
//	}

    void setMaLop(const string& newMaLop)
    {
        maLop = newMaLop;
    }

    string getMaLop()
    {
        return maLop;
    }

    void setTenLop(const string& newTenLop)
    {
        tenLop = newTenLop;
    }

    string getTenLop()
    {
        return tenLop;
    }

    void setDanhSachSinhVien(DSSinhVien_t *dsPtr)
    {
        danhSachSinhVien = dsPtr;
    }

    DSSinhVien_t *getDanhSachSinhVien()
    {
        return danhSachSinhVien;
    }

    void getDanhSachSinhVien(Vector<SinhVien *> &list)
    {
        return danhSachSinhVien->get(list);
    }

    void createLopFolder() const
    {
        string folderName = "data/Lop/" + maLop;
        _mkdir(folderName.c_str());
        string dssvFileName = folderName + "/DSSV.data";
        ofstream fw(dssvFileName.c_str());
        fw.close();
    }

    void removeLopFolder() const
    {
        string folderName = "data/Lop/" + maLop;
        assert(FileUtility::DeleteDirectory(folderName) == 0);
    }

    //Cap nhat sau khi xoa 1 mon hoc
    void capNhatDuLieuXoaMonHoc(const string& maMon)
    {
        danhSachSinhVien->capNhatDuLieuXoaMonHoc(maMon);
    }

    //Cap nhat sau khi xoa tat ca mon hoc
    void capNhatDuLieuXoaTatCaMonHoc()
    {
        danhSachSinhVien->capNhatDuLieuXoaTatCaMonHoc();
    }

    SinhVien *getSinhVien(const string& maSV)
    {
        return danhSachSinhVien->getSinhVien(maSV);
    }

#define compare(_op_) bool operator _op_ (const Lop& o) { return maLop _op_ o.maLop; }
    compare(<) compare(<=) compare(>) compare(>=) compare(==) compare(!=)
#undef compare

    Lop& operator = (const Lop& o)
    {
        maLop = o.maLop;
        tenLop = o.tenLop;
        danhSachSinhVien = new DSSinhVien_t;
        *danhSachSinhVien = *o.danhSachSinhVien;
        return *this;
    }
};

ostream& operator << (ostream& os, const Lop& rhs)
{
    os << rhs.maLop << '\n';
    os << rhs.tenLop;

    string dssvFileName = "data/Lop/" + rhs.maLop + "/DSSV.data";
    ofstream fw(dssvFileName.c_str());
    if (fw.good())
    {
        fw << *rhs.danhSachSinhVien;
    }
//  assert(fw.good());
//  fw << *rhs.danhSachSinhVien;

    fw.close();

    return os;
}

istream& operator >> (istream& is, Lop& rhs)
{
    getline(is, rhs.maLop);
    getline(is, rhs.tenLop);

    string dssvFileName = "data/Lop/" + rhs.maLop + "/DSSV.data";
    ifstream fr(dssvFileName.c_str());
    if (fr.good())
    {
        fr >> *rhs.danhSachSinhVien;
    }
//  assert(fr.good());
//  fr >> *rhs.danhSachSinhVien;

    fr.close();

    return is;
}

#endif
