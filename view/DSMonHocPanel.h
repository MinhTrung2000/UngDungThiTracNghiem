#ifndef DSMONHOCPANEL_H_INCLUDED
#define DSMONHOCPANEL_H_INCLUDED

#include "UIForm.h"
#include "../control/Panel_t.h"
#include "../control/MessageBox_t.h"
#include "../control/Button_t.h"
#include "../control/ComboBox_t.h"
#include "../control/Table_t.h"
#include "../control/TextField_t.h"

#include "../model/MonHoc_model.h"
#include "../model/DSMonHoc_t_model.h"

using namespace std;

class MonHocPanel : public Panel_t {
private:
  //Danh sach mon hoc
  DSMonHoc_t *dsMonHoc;
  DSLop_t *dsLop;

  //Chuc nang
  Label_t lbChucNang;
  ComboBox_t cmbChucNang;
  Button_t btnThucThi;

  //Ma mon
  Label_t lbMaMon;
  TextField_t tfMaMon;

  //ten mon
  Label_t lbTenMon;
  TextField_t tfTenMon;

  //Nhan thong bao so mon
  Label_t lbThongBaoSoMon;

  //Bang danh sach mon hoc
  Label_t lbDSMon;
  Table_t tbDSMon;

  //Ghi chu
  Label_t lbGhiChu;

  void capNhatTongSoMon(bool drawFlag) {
    lbThongBaoSoMon.clearValue();
    lbThongBaoSoMon.setValue("Tong so mon: " + StringUtility::to_string(dsMonHoc->getSoMon()));
    if (drawFlag) {
      lbThongBaoSoMon.draw(*Graphics::getCurrentActiveGraphics());
    }
  }

  //Chon Them
  void themOption() {
    string maMon = tfMaMon.getValue();
    string tenMon = tfTenMon.getValue();

    StringUtility::Trim(maMon);
    StringUtility::UpperCaseAllLetter(maMon);
    StringUtility::Trim(tenMon);
    StringUtility::UpperCaseFirstLetterPerWord(tenMon);

    if (maMon.empty()) {
      showMessageDialog("Ma mon hoc khong duoc de trong!", MessageBox_t::ERROR_MESSAGE);
      Rect_t::setFocus(tfMaMon);
      return;
    }

    //kiem tra ma mon co chua ki tu khac chu va so
    if (StringUtility::HavingPunctChar(maMon)) {
      showMessageDialog("Ma mon hoc chi bao gom chu cai va so!", MessageBox_t::ERROR_MESSAGE);
      Rect_t::setFocus(tfMaMon);
      return;
    }

    //kiem tra ma mon co khoang trang
    if (! StringUtility::IsAlNum(maMon)) {
      showMessageDialog("Ma mon hoc khong cho phep co khoang trang!", MessageBox_t::ERROR_MESSAGE);
      Rect_t::setFocus(tfMaMon);
      return;
    }

    if (tenMon.empty()) {
      showMessageDialog("Ten mon hoc khong duoc de trong!", MessageBox_t::ERROR_MESSAGE);
      Rect_t::setFocus(tfTenMon);
      return;
    }

    //kiem tra ten mon co chua ki tu khac chu va so
    if (StringUtility::HavingPunctChar(tenMon)) {
      showMessageDialog("Ten mon hoc chi bao gom chu cai va so!", MessageBox_t::ERROR_MESSAGE);
      Rect_t::setFocus(tfTenMon);
      return;
    }

    if (dsMonHoc->full()) {
      //thong bao danh sach day
      showMessageDialog("So mon hoc toi da la " + StringUtility::to_string(dsMonHoc->getSoMonToiDa()) + "mon hoc.\nKhong the them mon hoc nay!", MessageBox_t::ERROR_MESSAGE);
      return;
    }

    if (dsMonHoc->search(maMon) != -1) {
      //thong bao trung ma Mon
      showMessageDialog("Ma mon hoc bi trung lap.\nKhong the them mon hoc nay!", MessageBox_t::ERROR_MESSAGE);
      Rect_t::setFocus(tfMaMon);
      return;
    }

    MonHoc *newMonHoc = new MonHoc(maMon, tenMon);
    dsMonHoc->insert(newMonHoc);

    //cap nhat table
    tbDSMon.insertRow(getTableDataAtIndex(dsMonHoc->getSoMon() - 1, newMonHoc));

    //cap nhat so mon
    capNhatTongSoMon(true);

    generateInformationUIElement(*Graphics::getCurrentActiveGraphics(), true);

    showMessageDialog("Them mon hoc co ma mon " + maMon + " thanh cong!", MessageBox_t::INFORMATION_MESSAGE);
  }

