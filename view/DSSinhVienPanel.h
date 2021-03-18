#ifndef DSSINHVIENPANEL_H_INCLUDED
#define DSSINHVIENPANEL_H_INCLUDED

#include <direct.h>

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

class DSSinhVienPanel : public Panel_t
{
private:
    //Danh sach lop
    DSLop_t *dsLop;

    //Danh sach sinh vien duoc chon tu lop
    //dssv = NULL neu khong co lop nao
    DSSinhVien_t *dssv;

    //Chuc nang
    Label_t lbChucNang;
    ComboBox_t cmbChucNang;
    Button_t btnThucThi;

    //Nhan thong tin sv
    Label_t lbThongTinSV;

    //masv
    Label_t lbMaSV;
    TextField_t tfMaSV;

    //ho sv
    Label_t lbHoSV;
    TextField_t tfHoSV;

    //ten sv
    Label_t lbTenSV;
    TextField_t tfTenSV;

    //phai sv
    Label_t lbPhaiSV;
    ComboBox_t cmbPhaiSV;

    //mat khau sv
    Label_t lbMatKhauSV;
    TextField_t tfMatKhauSV;

    //Chon lop
    Label_t lbChonLop;
    ComboBox_t cmbChonLop;

    //Nhan thong bao so sinh vien trong ds
    Label_t lbThongBaoSoSinhVien;

    //Bang ds sinh vien cua lop
    Label_t lbDSSV;
    Table_t tbDSSV;

    //Ghi chu
    Label_t lbGhiChu;

    int preSelectRowByClick;

    void capNhatTongSoSinhVien(bool drawFlag)
    {
        lbThongBaoSoSinhVien.clearValue();
        if (dssv == NULL)
            return;
        lbThongBaoSoSinhVien.setValue("So sinh vien: " + StringUtility::to_string(dssv->getSoSinhVien()));
        if (drawFlag)
        {
            lbThongBaoSoSinhVien.draw(*Graphics::getCurrentActiveGraphics());
        }
    }

    //Chon Them
    void themOption()
    {
        //ds lop rong
        if (dsLop->empty())
        {
            showMessageDialog("Danh sach lop hien tai rong!\n Ban can tao them lop moi!", MessageBox_t::INFORMATION_MESSAGE);
            return;
        }

        string maSV = tfMaSV.getValue();
        string hoSV = tfHoSV.getValue();
        string tenSV = tfTenSV.getValue();
        string phaiSV = cmbPhaiSV.getSelectedValue();
        string matKhauSV = tfMatKhauSV.getValue();

        StringUtility::Trim(maSV);
        StringUtility::UpperCaseAllLetter(maSV);
        StringUtility::Trim(hoSV);
        StringUtility::UpperCaseFirstLetterPerWord(hoSV);
        StringUtility::Trim(tenSV);
        StringUtility::UpperCaseFirstLetterPerWord(tenSV);
        StringUtility::Trim(matKhauSV);

        if (maSV.empty())
        {
            showMessageDialog("Ma sinh vien khong duoc de trong!", MessageBox_t::ERROR_MESSAGE);
            Rect_t::setFocus(tfMaSV);
            return;
        }

        //kiem tra ma sinh vien co chua ki tu khac chu va so
        if (StringUtility::HavingPunctChar(maSV))
        {
            showMessageDialog("Ma sinh vien chi bao gom chu cai va so!", MessageBox_t::ERROR_MESSAGE);
            Rect_t::setFocus(tfMaSV);
            return;
        }

        //kiem tra ma sinh vien co khoang trang
        if (! StringUtility::IsAlNum(maSV))
        {
            showMessageDialog("Ma sinh vien khong cho phep co khoang trang!", MessageBox_t::ERROR_MESSAGE);
            Rect_t::setFocus(tfMaSV);
            return;
        }

        if (hoSV.empty())
        {
            showMessageDialog("Ho sinh vien khong duoc de trong!", MessageBox_t::ERROR_MESSAGE);
            Rect_t::setFocus(tfHoSV);
            return;
        }

        //kiem tra ho sinh vien co chua ki tu khac chu
        if (! StringUtility::IsAlphaSpace(hoSV))
        {
            showMessageDialog("Ho sinh vien chi bao gom chu cai va khoang trang!", MessageBox_t::ERROR_MESSAGE);
            Rect_t::setFocus(tfHoSV);
            return;
        }

        if (tenSV.empty())
        {
            showMessageDialog("Ten sinh vien khong duoc de trong!", MessageBox_t::ERROR_MESSAGE);
            Rect_t::setFocus(tfTenSV);
            return;
        }

        //kiem tra ten sinh vien co chua ki tu khac chu
        if (! StringUtility::IsAlpha(tenSV))
        {
            showMessageDialog("Ten sinh vien chi bao gom chu cai!", MessageBox_t::ERROR_MESSAGE);
            Rect_t::setFocus(tfTenSV);
            return;
        }

        if (matKhauSV.empty())
        {
            showMessageDialog("Mat khau danh nhap cua sinh vien khong duoc de trong!", MessageBox_t::ERROR_MESSAGE);
            Rect_t::setFocus(tfMatKhauSV);
            return;
        }

        //kiem tra mat khau sinh vien co khoang trang
        if (StringUtility::HavingSpace(matKhauSV))
        {
            showMessageDialog("Mat khau danh nhap cua sinh vien khong cho phep co khoang trang!", MessageBox_t::ERROR_MESSAGE);
            Rect_t::setFocus(tfMatKhauSV);
            return;
        }

        if (dsLop->getSinhVien(maSV) != NULL)
        {
            //thong bao trung ma sinh vien
            showMessageDialog("Ma sinh vien bi trung lap.\nKhong the them sinh vien nay!", MessageBox_t::ERROR_MESSAGE);
            Rect_t::setFocus(tfMaSV);
            return;
        }

        string maLop = cmbChonLop.getSelectedValue();
        assert(maLop != "");
        SinhVien *newSV = new SinhVien(maSV, hoSV, tenSV, phaiSV, matKhauSV, maLop);

        dssv->push_back(*newSV);

        //cap nhat table
        tbDSSV.insertRow(getTableDataAtIndex(dssv->getSoSinhVien() - 1, newSV));

        //cap nhat so sinh vien
        capNhatTongSoSinhVien(true);

        generateInformationUIElement(newSV, true);

        showMessageDialog("Them sinh vien co ma so " + maSV + " thanh cong!", MessageBox_t::INFORMATION_MESSAGE);
    }

