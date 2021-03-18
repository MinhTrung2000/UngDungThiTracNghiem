#ifndef DSDIEMTHI_T_MODEL_H_INCLUDED
#define DSDIEMTHI_T_MODEL_H_INCLUDED

#include "DiemThi_model.h"

using namespace std;

class DSDiemThi_t
{
private:
    //Type: DANH SACH LIEN KET DON
    SLL<DiemThi> ds;

    friend class SinhVien;
    friend class DSSinhVien_t;
    friend class Lop;
    friend class DSLop_t;

    friend ostream& operator << (ostream& os, const DSDiemThi_t& rhs);
    friend istream& operator >> (istream& is, DSDiemThi_t& rhs);

public:
    DSDiemThi_t() {}

    bool empty() const
    {
        return ds.empty();
    }

    void xoaDS()
    {
        ds.clear();
    }

    int getSoDiemThi()
    {
        return ds.size();
    }

    void reset()
    {
        SLLNode<DiemThi> *p = ds.head;
        while (p != NULL)
        {
            p->key.reset();
            p = p->next;
        }
    }

    SLLNode<DiemThi> *begin()
    {
        if (empty())
            return NULL;
        return ds.head;
    }

    void get(Vector<DiemThi *>& list)
    {
        SLLNode<DiemThi> *p = ds.head;
        while (p != NULL)
        {
            list.push_back(&(p->key));
            p = p->next;
        }
    }

    void insert(const DiemThi& newDiemThi)
    {
        ds.push_back(newDiemThi);
    }

    //Xoa diem thi co ma mon
    bool remove(const string& maMon)
    {
        DiemThi dt;
        dt.setMaMon(maMon);
        return ds.erase(dt);
    }

    DiemThi *getDiemThi(const string& maMon)
    {
        SLLNode<DiemThi> *p = ds.head;
        while (p != NULL)
        {
            if (p->key.maMonHoc == maMon)
                return &(p->key);
            p = p->next;
        }
        return NULL;
    }

    //Lay danh sach mon hoc trong diem thi
    Vector<string> getDSMaMon()
    {
        Vector<string> res;
        SLLNode<DiemThi> *p = ds.head;
        while (p != NULL)
        {
            res.push_back(p->key.getMaMon());
            p = p->next;
        }
        return res;
    }

    DSDiemThi_t& operator = (const DSDiemThi_t& rhs)
    {
        ds = rhs.ds;
        return *this;
    }
};

#endif // DSDIEMTHI_T_MODEL_H_INCLUDED
