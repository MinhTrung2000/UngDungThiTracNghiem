#ifndef CAUHOITHIPANEL_H_INCLUDED
#define CAUHOITHIPANEL_H_INCLUDED

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

using namespace std;

class CauHoiThiPanel : public Panel_t
{
private:
    struct HorRadioBox_t : public RadioBox_t
    {
        HorRadioBox_t() : RadioBox_t() {}
        HorRadioBox_t(int l, int t, const Vector<string>& str)
        {
            previousIndex = currentIndex = 0;
            maxWidthItem = DEFAULT_MAX_WIDTH;
            port = NULL;

            int nextLeft = 0;
            int wid = 0;

            if (! str.empty())
            {
                allCheckBoxes.push_back(new CheckBox_t(l, t, 0, str[0]));

                nextLeft = l + allCheckBoxes[0]->getCord().X + 3;
                wid = allCheckBoxes[0]->getCord().X;

                for (int i = 1; i < str.size() ; i++)
                {
                    allCheckBoxes.push_back(new CheckBox_t(nextLeft, t, 0, str[i]));
                    nextLeft += allCheckBoxes[i]->getCord().X + 3;
                }

                wid = allCheckBoxes.back()->getLeft() + allCheckBoxes.back()->getCord().X;
            }

            Rect_t::setCoor(l, t, {wid, 2});
            setDefaultColor();
        }
    };

    //Danh sach mon hoc
    DSMonHoc_t *dsMon;

    //Ngan hang cau hoi
    DSCauHoiThi_t *NganHangDeThi;

    //Chuc nang
    Label_t lbChucNang;
    ComboBox_t cmbChucNang;
    Button_t btnThucThi;

    //Chon ma mon
    Label_t lbMaMon;
    ComboBox_t cmbMaMon;

    //Chinh sua de cau
    Label_t lbCauHoi;
    TextArea_t taDe;

    //Chinh sua 4 lua chon A, B, C, D
    Label_t lbLuaChon[4];
    TextArea_t taLuaChon[4];

    //Chinh sua dap an
    Label_t lbDapAn;
    //set force check
    HorRadioBox_t rbDapAn;

    //Title bang ds cau hoi
    Label_t lbDSCauHoi;
    Table_t tbDSCauHoi;

    //Nhan thong bao tong so cau hoi trong ngan hang de
    Label_t lbSoCauHoi;

    //DS tap hop tat ca cau hoi khong phan biet ma mon
    Vector<CauHoiThi *> dsCauHoiThi;

    void capNhatSoCauHoiThi(bool drawFlag)
    {
        lbSoCauHoi.clearValue();
        lbSoCauHoi.setValue("Tong so cau: " + StringUtility::to_string(dsCauHoiThi.size()));
        if (drawFlag)
        {
            lbSoCauHoi.draw(*Graphics::getCurrentActiveGraphics());
        }
    }

    void themOption()
    {
        //Danh sach mon hoc rong
        if (dsMon->empty())
        {
            showMessageDialog("Danh sach mon hoc rong!\nVui long tao mon hoc moi.", MessageBox_t::ERROR_MESSAGE);
            return;
        }

        string maMon = cmbMaMon.getSelectedValue();

        string newDe = taDe.getValue();

        StringUtility::Trim(newDe);

        Vector<string> newLuaChon(4);

        for (int i = 0; i < 4; ++i)
        {
            newLuaChon[i] = taLuaChon[i].getValue();
            StringUtility::Trim(newLuaChon[i]);
        }

        string newDapAn = rbDapAn.getSelectedValue();

        if (newDe.empty())
        {
            showMessageDialog("Cau hoi khong duoc de trong!", MessageBox_t::ERROR_MESSAGE);
            Rect_t::setFocus(taDe);
            return;
        }

        for (int i = 0; i < 4; ++i)
        {
            if (newLuaChon[i].empty())
            {
                showMessageDialog("Lua chon " + string(1, 'A' + i) + " khong duoc de trong!", MessageBox_t::ERROR_MESSAGE);
                Rect_t::setFocus(taLuaChon[i]);
                return;
            }
        }

        CauHoiThi *newCauHoiThi = new CauHoiThi(maMon, newDe, newLuaChon, newDapAn[0]);

        NganHangDeThi->insert(*newCauHoiThi);

        dsCauHoiThi.push_back(newCauHoiThi);

        //cap nhat table
        tbDSCauHoi.insertRow(getTableDataAtIndex(NganHangDeThi->getSoCauHoi() - 1));

        //cap nhat so cau hoi
        capNhatSoCauHoiThi(true);

        generateInformationUIElement(*Graphics::getCurrentActiveGraphics(), true);

        showMessageDialog("Them cau hoi moi thanh cong!", MessageBox_t::INFORMATION_MESSAGE);
    }