    //Chon nut Cap nhat
    void capNhatOption()
    {
        //ds lop rong
        if (dsLop->empty())
        {
            showMessageDialog("Danh sach lop hien tai rong!\n Ban can tao lop moi!", MessageBox_t::INFORMATION_MESSAGE);
            return;
        }

        if (dssv->empty())
        {
            showMessageDialog("Danh sach sinh vien rong!", MessageBox_t::ERROR_MESSAGE);
            return;
        }

        //Lay ma sinh vien tu tfMaSV duoc generate
        string maSVCapNhat = tfMaSV.getValue();
        string newHoSV = tfHoSV.getValue();
        string newTenSV = tfTenSV.getValue();
        string newPhaiSV = cmbPhaiSV.getSelectedValue();
        string newMatKhauSV = tfMatKhauSV.getValue();

        StringUtility::Trim(maSVCapNhat);
        StringUtility::UpperCaseAllLetter(maSVCapNhat);
        StringUtility::Trim(newHoSV);
        StringUtility::UpperCaseFirstLetterPerWord(newHoSV);
        StringUtility::Trim(newTenSV);
        StringUtility::UpperCaseFirstLetterPerWord(newTenSV);
        StringUtility::Trim(newMatKhauSV);

        if (newHoSV.empty())
        {
            showMessageDialog("Ho sinh vien khong duoc de trong!", MessageBox_t::ERROR_MESSAGE);
            Rect_t::setFocus(tfHoSV);
            return;
        }

        //kiem tra ho sinh vien co chua ki tu khac chu
        if (! StringUtility::IsAlphaSpace(newHoSV))
        {
            showMessageDialog("Ho sinh vien chi bao gom chu cai va khoang trang!", MessageBox_t::ERROR_MESSAGE);
            Rect_t::setFocus(tfHoSV);
            return;
        }

        if (newTenSV.empty())
        {
            showMessageDialog("Ten sinh vien khong duoc de trong!", MessageBox_t::ERROR_MESSAGE);
            Rect_t::setFocus(tfTenSV);
            return;
        }

        //kiem tra ten sinh vien co chua ki tu khac chu
        if (! StringUtility::IsAlpha(newTenSV))
        {
            showMessageDialog("Ten sinh vien chi bao gom chu cai!", MessageBox_t::ERROR_MESSAGE);
            Rect_t::setFocus(tfTenSV);
            return;
        }

        if (newMatKhauSV.empty())
        {
            showMessageDialog("Mat khau danh nhap cua sinh vien khong duoc de trong!", MessageBox_t::ERROR_MESSAGE);
            Rect_t::setFocus(tfMatKhauSV);
            return;
        }

        //kiem tra mat khau sinh vien co khoang trang
        if (StringUtility::HavingSpace(newMatKhauSV))
        {
            showMessageDialog("Mat khau danh nhap cua sinh vien khong cho phep co khoang trang!", MessageBox_t::ERROR_MESSAGE);
            Rect_t::setFocus(tfMatKhauSV);
            return;
        }

        SinhVien *sv = dssv->getSinhVien(maSVCapNhat);

        assert(sv != NULL);

        sv->setHo(newHoSV);
        sv->setTen(newTenSV);
        sv->setPhai(newPhaiSV);
        sv->setMatKhau(newMatKhauSV);

        tbDSSV.updateRowByIndex(preSelectRowByClick, getTableDataAtIndex(preSelectRowByClick, sv));

        generateInformationUIElement(sv, true);

        showMessageDialog("Cap nhat thong tin sinh vien co ma " + maSVCapNhat + " thanh cong!", MessageBox_t::INFORMATION_MESSAGE);
    }

