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
#include "../control/TimeCounter.h"

#include "DSMonHocPanel.h"
#include "DSLopPanel.h"
#include "DSSinhVienPanel.h"
#include "DiemThiLopPanel.h"
#include "DiemThiSinhVienPanel.h"
#include "CauHoiThiPanel.h"

using namespace std;

class SVExecutiveForm : public UIForm
{
private:
    friend class DinhDangBaiThiPanel;
    struct DinhDangBaiThiPanel : public UIForm
    {
        SVExecutiveForm *fpanel;

        Label_t lbMaMon;
        ComboBox_t cmbMaMon;
        Label_t lbTenMon;

        Label_t lbNhapSoCau;
        TextField_t tfNhapSoCau;

        Label_t lbNhapGioThi;
        ComboBox_t cmbNhapGioThi;

        Label_t lbGhiChu;

        Button_t btnOk;
        Button_t btnThoat;

        void generateInformationUIElement(Graphics& g, bool drawFlag)
        {
            lbTenMon.clearValue();

            if (fpanel->dsMon->empty())
            {
                lbTenMon.setValue("*Danh sach mon hoc rong!");
            }
            else
            {
                int maMonId = cmbMaMon.getSelectedIndex();
                string tenMon = fpanel->dsMon->getMonHoc(maMonId)->getTenMon();
                lbTenMon.setValue("Ten mon: " + tenMon);
            }

            if (drawFlag)
            {
                lbTenMon.draw(g);
            }
        }

        DinhDangBaiThiPanel(SVExecutiveForm *_fpanel)
            : UIForm(BorderFactory::DOUBLE, 55, 12,
        {
            60, 23
        }, "Tao Bai Thi"),
        fpanel(_fpanel)
        {
            setDefaultColor();

            lbMaMon = Label_t(left + 5, top + 3, "Chon ma mon thi:");
            lbMaMon.setColor(this->attribute);
            add(&lbMaMon);

            cmbMaMon = ComboBox_t(lbMaMon.getLeft() + lbMaMon.getCord().X + 3, lbMaMon.getTop() - 1, 3, fpanel->dsMon->getDSMaMon());
            add(&cmbMaMon);

            lbTenMon = Label_t(BorderFactory::SINGLE, lbMaMon.getLeft() - 1, lbMaMon.getTop() + 3, "Ten Mon: ");
            lbTenMon.setMaxWidth(50);
            lbTenMon.setColor(this->attribute);
            add(&lbTenMon);

            lbNhapSoCau = Label_t(lbMaMon.getLeft(), lbTenMon.getTop() + 5, "Nhap so cau thi:");
            lbNhapSoCau.setColor(this->attribute);
            add(&lbNhapSoCau);

            tfNhapSoCau = TextField_t(BorderFactory::SINGLE, lbNhapSoCau.getLeft() + lbNhapSoCau.getCord().X + 3, lbNhapSoCau.getTop() - 1, 3);
            add(&tfNhapSoCau);

            lbNhapGioThi = Label_t(lbNhapSoCau.getLeft(), lbNhapSoCau.getTop() + 5, "Nhap so phut thi:");
            lbNhapGioThi.setColor(this->attribute);
            add(&lbNhapGioThi);

            Vector<string> gioThiOptions;
            int time = 15;
            for (int i = 0; i < 22; ++i)
            {
                gioThiOptions.push_back(StringUtility::to_string(time));
                time += 5;
            }

            cmbNhapGioThi = ComboBox_t(tfNhapSoCau.getLeft(), lbNhapGioThi.getTop() - 1, 3, gioThiOptions);
            add(&cmbNhapGioThi);

            btnThoat = Button_t(left + cord.X - 10, lbNhapGioThi.getTop() + 5, "Thoat");
            add(&btnThoat);

            btnOk = Button_t(btnThoat.getLeft() - 10, btnThoat.getTop(), "OK");
            add(&btnOk);
        }

        void reloadData()
        {
            generateInformationUIElement(*Graphics::getCurrentActiveGraphics(), false);
        }

        void draw(Graphics& g)
        {
            reloadData();

            Panel_t::draw(g);
        }