    void capNhatOption()
    {
        //Danh sach mon hoc rong
        if (dsMon->empty())
        {
            showMessageDialog("Danh sach mon hoc rong!\nVui long tao mon hoc moi.", MessageBox_t::ERROR_MESSAGE);
            return;
        }

        if (dsCauHoiThi.empty())
        {
            showMessageDialog("Danh sach cau hoi rong!", MessageBox_t::ERROR_MESSAGE);
            return;
        }

        string maMonCapNhat = cmbMaMon.getSelectedValue();

        string deCapNhat = taDe.getValue();

        StringUtility::Trim(deCapNhat);

        Vector<string> luaChonCapNhat(4);

        for (int i = 0; i < 4; ++i)
        {
            luaChonCapNhat[i] = taLuaChon[i].getValue();
            StringUtility::Trim(luaChonCapNhat[i]);
        }

        string dapAnCapNhat = rbDapAn.getSelectedValue();

        if (deCapNhat.empty())
        {
            showMessageDialog("Cau hoi khong duoc de trong!", MessageBox_t::ERROR_MESSAGE);
            Rect_t::setFocus(taDe);
            return;
        }

        for (int i = 0; i < 4; ++i)
        {
            if (luaChonCapNhat[i].empty())
            {
                showMessageDialog("Lua chon " + string(1, 'A' + i) + " khong duoc de trong!", MessageBox_t::ERROR_MESSAGE);
                Rect_t::setFocus(taLuaChon[i]);
                return;
            }
        }

        int id = atoi(tbDSCauHoi.getSelectedValue()[0].c_str()) - 1;

        if (! dsCauHoiThi[id]->getCanModifyFlag())
        {
            showMessageDialog("Cau hoi da duoc cho ra thi! Khong the cap nhat thong tin!", MessageBox_t::ERROR_MESSAGE);
            return;
        }

        dsCauHoiThi[id]->setMaMon(maMonCapNhat);
        dsCauHoiThi[id]->setCauHoi(deCapNhat);
        dsCauHoiThi[id]->setLuaChon(luaChonCapNhat);
        dsCauHoiThi[id]->setDapAn(dapAnCapNhat[0]);

        //Cap nhat bang
        tbDSCauHoi.updateRowByIndex(id, getTableDataAtIndex(id));

        generateInformationUIElement(*Graphics::getCurrentActiveGraphics(), true);

        showMessageDialog("Cap nhat cau hoi thanh cong!", MessageBox_t::INFORMATION_MESSAGE);
    }

    void xoaOption()
    {
        if (dsCauHoiThi.empty())
        {
            showMessageDialog("Danh sach cau hoi rong!", MessageBox_t::ERROR_MESSAGE);
            return;
        }

        string rep = showOptionDialog("Xac nhan xoa cau hoi thi?", MessageBox_t::WARNING_MESSAGE, MessageBox_t::OK_CANCEL_OPTION);

        if (rep == "CANCEL")
            return;

//    int id = tbDSCauHoi.getSelectedIndex();
        int id = atoi(tbDSCauHoi.getSelectedValue()[0].c_str()) - 1;

        if (dsCauHoiThi[id]->getCanModifyFlag() == false)
        {
            showMessageDialog("Cau hoi thi da cho ra thi!\nBan khong the xoa cau hoi nay.", MessageBox_t::ERROR_MESSAGE);
            return;
        }

        NganHangDeThi->remove(dsCauHoiThi[id]->getId());

        dsCauHoiThi.remove(id);

        tbDSCauHoi.setData(getTableData());
        tbDSCauHoi.draw(*Graphics::getCurrentActiveGraphics());

//    tbDSCauHoi.setFirstSelected();

        capNhatSoCauHoiThi(true);

        generateInformationUIElement(*Graphics::getCurrentActiveGraphics(), true);

        showMessageDialog("Xoa cau hoi thi thanh cong!", MessageBox_t::INFORMATION_MESSAGE);
    }