    //Chon nut Xoa
    void xoaOption()
    {
        //ds lop rong
        if (dsLop->empty())
        {
            showMessageDialog("Danh sach lop hien tai rong!\n Ban can tao lop moi!", MessageBox_t::INFORMATION_MESSAGE);
            return;
        }

        //Lay ma sinh vien duoc generate
        if (dssv->empty())
        {
            showMessageDialog("Danh sach sinh vien rong!", MessageBox_t::ERROR_MESSAGE);
            return;
        }

        string maSVXoa = tfMaSV.getValue();

        string rep = showOptionDialog("Xac nhan xoa sinh vien co ma sinh vien " + maSVXoa + "?", MessageBox_t::QUESTION_MESSAGE, MessageBox_t::OK_CANCEL_OPTION);

        if (rep == "CANCEL")
            return;

        dssv->remove(maSVXoa);

        tbDSSV.setData(getTableData());
        tbDSSV.draw(*Graphics::getCurrentActiveGraphics());

        capNhatTongSoSinhVien(true);

        generateInformationUIElement(true);
    }

    //Chon xoa tat ca
    void xoaTatCaOption()
    {
        //ds lop rong
        if (dsLop->empty())
        {
            showMessageDialog("Danh sach lop hien tai rong!\n Ban can tao lop moi!", MessageBox_t::INFORMATION_MESSAGE);
            return;
        }

        //Lay id sinh vien can xoa tai table
        if (dssv->empty())
        {
            showMessageDialog("Danh sach sinh vien rong!", MessageBox_t::ERROR_MESSAGE);
            return;
        }

        string rep = showOptionDialog("Xac nhan xoa het danh sach sinh vien?", MessageBox_t::QUESTION_MESSAGE, MessageBox_t::OK_CANCEL_OPTION);

        if (rep == "CANCEL")
            return;

        dssv->xoaDS();

        tbDSSV.clearAll();

        capNhatTongSoSinhVien(true);

        generateInformationUIElement(true);
    }

    //hien thi thong tin sinh vien khi chon tren bang
    void generateInformationUIElement(SinhVien *sv, bool drawFlag)
    {
        if (! tbDSSV.empty())
        {
            preSelectRowByClick = tbDSSV.getSelectedIndex();

//      SinhVien *sv = dssv->getSinhVien(tbDSSV.getSelectedValue()[1]);

            assert(sv != NULL);

            tfMaSV.setValue(sv->getMaSV());
            tfHoSV.setValue(sv->getHo());
            tfTenSV.setValue(sv->getTen());
            cmbPhaiSV.setSelectedIndex(sv->getPhai() == "Nam" ? 0 : 1);
            tfMatKhauSV.setValue(sv->getMatKhau());
        }
        else
        {
            preSelectRowByClick = -1;

            tfMaSV.clear();
            tfHoSV.clear();
            tfTenSV.clear();
            cmbPhaiSV.setSelectedIndex(0);
            tfMatKhauSV.clear();
        }

        if (drawFlag)
        {
            Graphics *g = Graphics::getCurrentActiveGraphics();
            tfMaSV.draw(*g);
            tfHoSV.draw(*g);
            tfTenSV.draw(*g);
            cmbPhaiSV.draw(*g);
            tfMatKhauSV.draw(*g);
        }
    }