        bool mouseClick(int l, int t, Graphics& g)
        {
            int preCmbMaMonSelectedIndex = cmbMaMon.getSelectedIndex();

            if (! Panel_t::mouseClick(l, t, g))
                return false;

            if (btnOk.isFocus())
            {
                string soCauStr = tfNhapSoCau.getValue();

                StringUtility::Trim(soCauStr);

                if (soCauStr.empty())
                {
                    showMessageDialog("Ban chua dien so cau hoi thi!", MessageBox_t::ERROR_MESSAGE);
                    Rect_t::setFocus(tfNhapSoCau);
                    return false;
                }

                if (! StringUtility::IsDigit(soCauStr))
                {
                    showMessageDialog("So cau hoi khong hop le!", MessageBox_t::ERROR_MESSAGE);
                    Rect_t::setFocus(tfNhapSoCau);
                    return false;
                }

                int soCau = atoi(soCauStr.c_str());

                if (soCau == 0)
                {
                    showMessageDialog("Nhap it nhat 1 cau hoi!", MessageBox_t::ERROR_MESSAGE);
                    return false;
                }
                string maMonThi = cmbMaMon.getSelectedValue();

                if (fpanel->currentSinhVien->getDiemMonDaThi(maMonThi) != NULL)
                {
                    showMessageDialog("Sinh vien da thi mon hoc nay!\nVui long chon mon hoc khac.", MessageBox_t::ERROR_MESSAGE);
                    return false;
                }

                fpanel->NganHangCauHoi->layDanhSachCauHoiTheoMon(maMonThi, true, fpanel->dsCauHoi);

                if (fpanel->dsCauHoi.empty())
                {
                    showMessageDialog("Ngan hang de thi chua co san cau hoi thi cho mon hoc co ma " + maMonThi + ".\nVui long chon mon hoc khac!", MessageBox_t::ERROR_MESSAGE);
                    return false;
                }

                if (fpanel->dsCauHoi.size() < soCau)
                {
                    string rep = showOptionDialog("So cau hoi thi hien tai cua mon hoc co ma " + maMonThi + "\nla " + StringUtility::to_string(fpanel->dsCauHoi.size()) + " cau hoi, khong du so cau hoi thi yeu cau.\n"
                                                  + "Xac nhan tiep tuc?", MessageBox_t::INFORMATION_MESSAGE, MessageBox_t::OK_CANCEL_OPTION);

                    if (rep == "CANCEL")
                        return false;
                }
                else
                {
                    fpanel->dsCauHoi.resize(soCau);
                    soCau = fpanel->dsCauHoi.size();
                }

                fpanel->dsCauTraLoiBuffer.resize(soCau, ' ');

                fpanel->diemThi = new DiemThi(maMonThi, fpanel->dsCauHoi, fpanel->dsCauTraLoiBuffer, 0);

                int time = atoi(cmbNhapGioThi.getSelectedValue().c_str()) * 60;

                int h = time / 3600;
                int m = time % 3600 / 60;
                int s = time % 3600 % 60;

                fpanel->timeCounter.setInitTime(h, m, s);

                stringstream line;

                line << "Sinh vien du thi: " << fpanel->currentSinhVien->getMaSV() << " - " << fpanel->currentSinhVien->getHo() << " " << fpanel->currentSinhVien->getTen() << ".\n";
                line << "Mon thi: " << maMonThi << " - " << fpanel->dsMon->getMonHoc(maMonThi)->getTenMon() << ".\n";
                line << "So cau hoi: " << soCau << " cau.\n";
                line << "Thoi gian lam bai: " << cmbNhapGioThi.getSelectedValue() << " phut.";

                fpanel->lbGhiChu.setValue(line.str());

                fpanel->capNhatDSCauHoi(false);

                setCloseFlag(true);

                return true;
            }

            if (btnThoat.isFocus())
            {
                fpanel->setCloseFlag(true);

                setCloseFlag(true);

                return true;
            }

            if (cmbMaMon.isFocus())
            {
                int newCmbMaMonSelectedIndex = cmbMaMon.getSelectedIndex();

                if (preCmbMaMonSelectedIndex != newCmbMaMonSelectedIndex)
                {
                    generateInformationUIElement(g, true);
                }
            }

            return true;
        }
    };

    DSMonHoc_t *dsMon;

    DSCauHoiThi_t *NganHangCauHoi;

    SinhVien *currentSinhVien;

    Panel_t *logForm;

    //Ghi thong tin thi sinh thi, mon thi, tong so cau va thoi gian lam bai
    Label_t lbGhiChu;

    //Danh sach cau hoi
    Label_t lbDSCauTraLoi;
    TextBox_t txbDSCauTraLoi;

    //Noi dung cau hoi
    Label_t lbCauHoi;
    Label_t lbDe;
    RadioBox_t rbLuaChon;

    Button_t btnCauTruoc;
    Button_t btnCauKe;

    Button_t btnHoanThanh;
    Button_t btnThoat;

