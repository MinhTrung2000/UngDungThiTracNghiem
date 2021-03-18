#ifndef DIEMTHILOPPANEL_H_INCLUDED
#define DIEMTHILOPPANEL_H_INCLUDED

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

class DiemThiLopPanel : public Panel_t
{
private:
    //Danh sach lop
    DSLop_t *dsLop;

    //Danh sach sinh vien cua lop duoc chon
    DSSinhVien_t *dssvPtr;
    Vector<DiemThi *> dsDiemThi;

    //Danh sach mon hoc
    DSMonHoc_t *dsMon;

    //Chon lop
    Label_t lbMaLop;
    Label_t lbTenLop;
    ComboBox_t cmbMaLop;

    //Chon mon hoc
    Label_t lbMaMon;
    Label_t lbTenMon;
    ComboBox_t cmbMaMon;

    //Bang diem mon hoc cua lop
    Label_t lbDiemLop;
    //6 Truong: Masv, ho, ten, gioi tinh, diem, ghi chu
    Table_t tbDiemLop;

    //Thong bao thong ke
    Label_t lbThongKe;

    //Khi ma lop, ma mon thay doi
    void capNhatThongKe(bool drawFlag)
    {
        lbThongKe.clearValue();

        //Khong ton tai lop nao
        if (dssvPtr == NULL || dsMon->empty())
            return;

        //Lay so luong sinh vien
        int soSV = dssvPtr->getSoSinhVien();

        string soSVStr = StringUtility::to_string(soSV);

        //So sv da thi
        int soSVDaThi = 0;

        for (int i = 0; i < dsDiemThi.size(); ++i)
        {
            if (dsDiemThi[i] != NULL)
                ++soSVDaThi;
        }

        stringstream line;
        line << "BANG THONG KE:\n";
        line << "Tong so sinh vien: " << StringUtility::to_string(soSV);

        if (soSV > 0)
        {
            line << "\nSo sinh vien da thi: " << StringUtility::to_string(soSVDaThi) << " / " << soSVStr;
            line << "\nSo sinh vien chua thi: " << StringUtility::to_string(soSV - soSVDaThi) << " / " << soSVStr;
        }

        lbThongKe.setValue(line.str());

        if (drawFlag)
        {
            lbThongKe.draw(*Graphics::getCurrentActiveGraphics());
        }
    }

    //Khi ma mon chon thay doi
    void capNhatDSDiemThi(bool drawFlag)
    {
        dsDiemThi.clear();
        lbTenMon.clearValue();

        if (dssvPtr != NULL)
        {
            //Neu khong co mon hoc nao, dsDiemThi se rong
            if (! dsMon->empty())
            {
                string newMaMon = cmbMaMon.getSelectedValue();
                lbTenMon.setValue("Ten mon: " + dsMon->getMonHoc(newMaMon)->getTenMon());

                dssvPtr->layDSDiemThiTheoMon(newMaMon, dsDiemThi);
            }
        }

        tbDiemLop.setData(getTableData());

        if (drawFlag)
        {
            lbTenMon.draw(*Graphics::getCurrentActiveGraphics());
            tbDiemLop.draw(*Graphics::getCurrentActiveGraphics());
        }

        capNhatThongKe(drawFlag);
    }

    //Khi ma lop chon thay doi
    void capNhatConTroDSSinhVien(bool drawFlag)
    {
        dssvPtr = NULL;
        lbTenLop.clearValue();

        if (! dsLop->empty())
        {
            lbTenLop.setValue("Ten lop: " + dsLop->getLop(cmbMaLop.getSelectedValue())->getTenLop());

            int newLopChonId = cmbMaLop.getSelectedIndex();
            dssvPtr = dsLop->getLop(newLopChonId)->getDanhSachSinhVien();
        }

        if (drawFlag)
        {
            lbTenLop.draw(*Graphics::getCurrentActiveGraphics());
        }

        capNhatDSDiemThi(drawFlag);
    }

    //Lay du lieu bang diem cua lop
    Vector<Vector<string> > getTableData()
    {
        Vector<Vector<string> > res;

        if (dssvPtr != NULL && ! dssvPtr->empty() && ! dsMon->empty())
        {
            int soSV = dssvPtr->getSoSinhVien();

            res.resize(soSV);

            SinhVien *sv = NULL;
            DiemThi *dt = NULL;

            SLLNode<SinhVien> *dsHeadPtr = dssvPtr->begin();

            for (int i = 0; i < soSV; ++i)
            {
                assert(dsHeadPtr != NULL);

                sv = &(dsHeadPtr->key);

                res[i].push_back(StringUtility::to_string(i + 1));
                res[i].push_back(sv->getMaSV());
                res[i].push_back(sv->getHo());
                res[i].push_back(sv->getTen());
                res[i].push_back(sv->getPhai());

                dt = dsDiemThi[i];

                if (dt == NULL)
                {
                    res[i].push_back("");
                    res[i].push_back("Chua thi");
                }
                else
                {
                    res[i].push_back(StringUtility::to_string(dt->getDiem()));
                    res[i].push_back("");
                }

                dsHeadPtr = dsHeadPtr->next;
            }
        }

        return res;
    }

