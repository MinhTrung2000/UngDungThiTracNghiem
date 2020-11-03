#ifndef LOGIN_FORM_H
#define LOGIN_FORM_H

#include <stdlib.h>
#include "UIForm.h"
#include "../model/DSLop_t_model.h"

#include "../control/Button_t.h"
#include "../control/Label_t.h"
#include "../control/TextField_t.h"
#include "../control/PasswordField_t.h"
#include "../control/MessageBox_t.h"

#include "sv_executive_form.h"
#include "gv_executive_form.h"

class LoginForm : public UIForm {
private:
  // POSITION
  const int PN_LOGIN_X = 49;
  const int PN_LOGIN_Y = 16;
  const COORD PN_LOGIN_CORD = {62, 13};

  const int LB_USERNAME_X = PN_LOGIN_X + 5;
  const int LB_USERNAME_Y = PN_LOGIN_Y + 4;

  const int TF_USERNAME_X = PN_LOGIN_X + 23;
  const int TF_USERNAME_Y = PN_LOGIN_Y + 3;
  const int TF_USERNAME_WID = 30;

  const int LB_PSW_X = PN_LOGIN_X + 5;
  const int LB_PSW_Y = PN_LOGIN_Y + 7;

  const int PW_PSW_X = PN_LOGIN_X + 23;
  const int PW_PSW_Y = PN_LOGIN_Y + 6;
  const int PW_PSW_WID = 30;

  const int BTN_ENTER_X = PN_LOGIN_X + 16;
  const int BTN_ENTER_Y = PN_LOGIN_Y + 11;

  const int BTN_EXIT_X = PN_LOGIN_X + 40;
  const int BTN_EXIT_Y = PN_LOGIN_Y + 11;

  static const string GV_USERNAME;
  static const string GV_PSW;

  // =================================
  static const string DSMONHOC_FILE_PATH;
  static const string DSCAUHOITHI_FILE_PATH;
  static const string DSLOP_FILE_PATH;

  bool loadMonHocDataSuccessfullFlag;
  bool loadDSLopDataSuccessfullFlag;
  bool loadDSCauHoiThiDataSuccessfullFlag;

  DSLop_t *dsLop;

  //Danh sach tat ca sinh vien
  Vector<SinhVien *> dssv;

  Label_t lbUsername;
	TextField_t tfUsername;

	Label_t lbPassword;
	PasswordField_t pwPassword;

	Button_t btnEnter, btnExit;

	void chooseEnterButton(Graphics& g) {
    string inpUsername = tfUsername.getValue();
    string inpPsw = pwPassword.getValue();

    StringUtility::Trim(inpUsername);
    StringUtility::Trim(inpPsw);

    if (inpUsername.empty()) {
      showMessageDialog("Loi Dang Nhap!", "Username khong duoc de trong.", MessageBox_t::ERROR_MESSAGE);
      Rect_t::setFocus(tfUsername);
    } else if (inpPsw.empty()) {
      showMessageDialog("Loi Dang Nhap!", "Password khong duoc de trong.", MessageBox_t::ERROR_MESSAGE);
      Rect_t::setFocus(pwPassword);
    } else {
      if (inpUsername == GV_USERNAME && inpPsw == GV_PSW) {
        //Giao vien enter
        GVExecutiveForm *formForGV = new GVExecutiveForm(this);
        UIForm::drawForm(formForGV, true);
        return;
      } else {
        //Sinh Vien enter
        for (int i = 0; i < dssv.size(); ++i) {
          if (inpUsername == dssv[i]->getMaSV() &&
              inpPsw == dssv[i]->getMatKhau()) {
            if (DSMonHoc_t::getDSMonHocHienHanh()->empty()) {
              showMessageDialog("Hien tai chua co mon hoc co san.\nSinh vien khong the thi luc nay!", MessageBox_t::ERROR_MESSAGE);
              return;
            }

            SVExecutiveForm *formForSV = new SVExecutiveForm(dssv[i], this);
            UIForm::drawForm(formForSV, true);
            return;
          }
        }
      }

      showMessageDialog("Ten hoac mat khau khong hop le!", MessageBox_t::ERROR_MESSAGE);
    }

    btnExit.setColor(DEFAULT_BUTTON_UNFOCUS_ATTRIBUTES);
    btnExit.draw(g);

    btnEnter.setColor(DEFAULT_BUTTON_FOCUS_ATTRIBUTES);
    btnEnter.draw(g);
	}