    void xoaTatCaOption()
    {
        if (dsCauHoiThi.empty())
        {
            showMessageDialog("Danh sach cau hoi rong!", MessageBox_t::ERROR_MESSAGE);
            return;
        }

        string rep = showOptionDialog("Xac nhan xoa het danh sach cau hoi?", MessageBox_t::WARNING_MESSAGE, MessageBox_t::OK_CANCEL_OPTION);

        if (rep == "CANCEL")
            return;

        if (NganHangDeThi->phatHienCauHoiKhongTheXoa())
        {
            showMessageDialog("Mot so cau hoi da duoc cho ra thi!\nKhong the xoa het danh sach!", MessageBox_t::ERROR_MESSAGE);
            return;
        }

        NganHangDeThi->xoaDS();

        dsCauHoiThi.clear();

        tbDSCauHoi.clearAll();

        generateInformationUIElement(*Graphics::getCurrentActiveGraphics(), true);

        capNhatSoCauHoiThi(true);
    }

    void generateInformationUIElement(Graphics& g, bool drawFlag)
    {
        if (! tbDSCauHoi.empty())
        {
//      int id = tbDSCauHoi.getSelectedIndex();
            Vector<string> selectedRow = tbDSCauHoi.getSelectedValue();

            assert(!selectedRow.empty());

            int id = atoi(selectedRow[0].c_str()) - 1;

            cmbMaMon.setSelectedIndex(dsMon->search(dsCauHoiThi[id]->getMaMon()));

            taDe.setValue(dsCauHoiThi[id]->getCauHoi());

            Vector<string> luaChon = dsCauHoiThi[id]->getLuaChon();

            for (int i = 0; i < 4; ++i)
                taLuaChon[i].setValue(luaChon[i]);

            rbDapAn.setSelectedIndex(dsCauHoiThi[id]->getDapAn() - 'A');
        }
        else
        {
            cmbMaMon.setFirstSelected();

            taDe.clear();

            for (int i = 0; i < 4; ++i)
                taLuaChon[i].clear();

            rbDapAn.setFirstSelected();
        }

        if (drawFlag)
        {
            cmbMaMon.draw(g);

            taDe.draw(g);

            for (int i = 0; i < 4; ++i)
            {
                taLuaChon[i].draw(g);
            }

            rbDapAn.setFirstDrawFlag(true);
            rbDapAn.draw(g);
        }
    }

    //Bang ds cau hoi co 4 truong: STT, Ma mon cau hoi, Da cho ra thi hay chua
    Vector<Vector<string> > getTableData()
    {
        Vector<Vector<string> > res;

        if (!dsMon->empty() && !dsCauHoiThi.empty())
        {
            Vector<string> row(4);
            MonHoc *mh = NULL;
            for (int i = 0; i < dsCauHoiThi.size(); ++i)
            {
                row[0] = StringUtility::to_string(i + 1);
                row[1] = dsCauHoiThi[i]->getMaMon();

                mh = dsMon->getMonHoc(row[1]);
                assert(mh != NULL);

                row[2] = mh->getTenMon();
                row[3] = (dsCauHoiThi[i]->getCanModifyFlag() ? "" : "Da ra thi");

                res.push_back(row);
            }
        }

        return res;
    }

    Vector<string> getTableDataAtIndex(int id)
    {
        assert(!dsMon->empty());
        assert(!dsCauHoiThi.empty());
        assert(0 <= id && id < dsCauHoiThi.size());

        Vector<string> res;
        res.push_back(StringUtility::to_string(id + 1));
        res.push_back(dsCauHoiThi[id]->getMaMon());

        MonHoc *mh = dsMon->getMonHoc(res[1]);
        assert(mh != NULL);

        res.push_back(mh->getTenMon());
        res.push_back(dsCauHoiThi[id]->getCanModifyFlag() ? "" : "Da ra thi");

        return res;
    }