    void generateInformationUIElement(bool drawFlag)
    {
        if (! tbDSSV.empty())
        {
            preSelectRowByClick = tbDSSV.getSelectedIndex();

            SinhVien *sv = dssv->getSinhVien(tbDSSV.getSelectedValue()[1]);

            assert(sv != NULL);

            tfMaSV.setValue(sv->getMaSV());
            tfHoSV.setValue(sv->getHo());
            tfTenSV.setValue(sv->getTen());
            cmbPhaiSV.setSelectedIndex(sv->getPhai() == "Nam" ? 0 : 1);
            tfMatKhauSV.setValue(sv->getMatKhau());
        }
        else
        {
            preSelectRowByClick = -1;

            tfMaSV.clear();
            tfHoSV.clear();
            tfTenSV.clear();
            cmbPhaiSV.setSelectedIndex(0);
            tfMatKhauSV.clear();
        }

        if (drawFlag)
        {
            Graphics *g = Graphics::getCurrentActiveGraphics();
            tfMaSV.draw(*g);
            tfHoSV.draw(*g);
            tfTenSV.draw(*g);
            cmbPhaiSV.draw(*g);
            tfMatKhauSV.draw(*g);
        }
    }

    /*
      Test:
      1. ds lop khac rong: OK
      2. ds lop rong: ?
    */
    void capNhatConTroDSSinhVien(bool drawFlag)
    {
        dssv = NULL;

        int id = cmbChonLop.getSelectedIndex();

        if (id != -1)
        {
            dssv = dsLop->getLop(id)->getDanhSachSinhVien();
        }

        tbDSSV.setData(getTableData());

        if (drawFlag)
        {
            tbDSSV.draw(*Graphics::getCurrentActiveGraphics());
        }

        capNhatTongSoSinhVien(drawFlag);

        generateInformationUIElement(drawFlag);
    }

    void capNhatChonChucNang()
    {
        string option = cmbChucNang.getSelectedValue();
        if (option == "Them")
        {
            tfMaSV.clear();
            tfHoSV.clear();
            tfTenSV.clear();
            cmbPhaiSV.setFirstSelected();
            tfMatKhauSV.clear();

            tfMaSV.setAbleFocus(true);
            tfHoSV.setAbleFocus(true);
            tfTenSV.setAbleFocus(true);
            cmbPhaiSV.setAbleFocus(true);
            tfMatKhauSV.setAbleFocus(true);
        }
        else if (option == "Cap Nhat")
        {
            generateInformationUIElement(true);
            tfMaSV.setAbleFocus(false);
            tfHoSV.setAbleFocus(true);
            tfTenSV.setAbleFocus(true);
            cmbPhaiSV.setAbleFocus(true);
            tfMatKhauSV.setAbleFocus(true);
        }
        else
        {
            generateInformationUIElement(true);
            tfMaSV.setAbleFocus(false);
            tfHoSV.setAbleFocus(false);
            tfTenSV.setAbleFocus(false);
            cmbPhaiSV.setAbleFocus(false);
            tfMatKhauSV.setAbleFocus(false);
        }
        Graphics *g = Graphics::getCurrentActiveGraphics();
        tfMaSV.draw(*g);
        tfHoSV.draw(*g);
        tfTenSV.draw(*g);
        cmbPhaiSV.draw(*g);
        tfMatKhauSV.draw(*g);
    }

