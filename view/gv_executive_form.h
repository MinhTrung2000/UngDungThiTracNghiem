#ifndef GV_EXECUTIVE_FORM_H_INCLUDED
#define GV_EXECUTIVE_FORM_H_INCLUDED

#include "UIForm.h"
#include "../control/Panel_t.h"
#include "../control/MessageBox_t.h"
#include "../control/Button_t.h"
#include "../control/ComboBox_t.h"
#include "../control/CheckList_t.h"
#include "../control/RadioBox_t.h"
#include "../control/TextBox_t.h"
#include "../control/TextField_t.h"
#include "../control/TextArea_t.h"
#include "../control/Menu_t.h"
#include "../control/MenuBar_t.h"

#include "DSMonHocPanel.h"
#include "DSLopPanel.h"
#include "DSSinhVienPanel.h"
#include "DiemThiLopPanel.h"
#include "DiemThiSinhVienPanel.h"
#include "CauHoiThiPanel.h"

using namespace std;

/*
  +>Mon Hoc menu:
    + Mon hoc panel

  +>Lop menu:
    + Danh sach lop
    + Danh sach sinh vien theo lop

  +>Diem thi menu:
    + Bang diem mon hoc cua lop panel
    + Bang diem mon hoc cua sinh vien panel

  +>Ngan hang de thi menu:
    + cau hoi thi panel
*/
class GVExecutiveForm : public UIForm {
private:
  Panel_t *logForm;

  MenuBar_t menuBar;

  MonHocPanel pnQuanLiMonHoc;

  DSLopPanel pnQuanLiDSLop;

  DSSinhVienPanel pnQuanLiDSSV;

  DiemThiLopPanel pnQuanLiDiemThiLop;

  DiemThiSVPanel pnQuanLiDiemThiSV;

  CauHoiThiPanel pnCauHoiThi;

public:
  GVExecutiveForm(Panel_t *_logForm)
  : UIForm(BorderFactory::DOUBLE, 0, 0, {170, 45}, ""),
    logForm(_logForm) {

    setColor(0x79);

    Menu_t menus[4];
    menus[0] = Menu_t("Mon Hoc");
    menus[0].setPanelLinkAtIndex(0, &pnQuanLiMonHoc);

    Vector<string> menuItems1;
    menuItems1.push_back("Danh sach lop hoc");
    menuItems1.push_back("Danh sach sinh vien theo lop");

    menus[1] = Menu_t("Lop Hoc", menuItems1);
    menus[1].setPanelLinkAtIndex(0, &pnQuanLiDSLop);
    menus[1].setPanelLinkAtIndex(1, &pnQuanLiDSSV);

    Vector<string> menuItems2;
    menuItems2.push_back("Diem mon hoc cua lop");
    menuItems2.push_back("Diem mon hoc cua sinh vien");

    menus[2] = Menu_t("Diem Thi", menuItems2);
    menus[2].setPanelLinkAtIndex(0, &pnQuanLiDiemThiLop);
    menus[2].setPanelLinkAtIndex(1, &pnQuanLiDiemThiSV);

    menus[3] = Menu_t("Ngan Hang Cau Hoi Thi");
    menus[3].setPanelLinkAtIndex(0, &pnCauHoiThi);

    for (int i = 0; i < 4; ++i) {
      menuBar.addMenu(menus[i]);
    }

    Menu_t mnDangXuat("Dang Xuat");

    mnDangXuat.setPanelLinkAtIndex(0, logForm);
    mnDangXuat.setCoor(159, 0);

    menuBar.addMenu(mnDangXuat, false);

    add(&menuBar);

    add(&pnQuanLiMonHoc);
    add(&pnQuanLiDSLop);
    add(&pnQuanLiDSSV);
    add(&pnQuanLiDiemThiLop);
    add(&pnQuanLiDiemThiSV);
    add(&pnCauHoiThi);

    //Mac dinh hien thi Mon hoc panel
    menuBar.setDisplayPanel(&pnQuanLiMonHoc);
  }

  ~GVExecutiveForm() {
    logForm->setReRunFlag(true);
  }

  void draw(Graphics& g) {
    menuBar.draw(g);
  }

  bool mouseClick(int l, int t, Graphics& g) {
    //items are only menubar and panels
    if (menuBar.mouseClick(l, t, g)) return true;

    Rect_t *activePanel = menuBar.getDisplayPanel();
    if (activePanel != NULL)
      return activePanel->mouseClick(l, t, g);
  }
};

#endif // GV_EXECUTIVE_FORM_H_INCLUDED
