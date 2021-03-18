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


void InitConsole()
{
    Graphics g = *Graphics::getCurrentActiveGraphics();
    assert(Graphics::getCurrentActiveGraphics() != NULL);
    g.setConsoleTitle("Phan Mem Thi Trac Nghiem");
    g.setScreenSize(WID_SCR + 2, HGT_SCR + 2);
    g.setColor(0xf0);
    g.clearScreen();
}


int main()
{
    InitConsole();

    LoginForm *loginForm = new LoginForm;

    UIForm::drawForm(loginForm, true);

    return 0;
}
