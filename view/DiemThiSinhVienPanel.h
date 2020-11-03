#ifndef DIEMTHISINHVIENPANEL_H_INCLUDED
#define DIEMTHISINHVIENPANEL_H_INCLUDED

#include "../control/Panel_t.h"
#include "../control/MessageBox_t.h"
#include "../control/Button_t.h"
#include "../control/ComboBox_t.h"
#include "../control/CheckList_t.h"
#include "../control/RadioBox_t.h"
#include "../control/TextBox_t.h"
#include "../control/TextField_t.h"
#include "../control/TextArea_t.h"

#include "../model/DSMonHoc_t_model.h"
#include "../model/DSLop_t_model.h"
#include "../model/DSSinhVien_t_model.h"
#include "../model/DSDiemThi_t_model.h"

using  namespace std;

class DiemThiSVPanel : public Panel_t {
private:
  struct ChonSVPanel : public UIForm {
    DiemThiSVPanel *fpanel;

    Label_t lbDSSV;
    Table_t tbDSSV;

    Button_t btnChon;
    Button_t btnCancel;

    string maLopChon;
    string maSVChon;

    friend class DiemThiSVPanel;

    void reloadData() {
      tbDSSV.setData(getTableData());
    }

    Vector<Vector<string> > getTableData() {
      Vector<Vector<string> > res;

      if (! fpanel->DSSVEmpty()) {
        Vector<string> row(5);
        for (int lopId = 0; lopId < fpanel->dssv.size(); ++lopId) {
          for (int svId = 0; svId < fpanel->dssv[lopId].size(); ++svId) {
            SinhVien *sv = fpanel->dssv[lopId][svId];

            assert(sv != NULL);

            row[0] = sv->getMaSV();
            row[1] = sv->getHo();
            row[2] = sv->getTen();
            row[3] = sv->getPhai();
            row[4] = fpanel->dsLop->getLop(lopId)->getMaLop();

            res.push_back(row);
          }
        }
      }

      return res;
    }

    ChonSVPanel(DiemThiSVPanel *_fpanel)
    : UIForm(BorderFactory::DOUBLE, 42, 8, {90, 30}, "Bang chon sinh vien"),
      fpanel(_fpanel) {

      setDefaultColor();

      lbDSSV = Label_t(BorderFactory::SINGLE, left + 5, top + 3, "Danh sach sinh vien");
      lbDSSV.setColor(this->attribute);
      add(&lbDSSV);

      Vector<string> DSSVTableHeaders;
      DSSVTableHeaders.push_back("MSSV");
      DSSVTableHeaders.push_back("Ho");
      DSSVTableHeaders.push_back("Ten");
      DSSVTableHeaders.push_back("Gioi Tinh");
      DSSVTableHeaders.push_back("Lop");

      tbDSSV = Table_t(lbDSSV.getLeft(), lbDSSV.getTop() + 4, 0, 15, DSSVTableHeaders);

      Vector<size_t> szTable;
      szTable.push_back(15);
      szTable.push_back(25);
      szTable.push_back(10);
      szTable.push_back(15);
      szTable.push_back(10);

      tbDSSV.setFixedWidths(szTable);
      tbDSSV.setFindOptionColumnIndex(0);
      add(&tbDSSV);

      btnCancel = Button_t(left + 78, tbDSSV.getTop() + 21, "Thoat");
      add(&btnCancel);

      btnChon = Button_t(btnCancel.getLeft() - 10, btnCancel.getTop(), "Chon");
      add(&btnChon);
    }

    void draw(Graphics& g) {
      reloadData();

      Panel_t::draw(g);
    }

    bool mouseClick(int l, int t, Graphics& g) {
      if (! Panel_t::mouseClick(l, t, g)) return false;

      if (btnChon.isFocus()) {
        if (tbDSSV.empty()) {
          showMessageDialog("Danh sach sinh vien rong!", MessageBox_t::ERROR_MESSAGE);
          return false;
        }

        Vector<string> selectedRow = tbDSSV.getSelectedValue();

        maLopChon = selectedRow[4];
        maSVChon = selectedRow[0];

        setCloseFlag(true);
      } else if (btnCancel.isFocus()) {
        setCloseFlag(true);
      }

      return true;
    }
  };

  //Danh sach lop
  DSLop_t *dsLop;

  //Danh sach mon hoc
  DSMonHoc_t *dsMon;

