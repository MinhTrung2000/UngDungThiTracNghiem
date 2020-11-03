#ifndef DSLOP_T_MODEL_H_INCLUDED
#define DSLOP_T_MODEL_H_INCLUDED

#include "Lop_model.h"
#include "SinhVien_model.h"

using namespace std;

//DANH SACH TUYEN TINH
class DSLop_t {
private:
  static DSLop_t *danhSachLopHienHanh;

  //Type: DANH SACH TUYEN TINH
  Vector<Lop> ds;

  DSLop_t() {}

  friend ostream& operator << (ostream& os, const DSLop_t& rhs);
  friend istream& operator >> (istream& is, DSLop_t& rhs);

public:
  bool empty() { return ds.empty(); }

  int getSoLop() { return ds.size(); }

  void xoaDS() {
    for (int i = 0; i < ds.size(); ++i) {
      ds[i].removeLopFolder();
    }

    ds.clear();
  }

	Lop *getLop(int id) {
	  assert(0 <= id && id < ds.size());
    return &ds[id];
	}

  Lop *getLop(const string& maLop) {
    int id = search(maLop);
    if (id == -1) return NULL;
    return &ds[id];
  }

  int search(const string& maLop) {
    for (int i = 0; i < ds.size(); ++i) {
      if (ds[i].getMaLop() == maLop) return i;
    }
    return -1;
  }

  void insert(const Lop& newLop) {
    ds.push_back(newLop);
    newLop.createLopFolder();
  }

  void remove(int id) {
    assert(0 <= id && id < ds.size());
    ds[id].removeLopFolder();
    ds.remove(id);
  }

	//Lay danh sach ma lop
	Vector<string> getDSMaLop() {
    Vector<string> res;
    for (int i = 0; i < ds.size(); ++i) {
      res.push_back(ds[i].maLop);
    }
    return res;
	}

	//Cap nhat sau khi xoa 1 mon hoc
	void capNhatDuLieuXoaMonHoc(const string& maMon) {
    for (int i = 0; i < ds.size(); ++i) {
      ds[i].capNhatDuLieuXoaMonHoc(maMon);
    }
	}

	//Cap nhat sau khi xoa toan bo danh sach mon hoc
	void capNhatDuLieuXoaTatCaMonHoc() {
    for (int i = 0; i < ds.size(); ++i) {
      ds[i].capNhatDuLieuXoaTatCaMonHoc();
    }
	}

	//Lay tat ca sinh vien
	Vector<SinhVien *> getTatCaSinhVien() {
    Vector<SinhVien *> res;
    for (int i = 0; i < ds.size(); ++i) {
      ds[i].getDanhSachSinhVien(res);
    }
    return res;
	}

	//Tim sinh vien theo masv o het cac danh sach lop
	SinhVien *getSinhVien(const string& maSV) {
	  SinhVien *res = NULL;
    for (int i = 0; i < ds.size(); ++i) {
      res = ds[i].getDanhSachSinhVien()->getSinhVien(maSV);
      if (res != NULL) return res;
    }
    return NULL;
	}

  DSLop_t& operator = (const DSLop_t& rhs) {
    ds = rhs.ds;
    return *this;
  }

  //=======================================================
  static DSLop_t *getDSLopHienHanh() { return danhSachLopHienHanh; }
};

DSLop_t *DSLop_t::danhSachLopHienHanh = new DSLop_t;

ostream& operator << (ostream& os, const DSLop_t& rhs) {
  //ghi so lop
  int n = rhs.ds.size();

  os << n << '\n';

  for (int i = 0; i < n - 1; ++i) {
    os << rhs.ds[i] << '\n';
  }

  if (! rhs.ds.empty())
    os << rhs.ds[n - 1];

  return os;
}

istream& operator >> (istream& is, DSLop_t& rhs) {
  //clear old data
  rhs.ds.clear();

  string line = "";

  if (getline(is, line)) {
    int soLop = atoi(line.c_str());

    rhs.ds.resize(soLop);

    for (int i = 0; i < soLop; ++i) {
      is >> rhs.ds[i];
    }
  }

  return is;
}

#endif // DSLOP_T_MODEL_H_INCLUDED