    friend class GVExecutiveForm;

public:
    CauHoiThiPanel()
        : Panel_t(BorderFactory::PLAIN, 0, 1,
    {
        170,44
    }),
    dsMon(DSMonHoc_t::getDSMonHocHienHanh()),
    NganHangDeThi(DSCauHoiThi_t::getDSCauHoiThiHienHanh())
    {

        setColor(0xf0);

        //Set element
        lbChucNang = Label_t(left + 3, top + 3, "Chon chuc nang:");
        lbChucNang.setColor(this->attribute);
        add(&lbChucNang);

        Vector<string> cmbChucNangOptions;
        cmbChucNangOptions.push_back("Them");
        cmbChucNangOptions.push_back("Cap Nhat");
        cmbChucNangOptions.push_back("Xoa");
        cmbChucNangOptions.push_back("Xoa Danh Sach");

        cmbChucNang = ComboBox_t(lbChucNang.getLeft() + lbChucNang.getCord().X + 3, lbChucNang.getTop() - 1, cmbChucNangOptions);
        add(&cmbChucNang);

        //Ma mon
        lbMaMon = Label_t(lbChucNang.getLeft(), lbChucNang.getTop() + 4, "Chon ma mon:");
        lbMaMon.setColor(this->attribute);
        add(&lbMaMon);

        cmbMaMon = ComboBox_t(lbMaMon.getLeft() + lbMaMon.getCord().X + 3, lbMaMon.getTop() - 1);
        add(&cmbMaMon);

        //De cau hoi
        lbCauHoi = Label_t(lbMaMon.getLeft(), lbMaMon.getTop() + 3, "Cau hoi:");
        lbCauHoi.setColor(this->attribute);
        add(&lbCauHoi);

        taDe = TextArea_t(lbCauHoi.getLeft() + 10, lbCauHoi.getTop(), 65, 8);
        add(&taDe);

        //Cac lua chon
        int nextTop = taDe.getTop() + taDe.getCord().Y + 3;
        int luaChonHgt = 2;

        for (int i = 0; i < 4; ++i)
        {
            lbLuaChon[i] = Label_t(taDe.getLeft() - 5, nextTop, string(1, 'A' + i) + ". ");
            lbLuaChon[i].setColor(this->attribute);
            add(&lbLuaChon[i]);

            taLuaChon[i] = TextArea_t(lbLuaChon[i].getLeft() + 5, nextTop - 1, 65, luaChonHgt);
            add(&taLuaChon[i]);

            nextTop += luaChonHgt + 3;
        }

        lbDapAn = Label_t(lbCauHoi.getLeft(), nextTop - 1, "Dap an:");
        lbDapAn.setColor(this->attribute);
        add(&lbDapAn);

        Vector<string> dapAnArr;
        dapAnArr.push_back("A");
        dapAnArr.push_back("B");
        dapAnArr.push_back("C");
        dapAnArr.push_back("D");

        rbDapAn = HorRadioBox_t( lbDapAn.getLeft() + lbDapAn.getCord().X + 3, lbDapAn.getTop(), dapAnArr);
        rbDapAn.setForceChecked();
        add(&rbDapAn);

        //Bang
        lbDSCauHoi = Label_t(BorderFactory::DOUBLE, 105, lbChucNang.getTop(), 10, "DANH SACH CAC CAU HOI THI");
        lbDSCauHoi.setColor(this->attribute);
        add(&lbDSCauHoi);

        lbSoCauHoi = Label_t(BorderFactory::SINGLE, lbDSCauHoi.getLeft() - 15, lbDSCauHoi.getTop() + 3, "Tong so cau hoi: " + StringUtility::to_string(dsCauHoiThi.size()));
        lbSoCauHoi.setColor(this->attribute);
        add(&lbSoCauHoi);

        Vector<string> tableHeader;
        tableHeader.push_back("STT");
        tableHeader.push_back("Ma Mon");
        tableHeader.push_back("Ten Mon");
        tableHeader.push_back("Ghi Chu");

        tbDSCauHoi = Table_t(lbSoCauHoi.getLeft(), lbSoCauHoi.getTop() + 5, 25, tableHeader);

        Vector<size_t> szTable;
        szTable.push_back(10);
        szTable.push_back(15);
        szTable.push_back(25);
        szTable.push_back(20);

        tbDSCauHoi.setFixedWidths(szTable);
        tbDSCauHoi.setFindOptionColumnIndex(0);
        add(&tbDSCauHoi);

        btnThucThi = Button_t(taDe.getLeft() + taDe.getCord().X - 8, lbDapAn.getTop() + 1, "Thuc thi");
        add(&btnThucThi);
    }