  //Thong tin ma sv da chon
  Label_t lbMaSV;
  TextField_t tfMaSV;

  Button_t btnTraCuu;
  Button_t btnChonSV;

  Label_t lbThongTinSV;

  //Chon ma mon
  Label_t lbMaMon;
  ComboBox_t cmbMaMon;

  //Thong bao diem
  Label_t lbDiem;

  //DS cau hoi thi
  Label_t lbDSCauHoi;
  //Truong: stt, lua chon sv, dap an, ket qua(dung/sai)
  Table_t tbDSCauHoi;

  Label_t lbCauHoi;
  Label_t lbDe;
  Label_t lbLuaChon[4];
  Label_t lbDapAn;

  //Danh sach sv theo lop
  Vector<Vector<SinhVien *> > dssv;

  SinhVien *currentSinhVien;

  bool DSSVEmpty() {
    bool dssvEmptyFlag = true;
    for (int i = 0; i < dssv.size(); ++i) {
      dssvEmptyFlag &= dssv[i].empty();
      if (! dssvEmptyFlag) break;
    }
    return dssvEmptyFlag;
  }

  //Danh sach cau hoi thi ma mon
  Vector<Vector<CauHoiThi *> > dsCauHoiThi;

  //======================================================================
  void capNhatMaSinhVienChon(const string& maLopSV, bool drawFlag) {
    lbThongTinSV.clearValue();

    if (currentSinhVien != NULL) {
      tfMaSV.setValue(currentSinhVien->getMaSV());

      stringstream line;

      string hoTenSV = "Ho ten: " + currentSinhVien->getHo() + " " + currentSinhVien->getTen();
      StringUtility::adjustStringToFit(hoTenSV, 60);

      string lopSV = "Lop: " + maLopSV;
      StringUtility::adjustStringToFit(lopSV, 60);

      line << hoTenSV << '\n';
      line << lopSV;

      lbThongTinSV.setValue(line.str());
    } else {
      tfMaSV.clear();
    }

    if (drawFlag) {
      tfMaSV.draw(*Graphics::getCurrentActiveGraphics());
      lbThongTinSV.draw(*Graphics::getCurrentActiveGraphics());
    }

    capNhatDSMonDaThi(drawFlag);
  }

  void capNhatNoiDungCauHoi(Graphics& g, bool drawFlag) {
    lbDe.clearValue();

    for (int i = 0; i < 4; ++i) {
      lbLuaChon[i].clearValue();
    }

    lbDapAn.clearValue();

    if (! tbDSCauHoi.empty()) {
      int idMon = cmbMaMon.getSelectedIndex();

      assert(idMon >= 0);

      int rowTableSelectedId = tbDSCauHoi.getSelectedIndex();

      CauHoiThi *cht = dsCauHoiThi[idMon][rowTableSelectedId];

//      lbCauHoi.setValue(cht->getCauHoi());
      lbDe.setValue(getTextToken(cht->getCauHoi(), 5, 80));

      Vector<string> luaChon(4, "");

      luaChon = cht->getLuaChon();

      for (int i = 0; i < 4; ++i) {
        luaChon[i] = getTextToken(luaChon[i], 2, 80);
        lbLuaChon[i].setValue(string(1, 'A' + i) + ". " + luaChon[i]);
      }

      lbDapAn.setValue("Dap an: " + string(1, cht->getDapAn()));
    }

    if (drawFlag) {
      lbDe.draw(g);

      for (int i = 0; i < 4; ++i) {
        lbLuaChon[i].draw(g);
      }

      lbDapAn.draw(g);
    }
  }

  void capNhatDSCauHoiThi(bool drawFlag) {
    dsCauHoiThi.clear();

    //Sinh vien da thi it nhat 1 mon
    if (! cmbMaMon.empty()) {
      assert(currentSinhVien != NULL);

      SLLNode<DiemThi> *dsDiemPtr = currentSinhVien->getDSMonDaThi()->begin();

      while (dsDiemPtr != NULL) {
        dsCauHoiThi.push_back(dsDiemPtr->key.getDSCauHoiThi());
        dsDiemPtr = dsDiemPtr->next;
      }
    }

    tbDSCauHoi.setData(getTableData());

    if (drawFlag) {
      tbDSCauHoi.draw(*Graphics::getCurrentActiveGraphics());
    }

    capNhatNoiDungCauHoi(*Graphics::getCurrentActiveGraphics(), drawFlag);
  }