    //Lay du lieu bang sinh vien
    Vector<Vector<string> > getTableData()
    {
        Vector<Vector<string> > res;

        if (dssv != NULL)
        {
            int soSV = dssv->getSoSinhVien();
            res.resize(soSV);

            SLLNode<SinhVien> *dssvPtr = dssv->begin();

            for (int i = 0; i < soSV; ++i)
            {
                SinhVien *sv = &(dssvPtr->key);
                res[i].push_back(StringUtility::to_string(i + 1));
                res[i].push_back(sv->getMaSV());
                res[i].push_back(sv->getHo());
                res[i].push_back(sv->getTen());
                res[i].push_back(sv->getPhai());
                res[i].push_back(sv->getMatKhau());

                dssvPtr = dssvPtr->next;
            }
        }

        return res;
    }

    Vector<string> getTableDataAtIndex(int id, SinhVien *sv)
    {
        assert(sv != NULL);

        Vector<string> res;

        if (dssv != NULL)
        {
            assert(0 <= id && id < dssv->getSoSinhVien());

            res.push_back(StringUtility::to_string(id + 1));
            res.push_back(sv->getMaSV());
            res.push_back(sv->getHo());
            res.push_back(sv->getTen());
            res.push_back(sv->getPhai());
            res.push_back(sv->getMatKhau());
        }

        return res;
    }

    friend class GVExecutiveForm;

public:
    DSSinhVienPanel()
        : Panel_t(BorderFactory::PLAIN, 0, 1,
    {
        170,44
    }),
    dsLop(DSLop_t::getDSLopHienHanh())
    {

        setColor(0xf0);

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

        //ma sv
        lbMaSV = Label_t(lbChucNang.getLeft(), lbChucNang.getTop() + 5, "Ma so:");
        lbMaSV.setColor(this->attribute);
        add(&lbMaSV);

        tfMaSV = TextField_t(BorderFactory::SINGLE, lbMaSV.getLeft() + 13, lbMaSV.getTop() - 1, 40, 100);
        add(&tfMaSV);

        //ho sv
        lbHoSV = Label_t(lbMaSV.getLeft(), lbMaSV.getTop() + 5, "Ho:");
        lbHoSV.setColor(this->attribute);
        add(&lbHoSV);

        tfHoSV = TextField_t(BorderFactory::SINGLE, tfMaSV.getLeft(), lbHoSV.getTop() - 1, 40, 100);
        add(&tfHoSV);

        //ten sv
        lbTenSV = Label_t(lbHoSV.getLeft(), lbHoSV.getTop() + 5, "Ten:");
        lbTenSV.setColor(this->attribute);
        add(&lbTenSV);

        tfTenSV = TextField_t(BorderFactory::SINGLE, tfHoSV.getLeft(), lbTenSV.getTop() - 1, 40, 100);
        add(&tfTenSV);

        //Phai
        lbPhaiSV = Label_t(lbTenSV.getLeft(), lbTenSV.getTop() + 5, "Gioi tinh:");
        lbPhaiSV.setColor(this->attribute);
        add(&lbPhaiSV);

        Vector<string> gioiTinh;
        gioiTinh.push_back("Nam");
        gioiTinh.push_back("Nu");

        cmbPhaiSV = ComboBox_t(tfTenSV.getLeft(), lbPhaiSV.getTop() - 1, gioiTinh);
        add(&cmbPhaiSV);

        //Mat khau sv
        lbMatKhauSV = Label_t(lbPhaiSV.getLeft(), lbPhaiSV.getTop() + 5, "Mat khau:");
        lbMatKhauSV.setColor(this->attribute);
        add(&lbMatKhauSV);

        tfMatKhauSV = TextField_t(BorderFactory::SINGLE, tfMaSV.getLeft(), lbMatKhauSV.getTop() - 1, 40, 100);
        add(&tfMatKhauSV);

        //
        btnThucThi = Button_t(tfMaSV.getLeft() + tfMaSV.getCord().X - 10, tfMatKhauSV.getTop() + 5, "Thuc Thi");
        add(&btnThucThi);

        lbGhiChu = Label_t(BorderFactory::SINGLE, lbChucNang.getLeft() - 1, btnThucThi.getTop() + 3,
                           "Ghi chu:\nMa so sinh vien chi bao gom chu cai va so.\nMat khau khong bao gom khoang trang.");
        lbGhiChu.setColor(this->attribute);
        add(&lbGhiChu);

        lbDSSV = Label_t(BorderFactory::DOUBLE, 95, 4, 15, "DANH SACH SINH VIEN");
        lbDSSV.setColor(this->attribute);
        add(&lbDSSV);

        //chon lop
        lbChonLop = Label_t(70, 9, "Chon lop:");
        lbChonLop.setColor(this->attribute);
        add(&lbChonLop);

        cmbChonLop = ComboBox_t(lbChonLop.getLeft() + lbChonLop.getCord().X + 3, lbChonLop.getTop() - 1);
        add(&cmbChonLop);

        lbThongBaoSoSinhVien = Label_t(BorderFactory::SINGLE, cmbChonLop.getLeft() + cmbChonLop.getCord().X + 50, lbChonLop.getTop() - 1, "Tong so sinh vien: ");
        lbThongBaoSoSinhVien.setColor(this->attribute);
        add(&lbThongBaoSoSinhVien);

        Vector<string> DSMonTableHeaders;
        DSMonTableHeaders.push_back("STT");
        DSMonTableHeaders.push_back("Ma so");
        DSMonTableHeaders.push_back("Ho");
        DSMonTableHeaders.push_back("Ten");
        DSMonTableHeaders.push_back("Gioi Tinh");
        DSMonTableHeaders.push_back("Mat Khau");

        tbDSSV = Table_t(lbChonLop.getLeft(), lbChonLop.getTop() + 3, 28, DSMonTableHeaders);

        Vector<size_t> szTable;
        szTable.push_back(10);
        szTable.push_back(15);
        szTable.push_back(25);
        szTable.push_back(15);
        szTable.push_back(12);
        szTable.push_back(12);

        tbDSSV.setFixedWidths(szTable);
        //find is set on ma sinh vien key
        tbDSSV.setFindOptionColumnIndex(1);

        add(&tbDSSV);
    }