    TimeCounter_t timeCounter;

    DiemThi *diemThi;

    Vector<CauHoiThi *> dsCauHoi;

    Vector<char> dsCauTraLoiBuffer;

    void generateInformationUIElement(Graphics &g, bool drawFlag)
    {
        lbDe.clearValue();

        rbLuaChon.clearAll();

        int cauHoiId = txbDSCauTraLoi.getSelectedIndex();

        assert(0 <= cauHoiId && cauHoiId < dsCauHoi.size());

        string deCauHoi = dsCauHoi[cauHoiId]->getCauHoi();

        lbDe.setValue(getTextToken(deCauHoi, 5, 80));

        Vector<string> luaChonData = dsCauHoi[cauHoiId]->getLuaChon();
        for (int i = 0; i < luaChonData.size(); ++i)
        {
            luaChonData[i] = getTextToken(luaChonData[i], 2, 80);
        }

        rbLuaChon.setData(luaChonData);

        char traLoi = dsCauTraLoiBuffer[cauHoiId];
        if (traLoi != ' ')
        {
            rbLuaChon.setSelectedIndex(traLoi - 'A');
        }

        if (drawFlag)
        {
            lbDe.draw(g);
            rbLuaChon.draw(g);
        }
    }

    void capNhatDSCauHoi(bool drawFlag)
    {
        txbDSCauTraLoi.setData(getDSLietKeCauHoi());
        txbDSCauTraLoi.setFirstSelected();

        if (drawFlag)
        {
            txbDSCauTraLoi.draw(*Graphics::getCurrentActiveGraphics());
        }

        generateInformationUIElement(*Graphics::getCurrentActiveGraphics(), drawFlag);
    }

    Vector<string> getDSLietKeCauHoi()
    {
        Vector<string> res;
        for (int i = 0; i < dsCauHoi.size(); ++i)
        {
            res.push_back(getRowAtIndex(i));
        }
        return res;
    }

    string getRowAtIndex(int id)
    {
        assert(0 <= id && id < dsCauHoi.size());

        return (string(2, ' ') + string(1, dsCauTraLoiBuffer[id]) + "    Cau hoi " + StringUtility::to_string(id + 1));
    }

public:
    SVExecutiveForm(SinhVien *sv, Panel_t *_logForm)
        : UIForm(BorderFactory::PLAIN, 0, 0,
    {
        170, 45
    }, ""),
    dsMon(DSMonHoc_t::getDSMonHocHienHanh()),
    NganHangCauHoi(DSCauHoiThi_t::getDSCauHoiThiHienHanh()),
    currentSinhVien(sv),
    logForm(_logForm),
    diemThi(NULL)
    {

        setColor(0xf0);

        lbGhiChu = Label_t(left + 3, top + 1, "");
        lbGhiChu.setColor(this->attribute);
        add(&lbGhiChu);

        lbDSCauTraLoi = Label_t(BorderFactory::SINGLE, lbGhiChu.getLeft(), lbGhiChu.getTop() + 6, "Danh sach cau hoi");
        lbDSCauTraLoi.setColor(this->attribute);
        add(&lbDSCauTraLoi);

        txbDSCauTraLoi = TextBox_t(lbGhiChu.getLeft(), lbDSCauTraLoi.getTop() + 3, 32);
        add(&txbDSCauTraLoi);

        lbCauHoi = Label_t(left + 45, txbDSCauTraLoi.getTop(), "Cau hoi:");
        lbCauHoi.setColor(this->attribute);
        add(&lbCauHoi);

        lbDe = Label_t(lbCauHoi.getLeft() + 10, lbCauHoi.getTop(), "");
        lbDe.setColor(this->attribute);
        add(&lbDe);

        rbLuaChon = RadioBox_t(lbDe.getLeft(), lbDe.getTop() + 10);
        add(&rbLuaChon);

        btnCauTruoc = Button_t(lbDe.getLeft() + 60, 35, "Cau truoc");
        add(&btnCauTruoc);

        btnCauKe = Button_t(btnCauTruoc.getLeft() + btnCauTruoc.getCord().X + 3, btnCauTruoc.getTop(), "Cau ke");
        add(&btnCauKe);

        btnHoanThanh = Button_t(125, 2, "Hoan thanh");
        add(&btnHoanThanh);

        btnThoat = Button_t(btnHoanThanh.getLeft() + btnHoanThanh.getCord().X + 3, btnHoanThanh.getTop(), "Thoat");
        add(&btnThoat);

        timeCounter = TimeCounter_t(btnThoat.getLeft() + btnThoat.getCord().X + 5, btnThoat.getTop() - 1);
        GlobalTimeCounter_t::getGlobalTimeCounter()->add(&timeCounter);
        add(&timeCounter);
    }