  void capNhatThongBaoDiem(bool drawFlag) {
    lbDiem.clearValue();

    if (currentSinhVien == NULL) return;

    if (cmbMaMon.empty()) return;

    string maMonChon = cmbMaMon.getSelectedValue();

    int diem = currentSinhVien->getDSMonDaThi()->getDiemThi(maMonChon)->getDiem();

    lbDiem.setValue("Diem: " + StringUtility::to_string(diem));

    if (drawFlag) {
      lbDiem.draw(*Graphics::getCurrentActiveGraphics());
    }
  }

  void capNhatDSMonDaThi(bool drawFlag) {
    if (currentSinhVien == NULL) {
      cmbMaMon.clear();
    } else {
      cmbMaMon.setData(currentSinhVien->getDSMonDaThi()->getDSMaMon());
    }

    if (drawFlag) {
      cmbMaMon.draw(*Graphics::getCurrentActiveGraphics());
    }

    capNhatDSCauHoiThi(drawFlag);

    capNhatThongBaoDiem(drawFlag);
  }

  Vector<Vector<string> > getTableData() {
    Vector<Vector<string> > res;

    if (currentSinhVien != NULL && !cmbMaMon.empty()) {
      string maMonDaThiChon = cmbMaMon.getSelectedValue();

      int idMonDaThiChon = cmbMaMon.getSelectedIndex();

      DiemThi *dt = currentSinhVien->getDiemMonDaThi(maMonDaThiChon);

      assert(dt != NULL);

      Vector<char> dsCauTraLoi = dt->getDSCauTraLoi();

      assert(dsCauTraLoi.size() > 0);

      int soCauHoi = dsCauHoiThi[idMonDaThiChon].size();

      assert(soCauHoi == dsCauTraLoi.size());

      res.resize(soCauHoi);

      for (int i = 0; i < res.size(); ++i) {
        char traLoi = dsCauTraLoi[i];
        char dapAn = dsCauHoiThi[idMonDaThiChon][i]->getDapAn();

        res[i].push_back(StringUtility::to_string(i + 1));
        res[i].push_back(string(1, traLoi));
        res[i].push_back(string(1, dapAn));
        res[i].push_back(traLoi == dapAn ? "Dung" : "Sai");
      }
    }

    return res;
  }