	void chooseExitButton(Graphics& g) {
    string choice = showOptionDialog("Xac nhan thoat?", MessageBox_t::WARNING_MESSAGE, MessageBox_t::OK_CANCEL_OPTION);
    if (choice == "OK") {
      EventEngine::setOnStopAllProcessFlag();
    }

    btnEnter.setColor(DEFAULT_BUTTON_UNFOCUS_ATTRIBUTES);
    btnEnter.draw(g);

    btnExit.setColor(DEFAULT_BUTTON_FOCUS_ATTRIBUTES);
    btnExit.draw(g);
	}

	friend class ExecutiveForm;

public:
	LoginForm()
	: UIForm(BorderFactory::DOUBLE, 49, 16, {62, 13}, "LOGIN"),
    loadMonHocDataSuccessfullFlag(true),
	  loadDSLopDataSuccessfullFlag(true),
    loadDSCauHoiThiDataSuccessfullFlag(true) {

    setDefaultColor();

    //set up data from file
    if (!readFileDanhSachCauHoiThi()) {
      loadDSCauHoiThiDataSuccessfullFlag = false;
    }
    if (!readFileDanhSachMonHoc()) {
      loadMonHocDataSuccessfullFlag = false;
    }
    if (!readFileDanhSachLop()) {
      loadDSLopDataSuccessfullFlag = false;
    }

    dsLop = DSLop_t::getDSLopHienHanh();

    lbUsername = Label_t(LB_USERNAME_X, LB_USERNAME_Y, "Username:");
    lbUsername.setColor(this->attribute);
    add(&lbUsername);

    tfUsername = TextField_t(BorderFactory::SINGLE, TF_USERNAME_X, TF_USERNAME_Y, TF_USERNAME_WID);
    add(&tfUsername);

    lbPassword = Label_t(LB_PSW_X, LB_PSW_Y, "Password:");
    lbPassword.setColor(this->attribute);
    add(&lbPassword);

    pwPassword = PasswordField_t(BorderFactory::SINGLE, PW_PSW_X, PW_PSW_Y, PW_PSW_WID);
    add(&pwPassword);

    btnEnter = Button_t(BTN_ENTER_X, BTN_ENTER_Y, "Enter");
    add(&btnEnter);

    btnExit = Button_t(BTN_EXIT_X, BTN_EXIT_Y, "Exit");
    add(&btnExit);
  }

  ~LoginForm() {
    if (! writeFileDanhSachCauHoiThi()) {
      showMessageDialog("Loi He Thong", "Khong the luu du lieu ngan hang de thi.", MessageBox_t::ERROR_MESSAGE);
    }
    if (! writeFileDanhSachMonHoc()) {
      showMessageDialog("Loi He Thong", "Khong the luu du lieu mon hoc.", MessageBox_t::ERROR_MESSAGE);
    }
    if (! writeFileDanhSachLop()) {
      showMessageDialog("Loi He Thong", "Khong the luu du lieu danh sach lop.", MessageBox_t::ERROR_MESSAGE);
    }
  }

  void reloadData() {
    tfUsername.clear();
    pwPassword.clear();

    dssv.clear();

    dssv = dsLop->getTatCaSinhVien();
  }

  void draw(Graphics& g) {
    reloadData();

    Panel_t::draw(g);

    if (! loadDSCauHoiThiDataSuccessfullFlag) {
      showMessageDialog("Loi He Thong", "Khong the tai du lieu ngan hang de thi.", MessageBox_t::ERROR_MESSAGE);
    }
    if (! loadMonHocDataSuccessfullFlag) {
      showMessageDialog("Loi He Thong", "Khong the tai du lieu mon hoc.", MessageBox_t::ERROR_MESSAGE);
    }
    if (! loadDSLopDataSuccessfullFlag) {
      showMessageDialog("Loi He Thong", "Khong the tai du lieu danh sach lop.", MessageBox_t::ERROR_MESSAGE);
    }
  }