  //Chon nut Cap nhat
  void capNhatOption() {
    if (dsMonHoc->empty()) {
      showMessageDialog("Danh sach mon hoc rong!", MessageBox_t::ERROR_MESSAGE);
      return;
    }

    //Lay ma mon tu tfMaMon duoc generate
    string maMonCapNhat = tfMaMon.getValue();
    string newTenMon = tfTenMon.getValue();

    StringUtility::Trim(newTenMon);
    StringUtility::UpperCaseFirstLetterPerWord(newTenMon);

    if (newTenMon.empty()) {
      showMessageDialog("Ten mon hoc khong duoc de trong!", MessageBox_t::ERROR_MESSAGE);
      Rect_t::setFocus(tfTenMon);
      return;
    }

    //kiem tra ten mon co chua ki tu khac chu va so
    if (StringUtility::HavingPunctChar(newTenMon)) {
      showMessageDialog("Ten mon hoc chi bao gom chu cai va so!", MessageBox_t::ERROR_MESSAGE);
      Rect_t::setFocus(tfTenMon);
      return;
    }

    int id = dsMonHoc->search(maMonCapNhat);

    MonHoc *mh = dsMonHoc->getMonHoc(id);
    mh->setTenMon(newTenMon);

    tbDSMon.updateRowByIndex(id, getTableDataAtIndex(id, mh));

    generateInformationUIElement(*Graphics::getCurrentActiveGraphics(), true);

    showMessageDialog("Thay doi ten mon hoc co ma mon " + maMonCapNhat + " thanh cong!", MessageBox_t::INFORMATION_MESSAGE);
  }

  //Chon nut Xoa
  void xoaOption() {
    //Lay ma mon duoc generate
    if (dsMonHoc->empty()) {
      showMessageDialog("Danh sach mon hoc rong. Khong the xoa!", MessageBox_t::ERROR_MESSAGE);
      return;
    }

    string maMonXoa = tfMaMon.getValue();

    string rep = showOptionDialog("Xac nhan xoa mon hoc co ma mon la " + maMonXoa + "?", MessageBox_t::QUESTION_MESSAGE, MessageBox_t::OK_CANCEL_OPTION);

    if (rep == "CANCEL") return;

    int id = dsMonHoc->search(maMonXoa);
    dsMonHoc->remove(id);

    //Dong bo du lieu cho DS Mon da thi -> Sinh Vien -> Lop
    DSLop_t::getDSLopHienHanh()->capNhatDuLieuXoaMonHoc(maMonXoa);

    //Dong bo du lieu cho DS Cau Hoi Thi
    DSCauHoiThi_t::getDSCauHoiThiHienHanh()->xoaCauHoiThiTheoMaMon(maMonXoa);

    //cap nhat lai chi so mon hoc
    tbDSMon.setData(getTableData());
    tbDSMon.draw(*Graphics::getCurrentActiveGraphics());

    //no problem if tbDSMon is empty
    tbDSMon.setFirstSelected();

    capNhatTongSoMon(true);

    generateInformationUIElement(*Graphics::getCurrentActiveGraphics(), true);

//    Vector<CauHoiThi *> v;
//    DSCauHoiThi_t::getDSCauHoiThiHienHanh()->layDanhSachCauHoiTheoMon(maMonXoa, false, v);
//    _deb(v.size());
  }

  //Chon xoa tat ca
  void xoaTatCaOption() {
    //Lay id mon can xoa tai table
    if (dsMonHoc->empty()) {
      showMessageDialog("Danh sach mon hoc rong!", MessageBox_t::ERROR_MESSAGE);
      return;
    }

    string rep = showOptionDialog("Xac nhan xoa het danh sach mon hoc?", MessageBox_t::QUESTION_MESSAGE, MessageBox_t::OK_CANCEL_OPTION);

    if (rep == "CANCEL") return;

    dsMonHoc->xoaDS();

    //Dong bo du lieu cho DS Mon da thi -> Sinh Vien -> Lop
    DSLop_t::getDSLopHienHanh()->capNhatDuLieuXoaTatCaMonHoc();

    //Dong bo du lieu cho DS Cau Hoi Thi
    DSCauHoiThi_t::getDSCauHoiThiHienHanh()->xoaDS();

    tbDSMon.clearAll();

    capNhatTongSoMon(true);

    generateInformationUIElement(*Graphics::getCurrentActiveGraphics(), true);
  }