    void reloadData()
    {
        dsCauHoiThi.clear();

        cmbMaMon.setData(dsMon->getDSMaMon());

        if (! dsMon->empty())
        {
            dsCauHoiThi = NganHangDeThi->layHetDanhSachCauHoi();
        }

        capNhatSoCauHoiThi(false);

        tbDSCauHoi.setData(getTableData());

        generateInformationUIElement(*Graphics::getCurrentActiveGraphics(), false);
    }

    void draw(Graphics& g)
    {
        reloadData();

        Panel_t::draw(g);
    }

    bool mouseClick(int l, int t, Graphics& g)
    {
        int preCmbChucNangSelectedIndex = cmbChucNang.getSelectedIndex();

        if (! Panel_t::mouseClick(l, t, g))
            return false;

        if (cmbChucNang.isFocus())
        {
            int newCmbChucNangSelectedIndex = cmbChucNang.getSelectedIndex();

            if (preCmbChucNangSelectedIndex != newCmbChucNangSelectedIndex)
            {
                string option = cmbChucNang.getSelectedValue();
                if (option == "Them")
                {
                    taDe.clear();
                    taDe.setAbleFocus(true);

                    for (int i = 0; i < 4; ++i)
                    {
                        taLuaChon[i].clear();
                        taLuaChon[i].setAbleFocus(true);
                    }

                    rbDapAn.setFirstSelected();
                    rbDapAn.setAbleFocus(true);
                }
                else if (option == "Cap Nhat")
                {
                    generateInformationUIElement(g, true);
                    taDe.setAbleFocus(true);

                    for (int i = 0; i < 4; ++i)
                    {
                        taLuaChon[i].setAbleFocus(true);
                    }

                    rbDapAn.setAbleFocus(true);
                }
                else
                {
                    generateInformationUIElement(g, true);
                    taDe.setAbleFocus(false);

                    for (int i = 0; i < 4; ++i)
                    {
                        taLuaChon[i].setAbleFocus(false);
                    }

                    rbDapAn.setAbleFocus(false);
                }
                taDe.draw(g);
                for (int i = 0; i < 4; ++i)
                {
                    taLuaChon[i].draw(g);
                }
                rbDapAn.draw(g);
            }
        }

        if (tbDSCauHoi.isFocus())
        {
            generateInformationUIElement(g, true);
            return true;
        }

        if (btnThucThi.isFocus())
        {
            string option = cmbChucNang.getSelectedValue();
            if (option == "Them")
            {
                themOption();
            }
            else if (option == "Cap Nhat")
            {
                capNhatOption();
            }
            else if (option == "Xoa")
            {
                xoaOption();
            }
            else if (option == "Xoa Danh Sach")
            {
                xoaTatCaOption();
            }
        }

        return true;
    }

    void keyPress(int keyCode, char ch, Graphics& g)
    {
        if (cmbChucNang.isFocus())
        {
            string option = cmbChucNang.getSelectedValue();
            if (option == "Them")
            {
                taDe.clear();
                taDe.setAbleFocus(true);

                for (int i = 0; i < 4; ++i)
                {
                    taLuaChon[i].clear();
                    taLuaChon[i].setAbleFocus(true);
                }

                rbDapAn.setFirstSelected();
                rbDapAn.setAbleFocus(true);
            }
            else if (option == "Cap Nhat")
            {
                generateInformationUIElement(g, true);
                taDe.setAbleFocus(true);

                for (int i = 0; i < 4; ++i)
                {
                    taLuaChon[i].setAbleFocus(true);
                }

                rbDapAn.setAbleFocus(true);
            }
            else
            {
                generateInformationUIElement(g, true);
                taDe.setAbleFocus(false);

                for (int i = 0; i < 4; ++i)
                {
                    taLuaChon[i].setAbleFocus(false);
                }

                rbDapAn.setAbleFocus(false);
            }
            taDe.draw(g);
            for (int i = 0; i < 4; ++i)
            {
                taLuaChon[i].draw(g);
            }
            rbDapAn.draw(g);
        }
    }
};


#endif // CAUHOITHIPANEL_H_INCLUDED