    friend class GVExecutiveForm;

public:
    DiemThiLopPanel()
        : Panel_t(BorderFactory::PLAIN, 0, 1,
    {
        170,44
    }),
    dsLop(DSLop_t::getDSLopHienHanh()),
    dsMon(DSMonHoc_t::getDSMonHocHienHanh()), dssvPtr(NULL)
    {

        setColor(0xf0);

        //Ma lop
        lbMaLop = Label_t(left + 3, top + 7, "Chon ma lop:");
        lbMaLop.setColor(this->attribute);
        add(&lbMaLop);

        cmbMaLop = ComboBox_t(lbMaLop.getLeft() + lbMaLop.getCord().X + 10, lbMaLop.getTop() - 1, 3);
        add(&cmbMaLop);

        lbTenLop = Label_t(BorderFactory::SINGLE, lbMaLop.getLeft() - 1, lbMaLop.getTop() + 3, "");
        lbTenLop.setColor(this->attribute);
        lbTenLop.setMaxWidth(50);
        add(&lbTenLop);

        //Ma mon
        lbMaMon = Label_t(lbMaLop.getLeft(), lbTenLop.getTop() + 5, "Chon ma mon hoc: ");
        lbMaMon.setColor(this->attribute);
        add(&lbMaMon);

        cmbMaMon = ComboBox_t(cmbMaLop.getLeft(), lbMaMon.getTop() - 1, 3);
        add(&cmbMaMon);

        lbTenMon = Label_t(BorderFactory::SINGLE, lbMaMon.getLeft() - 1, lbMaMon.getTop() + 3, "");
        lbTenMon.setColor(this->attribute);
        lbTenMon.setMaxWidth(50);
        add(&lbTenMon);

        lbThongKe = Label_t(BorderFactory::SINGLE, lbTenMon.getLeft(), lbTenMon.getTop() + 5, "");
        lbThongKe.setColor(this->attribute);
        add(&lbThongKe);

        lbDiemLop = Label_t(BorderFactory::DOUBLE, 85, lbMaLop.getTop() - 5, 15, "DIEM MON HOC CUA LOP");
        lbDiemLop.setColor(this->attribute);
        add(&lbDiemLop);

        Vector<string> tableHeaders;
        tableHeaders.push_back("STT");
        tableHeaders.push_back("MSSV");
        tableHeaders.push_back("Ho");
        tableHeaders.push_back("Ten");
        tableHeaders.push_back("Gioi tinh");
        tableHeaders.push_back("Diem");
        tableHeaders.push_back("Ghi chu");

        tbDiemLop = Table_t(55, lbDiemLop.getTop() + 5, 30, tableHeaders);

        Vector<size_t> szTable;
        szTable.push_back(10);
        szTable.push_back(15);
        szTable.push_back(25);
        szTable.push_back(15);
        szTable.push_back(15);
        szTable.push_back(10);
        szTable.push_back(12);
//
        tbDiemLop.setFixedWidths(szTable);
        tbDiemLop.setFindOptionColumnIndex(1);
        add(&tbDiemLop);
    }

    void reloadData()
    {
        //cap nhat ds ma lop, ds ma mon
        cmbMaLop.setData(dsLop->getDSMaLop());
        cmbMaMon.setData(dsMon->getDSMaMon());

        capNhatConTroDSSinhVien(false);
    }

    void draw(Graphics& g)
    {
        reloadData();

        Panel_t::draw(g);
    }

    bool mouseClick(int l, int t, Graphics& g)
    {
        int preCmbLopSelectedIndex = cmbMaLop.getSelectedIndex();

        int preCmbMonSelectedIndex = cmbMaMon.getSelectedIndex();

        if (! Panel_t::mouseClick(l, t, g))
            return false;

        if (cmbMaLop.isFocus())
        {
            int newCmbLopSelectedIndex = cmbMaLop.getSelectedIndex();

            if (preCmbLopSelectedIndex != newCmbLopSelectedIndex)
            {
                capNhatConTroDSSinhVien(true);

                return true;
            }
        }

        if (cmbMaMon.isFocus())
        {
            int newCmbMonSelectedIndex = cmbMaMon.getSelectedIndex();

            if (preCmbMonSelectedIndex != newCmbMonSelectedIndex)
            {
                capNhatDSDiemThi(true);

                return true;
            }
        }

        return true;
    }

    void keyPress(int keyCode, char ch, Graphics& g)
    {
        if (cmbMaLop.isFocus())
        {
            if (cmbMaLop.getOpenFlag() == false)
            {
                capNhatConTroDSSinhVien(true);
            }
        }

        if (cmbMaMon.isFocus())
        {
            if (cmbMaMon.getOpenFlag() == false)
            {
                capNhatDSDiemThi(true);
            }
        }
    }
};


#endif // DIEMTHILOPPANEL_H_INCLUDED