  //hien thi thong tin mon hoc khi chon tren bang
  void generateInformationUIElement(Graphics& g, bool drawFlag) {
    if (! tbDSMon.empty()) {
      MonHoc *mh = dsMonHoc->getMonHoc(tbDSMon.getSelectedValue()[1]);
      tfMaMon.setValue(mh->getMaMon());
      tfTenMon.setValue(mh->getTenMon());
    } else {
      tfMaMon.clear();
      tfTenMon.clear();
    }

    if (drawFlag) {
      tfMaMon.draw(g);
      tfTenMon.draw(g);
    }
  }

  //Lay du lieu bang mon hoc
  Vector<Vector<string> > getTableData() {
    Vector<Vector<string> > res;

    if (! dsMonHoc->empty()) {
      int soMon = dsMonHoc->getSoMon();
      res.resize(soMon);
      MonHoc *mh;
      for (int i = 0; i < soMon; ++i) {
        mh = dsMonHoc->getMonHoc(i);
        res[i].push_back(StringUtility::to_string(i + 1));
        res[i].push_back(mh->getMaMon());
        res[i].push_back(mh->getTenMon());
      }
    }
    return res;
  }

  //Lay du lieu hang i
  Vector<string> getTableDataAtIndex(int id, MonHoc *mh) {
    assert(0 <= id && id < dsMonHoc->getSoMon());

//    MonHoc *mh = dsMonHoc->getMonHoc(id);

    assert(mh != NULL);

    Vector<string> res;
    res.push_back(StringUtility::to_string(id + 1));
    res.push_back(mh->getMaMon());
    res.push_back(mh->getTenMon());
    return res;
  }

  friend class GVExecutiveForm;

public:
  MonHocPanel()
  : Panel_t(BorderFactory::PLAIN, 0, 1, {170,44}),
    dsMonHoc(DSMonHoc_t::getDSMonHocHienHanh()),
    dsLop(DSLop_t::getDSLopHienHanh()) {

    setColor(0xf0);

    //Set element
    lbChucNang = Label_t(5, 8, "Chon chuc nang:");
    lbChucNang.setColor(this->attribute);
    add(&lbChucNang);

    Vector<string> cmbChucNangOptions;
    cmbChucNangOptions.push_back("Them");
    cmbChucNangOptions.push_back("Cap Nhat");
    cmbChucNangOptions.push_back("Xoa");
    cmbChucNangOptions.push_back("Xoa Danh Sach");

    cmbChucNang = ComboBox_t(lbChucNang.getLeft() + lbChucNang.getCord().X + 2, 7, cmbChucNangOptions);
    add(&cmbChucNang);

    lbMaMon = Label_t(lbChucNang.getLeft(), lbChucNang.getTop() + 5, "Ma mon:");
    lbMaMon.setColor(this->attribute);
    add(&lbMaMon);

    tfMaMon = TextField_t(BorderFactory::SINGLE, lbMaMon.getLeft() + 10, lbMaMon.getTop() - 1, MonHoc::getMaxMaMonLength());
    add(&tfMaMon);

    lbTenMon = Label_t(lbMaMon.getLeft(), lbMaMon.getTop() + 5, "Ten mon:");
    lbTenMon.setColor(this->attribute);
    add(&lbTenMon);

    tfTenMon = TextField_t(BorderFactory::SINGLE, tfMaMon.getLeft(), lbTenMon.getTop() - 1, 40, 100);
    add(&tfTenMon);

    btnThucThi = Button_t(tfTenMon.getLeft() + tfTenMon.getCord().X - 10, tfTenMon.getTop() + 5, "Thuc Thi");
    add(&btnThucThi);

    lbGhiChu = Label_t(BorderFactory::SINGLE, lbChucNang.getLeft(), btnThucThi.getTop() + 8,
       "Ghi chu:\nMa mon hoc cho phep toi da " + StringUtility::to_string(MonHoc::getMaxMaMonLength()) + " ki tu.\n" +
       "Ma mon hoc chi bao gom chu cai va so.");
    lbGhiChu.setColor(this->attribute);
    add(&lbGhiChu);

    lbDSMon = Label_t(BorderFactory::DOUBLE, 95, 4, 10, "DANH SACH MON HOC HIEN HANH");
    lbDSMon.setColor(this->attribute);
    add(&lbDSMon);

    lbThongBaoSoMon = Label_t(BorderFactory::SINGLE, 72, 8, "So mon hoc: " + StringUtility::to_string(dsMonHoc->getSoMon()));
    lbThongBaoSoMon.setColor(this->attribute);
    add(&lbThongBaoSoMon);

    Vector<string> DSMonTableHeaders;
    DSMonTableHeaders.push_back("STT");
    DSMonTableHeaders.push_back("Ma Mon");
    DSMonTableHeaders.push_back("Ten Mon");

    tbDSMon = Table_t(72, lbThongBaoSoMon.getTop() + 5, 25, DSMonTableHeaders);

    Vector<size_t> szTable;
    szTable.push_back(10);
    szTable.push_back(30);
    szTable.push_back(50);

    tbDSMon.setFixedWidths(szTable);

    //find is set on ma mon key
    tbDSMon.setFindOptionColumnIndex(1);

    add(&tbDSMon);
  }

