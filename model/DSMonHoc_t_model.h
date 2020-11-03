#ifndef DSMONHOC_T_MODEL_H_INCLUDED
#define DSMONHOC_T_MODEL_H_INCLUDED

#include "MonHoc_model.h"
#include "DSLop_t_model.h"
#include "DSCauHoiThi_t_model.h"

using namespace std;

class DSMonHoc_t {
private:
  static DSMonHoc_t *danhSachMonHocHienHanh;

  static const int MAX_MONHOC_NUMBER = 50;

  //TYPE: Mang con tro
	MonHoc *ds[MAX_MONHOC_NUMBER];

	//so mon hoc hien tai
	int soMon;

	DSMonHoc_t() : soMon(0) {}

	friend ostream& operator << (ostream& os, const DSMonHoc_t& rhs);
	friend istream& operator >> (istream& is, DSMonHoc_t& rhs);

public:

	~DSMonHoc_t() {
		xoaDS();
	}

	bool empty() { return soMon == 0; }

	bool full() { return soMon == MAX_MONHOC_NUMBER; }

	void xoaDS() {
		while (soMon > 0) {
			delete ds[--soMon];
		}
	}

	int getSoMon() { return soMon; }

	static int getSoMonToiDa() { return MAX_MONHOC_NUMBER; }

	int search(const string& maMon) {
		for (int i = 0; i < soMon; ++i)
			if (ds[i]->maMon == maMon)
				return i;
		return -1;
	}

	MonHoc* getMonHoc(const string& maMon) {
		int id = search(maMon);
		if (id == -1) return NULL;
		return ds[id];
	}

	MonHoc* getMonHoc(int id) {
	  assert(0 <= id && id < soMon);
		return ds[id];
	}

	//Lay danh sach ma mon
	Vector<string> getDSMaMon() {
    Vector<string> res;
    for (int i = 0; i < soMon; ++i) {
      res.push_back(ds[i]->maMon);
    }
    return res;
	}

	void insert(MonHoc *newMonHoc) {
	  assert(soMon < MAX_MONHOC_NUMBER);
	  ds[soMon] = new MonHoc;
    ds[soMon] = newMonHoc;
    ++soMon;
	}

	void remove(int id) {
    assert(0 <= id && id < soMon);

    delete ds[id];
    for (int i = id; i < soMon - 1; ++i) {
      ds[i] = ds[i + 1];
    }
    --soMon;
	}

	DSMonHoc_t& operator = (const DSMonHoc_t& rhs) {
    xoaDS();
    soMon = rhs.soMon;
    for (int i = 0; i < soMon; ++i) {
      ds[i] = new MonHoc;
      *ds[i] = *rhs.ds[i];
    }
    return *this;
	}

	//========================================================================
	static DSMonHoc_t *getDSMonHocHienHanh() { return danhSachMonHocHienHanh; }
};

ostream& operator << (ostream& os, const DSMonHoc_t& rhs) {
  //ghi so mon
  os << rhs.soMon << '\n';

  for (int i = 0; i < rhs.soMon - 1; ++i) {
    os << *rhs.ds[i] << '\n';
  }

  if (rhs.soMon > 1)
    os << *rhs.ds[rhs.soMon - 1];

  return os;
}

istream& operator >> (istream& is, DSMonHoc_t& rhs) {
  string line;

  rhs.xoaDS();

  if (getline(is, line)) {
    rhs.soMon = atoi(line.c_str());

    assert(rhs.soMon <= rhs.MAX_MONHOC_NUMBER);

    if (rhs.soMon > 0) {
      for (int i = 0; i < rhs.soMon; ++i) {
        rhs.ds[i] = new MonHoc;
        is >> *rhs.ds[i];
      }
    }
  }

  return is;
}

DSMonHoc_t *DSMonHoc_t::danhSachMonHocHienHanh = new DSMonHoc_t;

#endif // DSMONHOC_T_MODEL_H_INCLUDED
