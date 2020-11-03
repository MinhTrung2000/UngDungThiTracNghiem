#ifndef DSLOPPANEL_H_INCLUDED
#define DSLOPPANEL_H_INCLUDED

#include "UIForm.h"
#include "../control/Panel_t.h"
#include "../control/MessageBox_t.h"
#include "../control/Button_t.h"
#include "../control/ComboBox_t.h"
#include "../control/Table_t.h"
#include "../control/TextField_t.h"

#include "../model/Lop_model.h"
#include "../model/DSLop_t_model.h"

using namespace std;

class DSLopPanel : public Panel_t {
private:
  //Danh sach lop hoc
  DSLop_t *dsLop;

  //Chuc nang
  Label_t lbChucNang;
  ComboBox_t cmbChucNang;
  Button_t btnThucThi;

  //Ma lop
  Label_t lbMaLop;
  TextField_t tfMaLop;

  //Ten Lop
  Label_t lbTenLop;
  TextField_t tfTenLop;

  //Nhan thong bao so lop
  Label_t lbThongBaoSoLop;

  //Bang danh sach lop hoc
  Label_t lbDSLop;
  Table_t tbDSLop;

  //Ghi chu
  Label_t lbGhiChu;

  void capNhatTongSoLop(bool drawFlag) {
    lbThongBaoSoLop.clearValue();
    lbThongBaoSoLop.setValue("Tong so lop: " + StringUtility::to_string(dsLop->getSoLop()));
    if (drawFlag) {
      lbThongBaoSoLop.draw(*Graphics::getCurrentActiveGraphics());
    }
  }

  //Chon Them
  void themOption() {
    string maLop = tfMaLop.getValue();
    string tenLop = tfTenLop.getValue();

    StringUtility::Trim(maLop);
    StringUtility::UpperCaseAllLetter(maLop);
    StringUtility::Trim(tenLop);
    StringUtility::UpperCaseFirstLetterPerWord(tenLop);

    //kiem tra ma lop co chua ki tu khac chu va so
    if (StringUtility::HavingPunctChar(maLop)) {
      showMessageDialog("Ma lop hoc chi bao gom chu cai va so!", MessageBox_t::ERROR_MESSAGE);
      Rect_t::setFocus(tfMaLop);
      return;
    }

    //kiem tra ma lop co khoang trang
    if (! StringUtility::IsAlNum(maLop)) {
      showMessageDialog("Ma lop hoc khong cho phep co khoang trang!", MessageBox_t::ERROR_MESSAGE);
      Rect_t::setFocus(tfMaLop);
      return;
    }

    if (maLop.empty()) {
      showMessageDialog("Ma lop hoc khong duoc de trong!", MessageBox_t::ERROR_MESSAGE);
      Rect_t::setFocus(tfMaLop);
      return;
    }

    if (tenLop.empty()) {
      showMessageDialog("Ten lop hoc khong duoc de trong!", MessageBox_t::ERROR_MESSAGE);
      Rect_t::setFocus(tfTenLop);
      return;
    }

    //kiem tra ten lop co chua ki tu khac chu va so
    if (StringUtility::HavingPunctChar(tenLop)) {
      showMessageDialog("Ten lop hoc chi bao gom chu cai va so!", MessageBox_t::ERROR_MESSAGE);
      Rect_t::setFocus(tfTenLop);
      return;
    }

    if (dsLop->search(maLop) != -1) {
      //thong bao trung ma lop
      showMessageDialog("Ma lop hoc bi trung lap.\nKhong the them lop hoc nay!", MessageBox_t::ERROR_MESSAGE);
      Rect_t::setFocus(tfMaLop);
      return;
    }

    Lop newLop(maLop, tenLop);
    dsLop->insert(newLop);

    //cap nhat table
    tbDSLop.insertRow(getTableDataAtIndex(&newLop, dsLop->getSoLop() - 1));

    //cap nhat so lop
    capNhatTongSoLop(true);

//    generateInformationUIElement(*Graphics::getCurrentActiveGraphics(), true);
    generateInformationUIElement(&newLop, true);

    showMessageDialog("Them lop hoc co ma lop " + maLop + " thanh cong!", MessageBox_t::INFORMATION_MESSAGE);
  }