  bool mouseClick(int l, int t, Graphics& g) {
    if (! Panel_t::mouseClick(l, t, g)) return false;

    if (btnEnter.isFocus()) {
      chooseEnterButton(g);
    } else if (btnExit.isFocus()) {
      chooseExitButton(g);
    }

    return true;
  }

  void keyPress(int keyCode, char ch, Graphics& g) {
    switch (keyCode) {
      case KEY_ENTER: {
        if (tfUsername.isFocus() || pwPassword.isFocus()) {
            chooseEnterButton(g);
          }
        break;
      }
      case KEY_LEFT: {
        if (btnExit.isFocus()) {
          btnExit.setColor(DEFAULT_BUTTON_UNFOCUS_ATTRIBUTES);
          btnExit.draw(g);

          btnEnter.setColor(DEFAULT_BUTTON_FOCUS_ATTRIBUTES);
          Rect_t::setFocus(btnEnter);
        }
        break;
      }
      case KEY_RIGHT: {
        if (btnEnter.isFocus()) {
          btnEnter.setColor(DEFAULT_BUTTON_UNFOCUS_ATTRIBUTES);
          btnEnter.draw(g);

          btnExit.setColor(DEFAULT_BUTTON_FOCUS_ATTRIBUTES);
          Rect_t::setFocus(btnExit);
        }
        break;
      }
    }
  }

  //Load file mon hoc
  bool readFileDanhSachMonHoc() {
    ifstream fr(DSMONHOC_FILE_PATH.c_str());
    if (!fr.good()) {
      fr.close();
      return false;
    }

    fr >> *DSMonHoc_t::getDSMonHocHienHanh();

    fr.close();
    return true;
  }

  //Ghi file mon hoc
  bool writeFileDanhSachMonHoc() {
    ofstream fw(DSMONHOC_FILE_PATH.c_str());
    if (!fw.good()) {
      fw.close();
      return false;
    }

    fw << *DSMonHoc_t::getDSMonHocHienHanh();

    fw.close();
    return true;
  }

  //Load file DS lop
  bool readFileDanhSachLop() {
    ifstream fr(DSLOP_FILE_PATH.c_str());
    if (!fr.good()) {
      fr.close();
      return false;
    }

    fr >> *DSLop_t::getDSLopHienHanh();

    fr.close();
    return true;
  }

  //Ghi file DS lop
  bool writeFileDanhSachLop() {
    ofstream fw(DSLOP_FILE_PATH.c_str());
    if (!fw.good()) {
      fw.close();
      return false;
    }

    fw << *DSLop_t::getDSLopHienHanh();

    fw.close();
    return true;
  }

  //Load file DS cau hoi thi
  bool readFileDanhSachCauHoiThi() {
    ifstream fr(DSCAUHOITHI_FILE_PATH.c_str());
    if (!fr.good()) {
      fr.close();
      return false;
    }

    fr >> *DSCauHoiThi_t::getDSCauHoiThiHienHanh();

    fr.close();
    return true;
  }

  //Ghi file DS cau hoi thi
  bool writeFileDanhSachCauHoiThi() {
    ofstream fw(DSCAUHOITHI_FILE_PATH.c_str());
    if (!fw.good()) {
      fw.close();
      return false;
    }

    fw << *DSCauHoiThi_t::getDSCauHoiThiHienHanh();

    fw.close();
    return true;
  }
};

const string LoginForm::GV_USERNAME = "GV";
const string LoginForm::GV_PSW = "GV";

const string LoginForm::DSMONHOC_FILE_PATH = "data/DSMonHoc.data";
const string LoginForm::DSCAUHOITHI_FILE_PATH = "data/NganHangDeThi.data";
const string LoginForm::DSLOP_FILE_PATH = "data/DSLop.data";

#endif
