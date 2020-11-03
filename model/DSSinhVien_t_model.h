#ifndef DSSINHVIEN_T_MODEL_H_INCLUDED
#define DSSINHVIEN_T_MODEL_H_INCLUDED

#include "SinhVien_model.h"

using namespace std;

class DSSinhVien_t {
private:
  //Type: DANH SACH LIEN KET DON
  SLL<SinhVien> ds;

  friend class Lop;
  friend ostream& operator << (ostream& os, const DSSinhVien_t& rhs);
	friend istream& operator >> (istream& is, DSSinhVien_t& rhs);

public:
  DSSinhVien_t() {}

  bool empty() { return ds.empty(); }

  //Lay so luong sinh vien
  int getSoSinhVien() { return ds.size(); }

  void xoaDS() {
    SLLNode<SinhVien> *dsPtr = ds.head;
    while (dsPtr != NULL) {
      dsPtr->key.removeFolderMonDaThiData();
      dsPtr->key.resetDSCauHoiThi();

      dsPtr = dsPtr->next;
    }

    ds.clear();
  }

  SLLNode<SinhVien> *begin() {
    if (empty()) return NULL;
    return ds.head;
  }

  SinhVien *getSinhVien(const string& maSV) {
    SLLNode<SinhVien> *p = ds.head;
    while (p != NULL) {
      if (p->key.maSV == maSV) return &(p->key);
      p = p->next;
    }
    return NULL;
  }

  //Lay tat ca sinh vien trong danh sach
  void get(Vector<SinhVien *>& list) {
    SLLNode<SinhVien> *p = ds.head;
    while (p != NULL) {
      list.push_back(&(p->key));
      p = p->next;
    }
  }

	//Cap nhat sau khi xoa 1 mon hoc
	void capNhatDuLieuXoaMonHoc(const string& maMon) {
	  SLLNode<SinhVien> *p = ds.head;
	  while (p != NULL) {
      p->key.capNhatDuLieuXoaMonHoc(maMon);
      p = p->next;
	  }
	}

	//Cap nhat sau khi xoa tat ca mon hoc
	void capNhatDuLieuXoaTatCaMonHoc() {
    SLLNode<SinhVien> *p = ds.head;
	  while (p != NULL) {
      p->key.capNhatDuLieuXoaTatCaMonHoc();
      p = p->next;
	  }
	}

	//Lay danh sach diem thi theo mon
	void layDSDiemThiTheoMon(const string& maMon, Vector<DiemThi *>& dsDiem) {
    SLLNode<SinhVien> *p = ds.head;
	  while (p != NULL) {
      dsDiem.push_back(p->key.getDiemMonDaThi(maMon));
      p = p->next;
	  }
	}

  void push_back(const SinhVien& newSinhVien) {
    ds.push_back(newSinhVien);
    newSinhVien.createFolderMonDaThiData();
  }

  void remove(const string& maSV) {
    SinhVien sv;
    sv.setMaSV(maSV);
    SLLNode<SinhVien> *dsNode = ds.search(sv);
    assert(dsNode != NULL);
    dsNode->key.removeFolderMonDaThiData();
    dsNode->key.resetDSCauHoiThi();

    ds.remove(dsNode);
//    assert(ds.erase(sv) == true);
  }

  DSSinhVien_t& operator = (const DSSinhVien_t& rhs) {
    ds = rhs.ds;
    return *this;
  }
};

ostream& operator << (ostream& os, const DSSinhVien_t& rhs) {
  os << rhs.ds.size();

  SLLNode<SinhVien> *p = rhs.ds.head;

  if (p != NULL) {
    while (p != NULL) {
      os << '\n' << p->key;
      p = p->next;
    }
  }

  return os;
}

istream& operator >> (istream& is, DSSinhVien_t& rhs) {
  rhs.xoaDS();

  string line = "";

  getline(is, line);
  int soLuongSV = 0;
  soLuongSV = atoi(line.c_str());
  for (int i = 0; i < soLuongSV; ++i) {
    SinhVien sv;
    is >> sv;
    rhs.ds.push_back(sv);
  }

  return is;
}

#endif // DSSINHVIEN_T_MODEL_H_INCLUDED
