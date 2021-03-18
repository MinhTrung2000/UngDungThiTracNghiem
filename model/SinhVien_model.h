#ifndef SINHVIEN_MODEL_H
#define SINHVIEN_MODEL_H

#include <string>
#include <direct.h>

#include "../control/lib_sll.h"

#include "DiemThi_model.h"
#include "DSDiemThi_t_model.h"

class SinhVien
{
private:
    string maSV;	//key
    string ho;
    string ten;
    string phai;
    string matKhau;
    string maLop;
    DSDiemThi_t *danhSachMonDaThi;

    friend class DSSinhVien_t;
    friend ostream& operator << (ostream& os, const SinhVien& rhs);
    friend istream& operator >> (istream& is, SinhVien& rhs);

public:
    //Khoi tao sinh vien dong thoi tao danh sach cac mon da thi la rong
    SinhVien() : danhSachMonDaThi(new DSDiemThi_t) {}
    SinhVien(const string& _maSV, const string& _ho, const string& _ten, const string& _phai, const string& _matKhau, const string& _maLop)
        : maSV(_maSV),
          ho(_ho),
          ten(_ten),
          phai(_phai),
          matKhau(_matKhau),
          maLop(_maLop),
          danhSachMonDaThi(new DSDiemThi_t)
    {
    }
    SinhVien(const SinhVien& rhs)
    {
        maSV = rhs.maSV;
        ho = rhs.ho;
        ten = rhs.ten;
        phai = rhs.phai;
        matKhau = rhs.matKhau;
        maLop = rhs.maLop;

        danhSachMonDaThi = new DSDiemThi_t;
        *danhSachMonDaThi = *rhs.danhSachMonDaThi;
    }

    ~SinhVien()
    {
        delete danhSachMonDaThi;
    }

    void setMaSV(const string& newMaSv)
    {
        maSV = newMaSv;
    }
    string getMaSV()
    {
        return maSV;
    }

    void setHo(const string& newHo)
    {
        ho = newHo;
    }
    string getHo()
    {
        return ho;
    }

    void setTen(const string& newTen)
    {
        ten = newTen;
    }
    string getTen()
    {
        return ten;
    }

    void setPhai(const string& newPhai)
    {
        phai = newPhai;
    }
    string getPhai()
    {
        return phai;
    }

    void setMatKhau(const string& newMatKhau)
    {
        matKhau = newMatKhau;
    }
    string getMatKhau()
    {
        return matKhau;
    }

    void setMaLop(const string& newMaLop)
    {
        maLop = newMaLop;
    }
    string getMaLop()
    {
        return maLop;
    }

    void setDSMonDaThi(DSDiemThi_t *ds)
    {
        danhSachMonDaThi = ds;
    }
    DSDiemThi_t *getDSMonDaThi()
    {
        return danhSachMonDaThi;
    }

    string getFileNameMonThi(const string& maMon) const
    {
        return "data/Lop/" + maLop + "/" + maSV + "/" + maMon + ".data";
    }

    //Khi tao sinh vien, tao new folder chua du lieu sv thi
    void createFolderMonDaThiData() const
    {
        string folderName = "data/Lop/" + maLop + "/" + maSV;
        _mkdir(folderName.c_str());
    }

    //Khi xoa sinh vien, dong thoi xoa folder chua du lieu sv thi
    void removeFolderMonDaThiData() const
    {
        string folderName = "data/Lop/" + maLop + "/" + maSV;
        assert(FileUtility::DeleteDirectory(folderName.c_str()) == 0);
    }

    //Cap nhat sau khi xoa 1 mon hoc
    void capNhatDuLieuXoaMonHoc(const string& maMon)
    {
        if (danhSachMonDaThi->remove(maMon))
        {
            assert(remove(getFileNameMonThi(maMon).c_str()) == 0);
        }
    }

    //Cap nhat sau khi xoa tat ca mon hoc
    void capNhatDuLieuXoaTatCaMonHoc()
    {
        SLLNode<DiemThi> *dsPtr = danhSachMonDaThi->begin();
        while (dsPtr != NULL)
        {
            assert(remove(getFileNameMonThi(dsPtr->key.getMaMon()).c_str()) == 0);
            dsPtr = dsPtr->next;
        }
        danhSachMonDaThi->xoaDS();
    }

    //Lay Diem Thi theo ma mon
    DiemThi *getDiemMonDaThi(const string& maMon)
    {
        return danhSachMonDaThi->getDiemThi(maMon);
    }

    //SV sau khi kiem tra thanh cong, them diem thi moi vao ds cac mon da thi
    void themDiemThi(DiemThi& newDiemThi)
    {
        if (danhSachMonDaThi->getDiemThi(newDiemThi.getMaMon()) == NULL)
        {
            danhSachMonDaThi->insert(newDiemThi);

            ofstream fw(getFileNameMonThi(newDiemThi.getMaMon()).c_str());
            assert(fw.good());
            fw << newDiemThi;
            fw.close();
        }
    }

    void resetDSCauHoiThi()
    {
        danhSachMonDaThi->reset();
    }

#define compare(_op_) bool operator _op_ (const SinhVien& o) { return maSV _op_ o.maSV; }
    compare(<) compare(<=) compare(>) compare(>=) compare(==) compare(!=)
#undef compare

    SinhVien& operator = (const SinhVien& o)
    {
        maSV = o.maSV;
        ho = o.ho;
        ten = o.ten;
        phai = o.phai;
        matKhau = o.matKhau;
        maLop = o.maLop;

        delete danhSachMonDaThi;
        danhSachMonDaThi = new DSDiemThi_t;
        *danhSachMonDaThi = *o.danhSachMonDaThi;

        return *this;
    }
};

ostream& operator << (ostream& os, const SinhVien& rhs)
{
    os << rhs.maSV << '\n';
    os << rhs.ho << '\n';
    os << rhs.ten << '\n';
    os << rhs.phai << '\n';
    os << rhs.matKhau << '\n';
    os << rhs.maLop << '\n';
    os << rhs.danhSachMonDaThi->getSoDiemThi();

    SLLNode<DiemThi> *dsMonThiPtr = rhs.danhSachMonDaThi->begin();

    while (dsMonThiPtr != NULL)
    {
        string maMon = dsMonThiPtr->key.getMaMon();
        os << '\n' << maMon;
        dsMonThiPtr = dsMonThiPtr->next;
    }

    return os;
}

istream& operator >> (istream& is, SinhVien& rhs)
{
    string line = "";

    if (getline(is, rhs.maSV))
    {
        getline(is, rhs.ho);
        getline(is, rhs.ten);
        getline(is, rhs.phai);
        getline(is, rhs.matKhau);
        getline(is, rhs.maLop);

        rhs.danhSachMonDaThi->xoaDS();

        getline(is, line);
        int soMonDaThi = atoi(line.c_str());

        ifstream fr;
        for (int i = 0; i < soMonDaThi; ++i)
        {
            getline(is, line);
            DiemThi newDiemThi;
            newDiemThi.setMaMon(line);
            fr.open(rhs.getFileNameMonThi(line).c_str());
            if (fr.good())
            {
                fr >> newDiemThi;
                rhs.danhSachMonDaThi->insert(newDiemThi);
            }

            fr.close();
        }
    }

    return is;
}

#endif