  friend class GVExecutiveForm;

public:
  DiemThiSVPanel()
  : Panel_t(BorderFactory::PLAIN, 0, 1, {170,44}),
    dsLop(DSLop_t::getDSLopHienHanh()),
    dsMon(DSMonHoc_t::getDSMonHocHienHanh()),
    currentSinhVien(NULL) {

    setColor(0xf0);

    //lbMaSV ---> tfMaSV
    //btnChon
    //btnTraCuu
    //|
    //lbMaMon ---> Combobox Chon ma mon
    lbMaSV = Label_t(left + 3, top + 2, "Ma so sinh vien:");
    lbMaSV.setColor(this->attribute);
    add(&lbMaSV);

    tfMaSV = TextField_t(BorderFactory::SINGLE, lbMaSV.getLeft() + lbMaSV.getCord().X + 3, lbMaSV.getTop() - 1, 20, 30);
    add(&tfMaSV);

    //Button chon sv
    btnChonSV = Button_t(tfMaSV.getLeft() + tfMaSV.getCord().X - 18, lbMaSV.getTop() + 3, "Chon");
    add(&btnChonSV);

    //Btn tra cuu
    btnTraCuu = Button_t(btnChonSV.getLeft() + btnChonSV.getCord().X + 3, btnChonSV.getTop(), "Tra cuu");
    add(&btnTraCuu);

    lbThongTinSV = Label_t(BorderFactory::SINGLE, lbMaSV.getLeft(), lbMaSV.getTop() + 5, "");
    lbThongTinSV.setColor(this->attribute);
    add(&lbThongTinSV);

    //Chon ma mon
    lbMaMon = Label_t(lbMaSV.getLeft(), lbThongTinSV.getTop() + 6, "Danh sach mon da thi:");
    lbMaMon.setColor(this->attribute);
    add(&lbMaMon);

    cmbMaMon = ComboBox_t(lbMaMon.getLeft() + lbMaMon.getCord().X + 2, lbMaMon.getTop() - 1, 3);
    add(&cmbMaMon);

    lbDSCauHoi = Label_t(BorderFactory::SINGLE, lbMaSV.getLeft(), lbMaMon.getTop() + 3, "Danh sach cau hoi:");
    lbDSCauHoi.setColor(this->attribute);
    add(&lbDSCauHoi);

    Vector<string> tableHeader;
    tableHeader.push_back("STT");
    tableHeader.push_back("Tra Loi");
    tableHeader.push_back("Dap An");
    tableHeader.push_back("Ket Qua");

    tbDSCauHoi = Table_t(lbDSCauHoi.getLeft(), lbDSCauHoi.getTop() + 4, 0, 15, tableHeader);

    Vector<size_t> szTable;
    szTable.push_back(10);
    szTable.push_back(10);
    szTable.push_back(10);
    szTable.push_back(15);

    tbDSCauHoi.setFixedWidths(szTable);
    tbDSCauHoi.setFindOptionColumnIndex(0);
    add(&tbDSCauHoi);

    //Thong bao diem
    lbDiem = Label_t(BorderFactory::SINGLE, lbMaSV.getLeft(), lbDSCauHoi.getTop() + 25, "Diem:    ");
    lbDiem.setColor(this->attribute);
    add(&lbDiem);

    //De: ...
    //
    //--3-->A.
    //--3-->B.
    //--3-->C.
    //--3-->D.
    //Dap an: ...
    lbCauHoi = Label_t(BorderFactory::SINGLE, 70, top + 5, "Cau hoi:");
    lbCauHoi.setColor(this->attribute);
    add(&lbCauHoi);

    lbDe = Label_t(lbCauHoi.getLeft() + 11, lbCauHoi.getTop() + 1, "");
    lbDe.setColor(this->attribute);
    add(&lbDe);

    int nextTop = lbCauHoi.getTop() + 10;

    for (int i = 0; i < 4; ++i) {
      lbLuaChon[i] = Label_t(lbDe.getLeft(), nextTop, string(1, 'A' + i));
      lbLuaChon[i].setColor(this->attribute);
      add(&lbLuaChon[i]);
      nextTop += 3;
    }

    lbDapAn = Label_t(lbCauHoi.getLeft() + 1, nextTop, "Dap an: ");
    lbDapAn.setColor(this->attribute);
    add(&lbDapAn);
  }

  void reloadData() {
    currentSinhVien = NULL;

    dssv.resize(dsLop->getSoLop());

    string maLopDauTien = "";

    for (int i = 0; i < dssv.size(); ++i) {
      dssv[i].clear();
      dsLop->getLop(i)->getDanhSachSinhVien(dssv[i]);
    }

    //Mac dinh chon sv dau tien
    if (! dssv.empty() && ! dssv[0].empty()) {
      currentSinhVien = dssv[0][0];
    }

    //load danh sach ma mon
    cmbMaMon.setData(dsMon->getDSMaMon());

    string maLopSV;

    if (!dsLop->empty()) {
      maLopSV = dsLop->getLop(0)->getMaLop();
    }

    capNhatMaSinhVienChon(maLopSV, false);
  }

  void draw(Graphics& g) {
    reloadData();

    Panel_t::draw(g);
  }