  //Chon nut Cap nhat
  void capNhatOption() {
    if (dsLop->empty()) {
      showMessageDialog("Danh sach lop hoc rong!", MessageBox_t::ERROR_MESSAGE);
      return;
    }

    //Lay ma lop tu tfMaLop duoc generate
    string maLopCapNhat = tfMaLop.getValue();

    string newTenLop = tfTenLop.getValue();

    StringUtility::Trim(newTenLop);
    StringUtility::UpperCaseFirstLetterPerWord(newTenLop);

    if (newTenLop.empty()) {
      showMessageDialog("Ten lop hoc khong duoc de trong!", MessageBox_t::ERROR_MESSAGE);
      Rect_t::setFocus(tfTenLop);
      return;
    }

    //kiem tra ten lop co chua ki tu khac chu va so
    if (StringUtility::HavingPunctChar(newTenLop)) {
      showMessageDialog("Ten lop hoc chi bao gom chu cai va so!", MessageBox_t::ERROR_MESSAGE);
      Rect_t::setFocus(tfTenLop);
      return;
    }

    int id = dsLop->search(maLopCapNhat);

    Lop *lopCapNhat = dsLop->getLop(id);
    lopCapNhat->setTenLop(newTenLop);

    tbDSLop.updateRowByIndex(id, getTableDataAtIndex(lopCapNhat, id));

    generateInformationUIElement(lopCapNhat, true);

    showMessageDialog("Thay doi ten lop hoc co ma " + maLopCapNhat + " thanh cong!", MessageBox_t::INFORMATION_MESSAGE);
  }

  //Chon nut Xoa
  void xoaOption() {
    //Lay ma lop duoc generate
    if (dsLop->empty()) {
      showMessageDialog("Danh sach lop hoc rong. Khong the xoa!", MessageBox_t::ERROR_MESSAGE);
      return;
    }

    string maLopXoa = tfMaLop.getValue();

    int id = dsLop->search(maLopXoa);

    string rep = showOptionDialog("Xac nhan xoa lop hoc co ma lop " + maLopXoa + "?", MessageBox_t::QUESTION_MESSAGE, MessageBox_t::OK_CANCEL_OPTION);

    if (rep == "CANCEL") return;

//    assert(remove(dsLop->getLop(id)->getDSSVFileName().c_str()) == 0);

    dsLop->remove(id);

    tbDSLop.setData(getTableData());
    tbDSLop.draw(*Graphics::getCurrentActiveGraphics());

    //no problem if tbDSLop is empty
    tbDSLop.setFirstSelected();

    Lop *lopDauTien = dsLop->getLop(tbDSLop.getSelectedValue()[1]);

    capNhatTongSoLop(true);

    generateInformationUIElement(lopDauTien, true);
  }

  //Chon xoa tat ca
  void xoaTatCaOption() {
    //Lay id lop can xoa tai table
    if (dsLop->empty()) {
      showMessageDialog("Danh sach lop hoc rong!", MessageBox_t::ERROR_MESSAGE);
      return;
    }

    string rep = showOptionDialog("Xac nhan xoa het danh sach lop hoc?", MessageBox_t::QUESTION_MESSAGE, MessageBox_t::OK_CANCEL_OPTION);

    if (rep == "CANCEL") return;

    int soLop = dsLop->getSoLop();

    dsLop->xoaDS();

    tbDSLop.clearAll();

    capNhatTongSoLop(true);

    generateInformationUIElement(NULL, true);
  }

  void generateInformationUIElement(Lop *lopPtr, bool drawFlag) {
    if (! tbDSLop.empty()) {
//      Lop *lop = dsLop->getLop(tbDSLop.getSelectedValue()[1]);
//      tfMaLop.setValue(lop->getMaLop());
//      tfTenLop.setValue(lop->getTenLop());
      tfMaLop.setValue(lopPtr->getMaLop());
      tfTenLop.setValue(lopPtr->getTenLop());
    } else {
      //xoa noi dung tfMaLop, tfTenLop
      tfMaLop.clear();
      tfTenLop.clear();
    }
    if (drawFlag) {
      tfMaLop.draw(*Graphics::getCurrentActiveGraphics());
      tfTenLop.draw(*Graphics::getCurrentActiveGraphics());
    }
  }

  //Lay du lieu bang lop hoc
  Vector<Vector<string> > getTableData() {
    Vector<Vector<string> > res;

    if (! dsLop->empty()) {
      int soLop = dsLop->getSoLop();
      res.resize(soLop);
      Lop *lop;
      for (int i = 0; i < soLop; ++i) {
        lop = dsLop->getLop(i);
        res[i].push_back(StringUtility::to_string(i + 1));
        res[i].push_back(lop->getMaLop());
        res[i].push_back(lop->getTenLop());
      }
    }

    return res;
  }

  //Lay du lieu hang i
  Vector<string> getTableDataAtIndex(Lop* lopPtr, int id) {
    assert(0 <= id && id < dsLop->getSoLop());

//    Lop *lop = dsLop->getLop(id);

//    assert(lop != NULL);
    assert(lopPtr != NULL);

    Vector<string> res;
    res.push_back(StringUtility::to_string(id + 1));
//    res.push_back(lop->getMaLop());
//    res.push_back(lop->getTenLop());
    res.push_back(lopPtr->getMaLop());
    res.push_back(lopPtr->getTenLop());
    return res;
  }