    void reloadData()
    {
        cmbChucNang.setFirstSelected();

        cmbChonLop.setData(dsLop->getDSMaLop());

        tfMaSV.clear();
        tfHoSV.clear();
        tfTenSV.clear();
        cmbPhaiSV.setFirstSelected();
        tfMatKhauSV.clear();

        tfMaSV.setAbleFocus(true);
        tfHoSV.setAbleFocus(true);
        tfTenSV.setAbleFocus(true);
        cmbPhaiSV.setAbleFocus(true);
        tfMatKhauSV.setAbleFocus(true);

        capNhatConTroDSSinhVien(false);
    }

    //Khi draw, phai load lai du lieu hien thi
    void draw(Graphics& g)
    {
        //reload data
        reloadData();

        Panel_t::draw(g);

        g.setColor(this->attribute);
        for (int i = 2; i < getCord().Y + 1; ++i)
        {
            g.write(64, i, "\xBA");
        }
    }

    bool mouseClick(int l, int t, Graphics& g)
    {
        int preCmbChucNangSelectedIndex = cmbChucNang.getSelectedIndex();

        int preCmbChonLopSelectedIndex = cmbChonLop.getSelectedIndex();

        if (! Panel_t::mouseClick(l, t, g))
            return false;

        if (cmbChucNang.isFocus())
        {
            int newCmbChucNangSelectedIndex = cmbChucNang.getSelectedIndex();

            if (preCmbChucNangSelectedIndex != newCmbChucNangSelectedIndex)
            {
                capNhatChonChucNang();
            }

            return true;
        }

        if (tbDSSV.isFocus())
        {
            generateInformationUIElement(true);
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
            return true;
        }

        if (cmbChonLop.isFocus())
        {
            int newCmbChonLopSelectedIndex = cmbChonLop.getSelectedIndex();

            if (preCmbChonLopSelectedIndex != newCmbChonLopSelectedIndex)
            {
                capNhatConTroDSSinhVien(true);
            }
        }

        return true;
    }

    void keyPress(int keyCode, char ch, Graphics& g)
    {
        if (tfMaSV.isFocus() || tfHoSV.isFocus() || tfTenSV.isFocus() || tfMatKhauSV.isFocus())
        {
            if (keyCode == KEY_ENTER)
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

                return;
            }
        }

        if (cmbChucNang.isFocus())
        {
            if (cmbChucNang.getOpenFlag() == false)
            {
                capNhatChonChucNang();

                return;
            }
        }

        if (cmbChonLop.isFocus())
        {
            if (cmbChonLop.getOpenFlag() == false)
            {
                capNhatConTroDSSinhVien(true);
            }
        }
    }
};


#endif // DSSINHVIENPANEL_H_INCLUDED