  bool mouseClick(int l, int t, Graphics& g) {
    int preCmbMaMonSelectedIndex = cmbMaMon.getSelectedIndex();

    int preTbDSCauHoiThiSelectedIndex = tbDSCauHoi.getSelectedIndex();

    if (! Panel_t::mouseClick(l, t, g)) return false;

    if (btnTraCuu.isFocus()) {
      if (DSSVEmpty()) {
        showMessageDialog("Danh sach sinh vien hien tai rong.\nVui long tao them sinh vien.", MessageBox_t::ERROR_MESSAGE);
        return false;
      }

      string maSV = tfMaSV.getValue();

      StringUtility::Trim(maSV);
      StringUtility::UpperCaseAllLetter(maSV);

      if (maSV.empty()) {
        showMessageDialog("Ban chua nhap sinh vien!\n*Goi y: Nhan nut \"Chon\" de mo hop thoai chon sinh vien", MessageBox_t::INFORMATION_MESSAGE);
        Rect_t::setFocus(tfMaSV);
        return true;
      }

      //kiem tra ma sinh vien co chua ki tu khac chu va so
      if (StringUtility::HavingPunctChar(maSV)) {
        showMessageDialog("Ma sinh vien chi bao gom chu cai va so!", MessageBox_t::ERROR_MESSAGE);
        Rect_t::setFocus(tfMaSV);
        return true;
      }

      //kiem tra ma sinh vien co khoang trang
      if (! StringUtility::IsAlNum(maSV)) {
        showMessageDialog("Ma sinh vien khong cho phep co khoang trang!", MessageBox_t::ERROR_MESSAGE);
        Rect_t::setFocus(tfMaSV);
        return true;
      }

      SinhVien *sv = NULL;

      for (int lopId = 0; lopId < dssv.size(); ++lopId) {
        for (int svId = 0; svId < dssv[lopId].size(); ++svId) {
          sv = dssv[lopId][svId];
          if (sv->getMaSV() == maSV) {
            currentSinhVien = sv;
            capNhatMaSinhVienChon(dsLop->getLop(lopId)->getMaLop(), true);

            return true;
          }
        }
      }

      showMessageDialog("Ma sinh vien nhap khong hop le!\n*Goi y: Nhan nut \"Chon\" de mo hop thoai chon sinh vien", MessageBox_t::ERROR_MESSAGE);
      Rect_t::setFocus(tfMaSV);
      return true;

    } else if (btnChonSV.isFocus()) {
      ChonSVPanel *pnChonSV = new ChonSVPanel(this);
      EventEngine process(EventEngine::getCurrentProcess());
      process.setMainFrame(*pnChonSV);
      process.run(false);

      string maLopChon = pnChonSV->maLopChon;
      string maSVChon = pnChonSV->maSVChon;

      if (maSVChon != "" && (currentSinhVien == NULL || currentSinhVien->getMaSV() != maSVChon)) {
        currentSinhVien = dsLop->getLop(maLopChon)->getSinhVien(maSVChon);
        capNhatMaSinhVienChon(maLopChon, true);
      }

      delete pnChonSV;
    } else if (cmbMaMon.isFocus()) {
      int newCmbMaMonSelectedIndex = cmbMaMon.getSelectedIndex();

      if (preCmbMaMonSelectedIndex != newCmbMaMonSelectedIndex) {
        capNhatDSCauHoiThi(true);
      }
    } else if (tbDSCauHoi.isFocus()) {
      capNhatNoiDungCauHoi(*Graphics::getCurrentActiveGraphics(), true);
    }

    return true;
  }

  void keyPress(int keyCode, char ch, Graphics& g) {
    //DSSV rong
    if (DSSVEmpty()) return;

    if (keyCode == KEY_ENTER && tfMaSV.isFocus()) {
      string maSV = tfMaSV.getValue();

      StringUtility::Trim(maSV);
      StringUtility::UpperCaseAllLetter(maSV);

      if (maSV.empty()) {
        showMessageDialog("Ban chua nhap ma so sinh vien!\n*Goi y: Nhan nut \"Chon\" de mo hop thoai chon sinh vien", MessageBox_t::INFORMATION_MESSAGE);
        Rect_t::setFocus(tfMaSV);
        return;
      }

      //kiem tra ma sinh vien co chua ki tu khac chu va so
      if (StringUtility::HavingPunctChar(maSV)) {
        showMessageDialog("Ma sinh vien chi bao gom chu cai va so!", MessageBox_t::ERROR_MESSAGE);
        Rect_t::setFocus(tfMaSV);
        return;
      }

      //kiem tra ma sinh vien co khoang trang
      if (! StringUtility::IsAlNum(maSV)) {
        showMessageDialog("Ma sinh vien khong cho phep co khoang trang!", MessageBox_t::ERROR_MESSAGE);
        Rect_t::setFocus(tfMaSV);
        return;
      }

      SinhVien *sv = NULL;

      for (int lopId = 0; lopId < dssv.size(); ++lopId) {
        for (int svId = 0; svId < dssv[lopId].size(); ++svId) {
          sv = dssv[lopId][svId];
          if (sv->getMaSV() == maSV) {
            currentSinhVien = sv;
            capNhatMaSinhVienChon(dsLop->getLop(lopId)->getMaLop(), true);

            return;
          }
        }
      }

      showMessageDialog("Ma sinh vien nhap khong hop le!\n*Goi y: Nhan nut \"Chon\" de mo hop thoai chon sinh vien", MessageBox_t::ERROR_MESSAGE);
      Rect_t::setFocus(tfMaSV);
    }
  }
};

#endif // DIEMTHISINHVIENPANEL_H_INCLUDED