  friend class GVExecutiveForm;

public:
  DSLopPanel()
  : Panel_t(BorderFactory::PLAIN, 0, 1, {170,44}),
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

    lbMaLop = Label_t(lbChucNang.getLeft(), lbChucNang.getTop() + 5, "Ma lop:");
    lbMaLop.setColor(this->attribute);
    add(&lbMaLop);

    tfMaLop = TextField_t(BorderFactory::SINGLE, lbMaLop.getLeft() + 10, lbMaLop.getTop() - 1, 40, 100);
    add(&tfMaLop);

    lbTenLop = Label_t(lbMaLop.getLeft(), lbMaLop.getTop() + 5, "Ten lop:");
    lbTenLop.setColor(this->attribute);
    add(&lbTenLop);

    tfTenLop = TextField_t(BorderFactory::SINGLE, tfMaLop.getLeft(), lbTenLop.getTop() - 1, 40, 100);
    add(&tfTenLop);

    btnThucThi = Button_t(tfTenLop.getLeft() + tfTenLop.getCord().X - 10, tfTenLop.getTop() + 5, "Thuc Thi");
    add(&btnThucThi);

    lbGhiChu = Label_t(BorderFactory::SINGLE, lbChucNang.getLeft(), btnThucThi.getTop() + 8,
       "Ghi chu:\nMa lop va ten lop chi bao gom chu cai va so.\nMa lop khong chua khoang trang.");
    lbGhiChu.setColor(this->attribute);
    add(&lbGhiChu);

    lbDSLop = Label_t(BorderFactory::DOUBLE, 95, 4, 10, "DANH SACH LOP HOC HIEN HANH");
    lbDSLop.setColor(this->attribute);
    add(&lbDSLop);

    lbThongBaoSoLop = Label_t(BorderFactory::SINGLE, 72, 8, "So lop hoc: " + StringUtility::to_string(dsLop->getSoLop()));
    lbThongBaoSoLop.setColor(this->attribute);
    add(&lbThongBaoSoLop);

    Vector<string> DSMonTableHeaders;
    DSMonTableHeaders.push_back("STT");
    DSMonTableHeaders.push_back("Ma Lop");
    DSMonTableHeaders.push_back("Ten Lop");

    tbDSLop = Table_t(72, lbThongBaoSoLop.getTop() + 5, 25, DSMonTableHeaders);

    Vector<size_t> szTable;
    szTable.push_back(10);
    szTable.push_back(30);
    szTable.push_back(50);

    tbDSLop.setFixedWidths(szTable);

    //find is set on ma lop key
    tbDSLop.setFindOptionColumnIndex(1);

    add(&tbDSLop);
  }

  void reloadData() {
    cmbChucNang.setSelectedIndex(0);
    tfMaLop.clear();
    tfTenLop.clear();
    tfMaLop.setAbleFocus(true);
    tfTenLop.setAbleFocus(true);

    //reload so lop
    capNhatTongSoLop(false);

    //reload bang ds lop hoc
    tbDSLop.setData(getTableData());
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
          tfMaLop.clear();
          tfTenLop.clear();
          tfMaLop.setAbleFocus(true);
          tfTenLop.setAbleFocus(true);
        } else if (option == "Cap Nhat") {
          generateInformationUIElement(dsLop->getLop(tbDSLop.getSelectedValue()[1]), true);
          tfMaLop.setAbleFocus(false);
          tfTenLop.setAbleFocus(true);
        } else {
          generateInformationUIElement(dsLop->getLop(tbDSLop.getSelectedValue()[1]), true);
          tfMaLop.setAbleFocus(false);
          tfTenLop.setAbleFocus(false);
        }

        tfMaLop.draw(g);
        tfTenLop.draw(g);
      }
      return true;
    }

    if (tbDSLop.isFocus()) {
      generateInformationUIElement(dsLop->getLop(tbDSLop.getSelectedValue()[1]), true);

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
    if (tfMaLop.isFocus() || tfTenLop.isFocus()) {
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
          tfMaLop.clear();
          tfTenLop.clear();
          tfMaLop.setAbleFocus(true);
          tfTenLop.setAbleFocus(true);
        } else if (option == "Cap Nhat") {
          generateInformationUIElement(dsLop->getLop(tbDSLop.getSelectedValue()[1]), true);
          tfMaLop.setAbleFocus(false);
          tfTenLop.setAbleFocus(true);
        } else {
          generateInformationUIElement(dsLop->getLop(tbDSLop.getSelectedValue()[1]), true);
          tfMaLop.setAbleFocus(false);
          tfTenLop.setAbleFocus(false);
        }

        tfMaLop.draw(g);
        tfTenLop.draw(g);
      }
    }
  }
};


#endif // DSLOPPANEL_H_INCLUDED