    ~SVExecutiveForm()
    {
        delete diemThi;

        logForm->setFirstDrawFlag(true);
        logForm->setReRunFlag(true);
    }

    void reloadData()
    {
        DinhDangBaiThiPanel *pn = new DinhDangBaiThiPanel(this);
        UIForm::drawForm(pn, false);
    }

    void draw(Graphics& g)
    {
        reloadData();

        Panel_t::draw(g);
    }

    void getActionTimeEnding()
    {
        for (int i = 0; i < dsCauHoi.size(); ++i)
        {
            dsCauHoi[i]->setCanModifyFlag(false);
        }

        diemThi->setDSCautTraLoi(dsCauTraLoiBuffer);

        diemThi->tinhDiem();

        currentSinhVien->themDiemThi(*diemThi);

        stringstream line;
        line << "Het thoi gian lam bai!" << "\n";
        line << "Ma so: " << currentSinhVien->getMaSV() << ".\n";
        line << "Ho ten: " << currentSinhVien->getHo() << " " << currentSinhVien->getTen() << ".\n";
        line << "Diem so: " << diemThi->getDiem() << " diem.";

        showMessageDialog("KET QUA", line.str(), MessageBox_t::INFORMATION_MESSAGE);

        setCloseFlag(true);
    }

    bool mouseClick(int l, int t, Graphics& g)
    {
        int preSelectedCauHoiThiId = txbDSCauTraLoi.getSelectedIndex();

        if (!Panel_t::mouseClick(l, t, g))
            return false;

        if (txbDSCauTraLoi.isFocus())
        {
            generateInformationUIElement(g, true);
            return true;
        }

        if (rbLuaChon.isFocus())
        {
            int selectedId = rbLuaChon.getSelectedIndex();
            if (selectedId == -1)
            {
                dsCauTraLoiBuffer[preSelectedCauHoiThiId] = ' ';
            }
            else
            {
                dsCauTraLoiBuffer[preSelectedCauHoiThiId] = 'A' + selectedId;
            }
            txbDSCauTraLoi.replaceItem(preSelectedCauHoiThiId, getRowAtIndex(preSelectedCauHoiThiId));
            txbDSCauTraLoi.draw(g);

            return true;
        }

        if (btnCauTruoc.isFocus())
        {
            if (preSelectedCauHoiThiId != 0)
            {
                txbDSCauTraLoi.setSelectedIndex(preSelectedCauHoiThiId - 1);
                txbDSCauTraLoi.draw(g);
                generateInformationUIElement(g, true);
                return true;
            }
        }

        if (btnCauKe.isFocus())
        {
            if (preSelectedCauHoiThiId != dsCauHoi.size() - 1)
            {
                txbDSCauTraLoi.setSelectedIndex(preSelectedCauHoiThiId + 1);
                txbDSCauTraLoi.draw(g);
                generateInformationUIElement(g, true);
                return true;
            }
        }

        if (btnHoanThanh.isFocus())
        {
            string rep = showOptionDialog("Xac nhan nop bai?", MessageBox_t::WARNING_MESSAGE, MessageBox_t::OK_CANCEL_OPTION);
            if (rep == "CANCEL")
            {
                return true;
            }

            for (int i = 0; i < dsCauHoi.size(); ++i)
            {
                dsCauHoi[i]->setCanModifyFlag(false);
            }

            diemThi->setDSCautTraLoi(dsCauTraLoiBuffer);

            diemThi->tinhDiem();

            currentSinhVien->themDiemThi(*diemThi);

            stringstream line;
            line << "Ma so: " << currentSinhVien->getMaSV() << ".\n";
            line << "Ho ten: " << currentSinhVien->getHo() << " " << currentSinhVien->getTen() << ".\n";
            line << "Diem so: " << diemThi->getDiem() << " diem.";

            showMessageDialog("KET QUA", line.str(), MessageBox_t::INFORMATION_MESSAGE);

            setCloseFlag(true);

            return true;
        }

        if (btnThoat.isFocus())
        {
            string rep = showOptionDialog("Xac nhan ket thuc bai thi va thoat?", MessageBox_t::WARNING_MESSAGE, MessageBox_t::OK_CANCEL_OPTION);
            if (rep == "CANCEL")
            {
                return true;
            }

            setCloseFlag(true);

            return true;
        }

        return true;
    }
};