  void reloadData() {
    cmbChucNang.setSelectedIndex(0);
    tfMaMon.clear();
    tfTenMon.clear();
    tfMaMon.setAbleFocus(true);
    tfTenMon.setAbleFocus(true);

    //reload so mon
    capNhatTongSoMon(false);

    //reload bang ds mon hoc
    tbDSMon.setData(getTableData());
  }

  //Khi draw, phai load lai du lieu hien thi
  void draw(Graphics& g) {
    //reload data
    reloadData();

    Panel_t::draw(g);

    g.setColor(this->attribute);
    for (int i = 2; i <= getCord().Y + 1; ++i) {
      g.write(64, i, "\xBA");
    }
  }

  bool mouseClick(int l, int t, Graphics& g) {
    int preCmbChucNangSelectedIndex = cmbChucNang.getSelectedIndex();

    if (! Panel_t::mouseClick(l, t, g)) return false;

    if (cmbChucNang.isFocus()) {
      int newCmbChucNangSelectedIndex = cmbChucNang.getSelectedIndex();
      if (preCmbChucNangSelectedIndex != newCmbChucNangSelectedIndex) {
        string option = cmbChucNang.getSelectedValue();
        if (option == "Them") {
          tfMaMon.clear();
          tfTenMon.clear();
          tfMaMon.setAbleFocus(true);
          tfTenMon.setAbleFocus(true);
        } else if (option == "Cap Nhat") {
          generateInformationUIElement(g, true);
          tfMaMon.setAbleFocus(false);
          tfTenMon.setAbleFocus(true);
        } else {
          generateInformationUIElement(g, true);
          tfMaMon.setAbleFocus(false);
          tfTenMon.setAbleFocus(false);
        }
        tfMaMon.draw(g);
        tfTenMon.draw(g);
      }
      return true;
    }

    if (tbDSMon.isFocus()) {
      generateInformationUIElement(g, true);
      return true;
    }

    if (btnThucThi.isFocus()) {
      string option = cmbChucNang.getSelectedValue();
      if (option == "Them") {
        themOption();
      } else if (option == "Cap Nhat") {
        capNhatOption();
      } else if (option == "Xoa") {
        xoaOption();
      } else if (option == "Xoa Danh Sach") {
        xoaTatCaOption();
      }
    }

    return true;
  }

  void keyPress(int keyCode, char ch, Graphics& g) {
    if (tfMaMon.isFocus() || tfTenMon.isFocus()) {
      if (keyCode == KEY_ENTER) {
        string option = cmbChucNang.getSelectedValue();
        if (option == "Them") {
          themOption();
        } else if (option == "Cap Nhat") {
          capNhatOption();
        } else if (option == "Xoa") {
          xoaOption();
        } else if (option == "Xoa Danh Sach") {
          xoaTatCaOption();
        }
      }
    }
    if (cmbChucNang.isFocus()) {
      if (cmbChucNang.getOpenFlag() == false) {
        string option = cmbChucNang.getSelectedValue();
        if (option == "Them") {
          tfMaMon.clear();
          tfTenMon.clear();
          tfMaMon.setAbleFocus(true);
          tfTenMon.setAbleFocus(true);
        } else if (option == "Cap Nhat") {
          generateInformationUIElement(g, true);
          tfMaMon.setAbleFocus(false);
          tfTenMon.setAbleFocus(true);
        } else {
          generateInformationUIElement(g, true);
          tfMaMon.setAbleFocus(false);
          tfTenMon.setAbleFocus(false);
        }
        tfMaMon.draw(g);
        tfTenMon.draw(g);
      }
    }
  }
};

#endif // DSMONHOCPANEL_H_INCLUDED
