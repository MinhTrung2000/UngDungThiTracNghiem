#include <cstring>
#include <string>
#include <cassert>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>

#include "control/Menu_t.h"
#include "control/Table_t.h"
#include "control/ComboBox_t.h"
#include "control/RadioBox_t.h"
#include "control/TextBox_t.h"

#include "view/login_form.h"

using namespace std;

const int WID_SCR = 170;
const int HGT_SCR = 45;


void InitConsole() {
  Graphics g = *Graphics::getCurrentActiveGraphics();
  assert(Graphics::getCurrentActiveGraphics() != NULL);
	g.setConsoleTitle("Phan Mem Thi Trac Nghiem");
	g.setScreenSize(WID_SCR + 2, HGT_SCR + 2);
	g.setColor(0xf0);
	g.clearScreen();
}

///*
int main() {
	InitConsole();

  LoginForm *loginForm = new LoginForm;

  UIForm::drawForm(loginForm, true);

	return 0;
}
//*/

/*
int main() {
	InitConsole();

  EventEngine globalProcess;

  Panel_t mainPanel(BorderFactory::DOUBLE, 0, 0, {WID_SCR, HGT_SCR});
  globalProcess.setMainFrame(mainPanel);

//  mainPanel.add(&loginForm);
//  mainPanel.add(&svForm);

  Vector<string> v;
  v.push_back("lsfjajsdfasfasfaf;la");
  v.push_back("lsfsfasfafaja");
  v.push_back("lsfjasfafaaafjf;la");
  v.push_back("fafa");
  v.push_back(string(4, 'a'));
  v.push_back(string(5, '1'));
  v.push_back("lsfjajf;laafs");
  v.push_back("aieuiowue");
//
//  Vector<string> v2;
//  v2.push_back(";la");
//  v2.push_back("lfajsklj");
//  v2.push_back("156465135");
//  v2.push_back("1111111");
//  v2.push_back("222222222");
//  v2.push_back("33333333333");
//  v2.push_back("444444444");
//  v2.push_back("555555555");
//  v2.push_back("878afsfsaf");
//  v2.push_back("878afsfsaf");
//  v2.push_back("878afsfsaf");
//  v2.push_back("878afsfsaf");
//  v2.push_back("878afsfsaf");
//  v2.push_back("878afsfsaf");
//  v2.push_back(" kfjlasjf klask fks  ");
//
  CheckList_t chLista(60, 3, 5, CheckBox_t::BOX_STYLE, v);
//  CheckList_t chLista(60, 3, 3, CheckBox_t::BOX_STYLE);
//  CheckList_t chLista(60, 3, 3, CheckBox_t::BOX_STYLE, v);

//  chLista.insertNewItem(2,"new item......sakldfja");
//  chLista.deleteItem(3);
//  chLista.replaceItem(5,"replace it");

  mainPanel.add(&chLista);
//
  RadioBox_t chBox(100, 10, 4, CheckBox_t::BOX_STYLE, v);
//  chBox.setForceChecked();
  mainPanel.add(&chBox);

  TextBox_t tBox(100, 30, 7);
  tBox.setData(v);
  mainPanel.add(&tBox);
//


//  MenuBar_t mnb;
////  mainPanel.add(&mnb);
//
//  Menu_t mns[4];
//  mns[0] = Menu_t("Mon");
////  mns[0].setPanelLinkAtIndex(0, &loginForm);
//  mns[1] = Menu_t("Lop", v);
//  mns[2] = Menu_t("Diem Thi", v);
//  mns[3] = Menu_t("Ngan Hang De Thi", v);
//
////  for (int i = 0; i < 4; ++i)
////    mnb.addMenu(mns[i]);
////
  Vector<string> hdrs;
  hdrs.push_back("STT");
  hdrs.push_back("HO");
  hdrs.push_back("TEN");
  hdrs.push_back("NAM SINH");
  hdrs.push_back("GIOI TINH");
  hdrs.push_back("NOI CHON");
  hdrs.push_back("LUONG");

    ComboBox_t comb(20, 15, 3, hdrs);
  mainPanel.add(&comb);
//
  Vector<Vector<string> > data(14);
//
  data[0].push_back("001");
  data[0].push_back("TRAN");
  data[0].push_back("LAM");
  data[0].push_back("01/11/2012");
  data[0].push_back("Nam");
  data[0].push_back("THAI BINH");
  data[0].push_back("100000");

  data[1].push_back("002");
  data[1].push_back("NGUYEN");
  data[1].push_back("TAN TAI");
  data[1].push_back("25/03/1999");
  data[1].push_back("Nam");
  data[1].push_back("THUA THIEN HUEddddddddddddddddddddddddddddddddddddd");
  data[1].push_back("15000");

  data[2].push_back("003");
  data[2].push_back("DOAN");
  data[2].push_back("THI MAN");
  data[2].push_back("13/08/2005");
  data[2].push_back("Nu");
  data[2].push_back("VINH PHUC");
  data[2].push_back("21000");

  data[3].push_back("004");
  data[3].push_back("TRAN");
  data[3].push_back("HOAI NAM");
  data[3].push_back("28/12/2001");
  data[3].push_back("Nam");
  data[3].push_back("BA RIA VUNG TAU");
  data[3].push_back("25000");

  data[4].push_back("005");
  data[4].push_back("PHAM");
  data[4].push_back("ANH");
  data[4].push_back("04/09/1997");
  data[4].push_back("Nu");
  data[4].push_back("TPHCM");
  data[4].push_back("14000");

  data[5].push_back("006");
  data[5].push_back("TRAN");
  data[5].push_back("TIEN DUNG");
  data[5].push_back("11/08/2003");
  data[5].push_back("Nu");
  data[5].push_back("CA MAU");
  data[5].push_back("19500");

  data[6].push_back("007");
  data[6].push_back("TRUONG");
  data[6].push_back("TUAN NGHI");
  data[6].push_back("25/09/1999");
  data[6].push_back("Nam");
  data[6].push_back("VINH LONG");
  data[6].push_back("20000");

  data[7].push_back("008");
  data[7].push_back("HOANG");
  data[7].push_back("QUOC THAI");
  data[7].push_back("12/05/2003");
  data[7].push_back("Nam");
  data[7].push_back("HOC MON");
  data[7].push_back("18000");

  data[8].push_back("009");
  data[8].push_back("HOANG");
  data[8].push_back("THI NGOC CHAU");
  data[8].push_back("04/09/1997");
  data[8].push_back("Nu");
  data[8].push_back("NINH BINH");
  data[8].push_back("16000");

  data[9].push_back("010");
  data[9].push_back("TRAN");
  data[9].push_back("MANH CUONG");
  data[9].push_back("27/07/2003");
  data[9].push_back("Nam");
  data[9].push_back("BEN TRE");
  data[9].push_back("19000");

  data[10].push_back("011");
  data[10].push_back("TRAN");
  data[10].push_back("MANH CUONG");
  data[10].push_back("27/07/2003");
  data[10].push_back("Nam");
  data[10].push_back("BEN TRE");
  data[10].push_back("19000");

  data[11].push_back("012");
  data[11].push_back("TRAN");
  data[11].push_back("MANH CUONG");
  data[11].push_back("27/07/2003");
  data[11].push_back("Nam");
  data[11].push_back("BEN TRE");
  data[11].push_back("19000");

  data[12].push_back("013");
  data[12].push_back("TRAN");
  data[12].push_back("MANH CUONG");
  data[12].push_back("27/07/2003");
  data[12].push_back("Nam");
  data[12].push_back("BEN TRE");
  data[12].push_back("19000");

  data[13].push_back("014");
  data[13].push_back("TRAN");
  data[13].push_back("MANH CUONG");
  data[13].push_back("27/07/2003");
  data[13].push_back("Nam");
  data[13].push_back("BEN TRE");
  data[13].push_back("19000");




  Table_t table(20, 20, 5, hdrs);
  table.setFixedWidths(Vector<size_t>(hdrs.size(), 15));
  table.setData(data);
  mainPanel.add(&table);

//  TextField_t tf(BorderFactory::SINGLE, 20, 30, 20, 50);
////  TextField_t tf(BorderFactory::SINGLE, 20, 30, 20);
//  tf.setValue(" warning: variadic templates only available with -std=c++11 or -std=gnu++11| warning: unknown escape sequence: ");
////  tf.clear();
//  mainPanel.add(&tf);

  globalProcess.run(true);
	return 0;
}
*/
