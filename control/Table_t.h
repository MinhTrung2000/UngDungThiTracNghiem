#ifndef TABLE_T_H
#define TABLE_T_H

#include <string>
#include <sstream>
#include <stdint.h>
#include <cmath>

#include "Rect_t.h"
#include "RadioBox_t.h"
#include "CheckList_t.h"
#include "TextBox_t.h"
#include "Button_t.h"
#include "MessageBox_t.h"
#include "TextField_t.h"
#include "TextArea_t.h"
#include "ComboBox_t.h"
#include "TextBox_t.h"

#include "../view/UIForm.h"

#include "lib_vector.h"
#include "lib_stack.h"
#include "lib_utility.h"
#include "lib_avltree.h"

using namespace std;

inline int cmp (const string& s1, const string& s2)
{
    if (StringUtility::IsDigit(s1) && StringUtility::IsDigit(s2))
    {
        int d1 = atoi(s1.c_str());
        int d2 = atoi(s2.c_str());
        return (d1 < d2 ? 1 : (d1 == d2 ? 0 : -1));
    }
    //sort date type dd/mm/yyyy
    if (s1.size() == 10 && s1[2] == '/' && s1[5] == '/' &&
            s2.size() == 10 && s2[2] == '/' && s2[5] == '/')
    {
        int d1 = atoi(s1.substr(0, 2).c_str());
        int d2 = atoi(s2.substr(0, 2).c_str());
        int m1 = atoi(s1.substr(3, 2).c_str());
        int m2 = atoi(s2.substr(3, 2).c_str());
        int y1 = atoi(s1.substr(6, 4).c_str());
        int y2 = atoi(s2.substr(6, 4).c_str());
        if (y1 > y2)
            return 1;
        if (y1 == y2 && m1 > m2)
            return 1;
        if (y1 == y2 && m1 == m2 && d1 > d2)
            return 1;
        if (y1 == y2 && m1 == m2 && d1 == d2)
            return 0;
        return -1;
    }
    return (s1 < s2 ? 1 : (s1 == s2 ? 0 : -1));
}

class CheckListBox_t : public Rect_t
{
private:
    Vector<CheckList_t *> chlList;
    int currentIndex;

public:
    CheckListBox_t() : Rect_t(), currentIndex(-1) {}

    void push_back(CheckList_t *chlPtr)
    {
        assert(chlPtr != NULL);
        chlList.push_back(chlPtr);
    }

    void setCurrentListIndex(int id)
    {
        assert(0 <= id && id < chlList.size());
        currentIndex = id;
    }

    int getCurrentListIndex()
    {
        return currentIndex;
    }

    CheckList_t *getCurrentList()
    {
        if (currentIndex == -1)
            return NULL;
        return chlList[currentIndex];
    }

    void draw(Graphics& g)
    {
        if (chlList.empty())
            return;

        if (firstDrawFlag)
        {
            currentIndex = 0;

            Rect_t::setCoor(chlList[0]->getLeft(), chlList[0]->getTop(), chlList[0]->getCord());

            firstDrawFlag = false;
        }

        chlList[currentIndex]->port->drawAllLineFlag = true;
        chlList[currentIndex]->draw(g);
    }

    bool mouseClick(int l, int t, Graphics& g)
    {
        if (chlList.empty())
            return false;

        if (chlList[currentIndex]->mouseClick(l, t, g))
        {
            Rect_t::setFocus(*this);
            return true;
        }
        return false;
    }

    bool mouseScroll(int state, Graphics& g)
    {
        if (chlList.empty())
            return false;

        if (chlList[currentIndex]->mouseScroll(state, g))
        {
            Rect_t::setFocus(*this);
            return true;
        }

        return false;
    }

    void keyPress(int keyCode, char ch, Graphics& g)
    {
        if (chlList.empty())
            return;

        chlList[currentIndex]->keyPress(keyCode, ch, g);
    }

    bool canGetFocus()
    {
        return true;
    }

    CheckListBox_t& operator = (const CheckListBox_t& rhs)
    {
        Rect_t::operator=(rhs);

        chlList = rhs.chlList;

        currentIndex = rhs.currentIndex;

        return *this;
    }
};

class Table_t : public Rect_t
{
private:
    //Default attributes
    static const uint8_t DEFAULT_TABLE_ATTRIBUTES = 0xf1;

    static const uint8_t DEFAULT_HEADER_ATTRIBUTES = 0xe1;

    static const uint8_t DEFAULT_FOCUS_ROW_ATTRIBUTES = 0xb6;
    static const uint8_t DEFAULT_UNFOCUS_ROW_ATTRIBUTES = 0xf1;

    //Temporary file for full view table, it is opened by notepad.exe
    static const string DEFAULT_TEMP_FILE_FULLTABLE_PATH;

    //Default style of frame
    struct TableStyle
    {
        struct RowStyle
        {
            char  left, intersect, right;
        };

        char horizontal;
        char vertical;
        RowStyle top;
        RowStyle middle;
        RowStyle bottom;
    };

    //===========================================================================
    struct SortingSign : public Label_t
    {
        static const string INC_SORT_SIGN;
        static const uint8_t INC_SORT_ATTRIBUTES = 0xe2;

        static const string DEC_SORT_SIGN;
        static const uint8_t DEC_SORT_ATTRIBUTES = 0xe5;

        static const string NONE_SORT_SIGN;
        static const uint8_t NONE_SORT_ATTRIBUTES = 0xe7;

        //0: none, 1: inc, 2: dec
        short state;

        SortingSign() : Label_t(), state(0) {}
        SortingSign(int l, int t) : Label_t(l, t, NONE_SORT_SIGN), state(0)
        {
            setDefaultColor();
        }

        void setNoneState(Graphics& g)
        {
            state = 0;
            setValue(NONE_SORT_SIGN);
            setColor(NONE_SORT_ATTRIBUTES);
            draw(g);
        }

        void setDefaultColor()
        {
            setColor(NONE_SORT_ATTRIBUTES);
        }

        //if mouse click, set focus to table, redraw table and this sign
        //IF NONE STATE IS ACTIVE, SORT KEY COLUMN INCREASINGLY
        bool mouseClick(int l, int t, Graphics& g)
        {
            if (g.isInside(l, t, left, top, cord.X - 1, 0))
            {
                state = (state + 1) % 3;
                if (state == 0)
                {
                    setValue(NONE_SORT_SIGN);
                    setColor(NONE_SORT_ATTRIBUTES);
                }
                else if (state == 1)
                {
                    setValue(INC_SORT_SIGN);
                    setColor(INC_SORT_ATTRIBUTES);
                }
                else
                {
                    setValue(DEC_SORT_SIGN);
                    setColor(DEC_SORT_ATTRIBUTES);
                }
                draw(g);
                return true;
            }
            return false;
        }

        //ignore key press

        SortingSign& operator = (const SortingSign& rhs)
        {
            Label_t::operator=(rhs);

            state = rhs.state;

            return *this;
        }
    };

    //===========================================================================
    friend class FilterPanel;
    struct FilterPanel : public UIForm
    {

        static const int CMB_FIELD_LINE_NUMBER = 3;
        static const int ITEMLIST_LINE_NUMBER = 10;

        Table_t *table;

        Label_t lbField;
        ComboBox_t cmbField;

        Label_t lbItemList;

        Vector<CheckList_t> itemLists;
        CheckListBox_t chListManager;

        Button_t btnToggleAll;

        Button_t btnClearAll;

        //Default button : clear all filter and set table to original state
        Button_t btnDefault;

        Button_t btnOk;

        Button_t btnCancel;

        ButtonList_t btnListManager;

        Vector<Vector<string> > filter;

        void resetAll()
        {
            filter.clear();
            cmbField.setFirstSelected();
            for (int i = 0; i < itemLists.size(); ++i)
            {
                itemLists[i].setClearAll();
            }
            chListManager.setCurrentListIndex(0);
        }

        void getFilter()
        {
            filter.resize(table->nColumn);
            for (int i = 0; i < table->nColumn; ++i)
            {
                if (itemLists[i].havingCheck())
                {
                    filter[i] = itemLists[i].getSelectedValueList();
                }
            }
        }

        void chooseBtnToggleAll(Graphics& g)
        {
            chListManager.getCurrentList()->setToggleAll();

            Rect_t::setFocus(chListManager);
        }

        void chooseBtnClearAll(Graphics& g)
        {
            chListManager.getCurrentList()->setClearAll();

            Rect_t::setFocus(chListManager);
        }

        void chooseBtnDefault(Graphics& g)
        {
            //reset row to original
            table->rows = table->backUpRows;
            table->updateTableViewData();
            table->tableView.setDrawAllLinePort(true);

            resetAll();
            setCloseFlag(true);
        }

        void chooseBtnOk(Graphics& g)
        {
            getFilter();

            bool emptyFilter = true;
            for (int i = 0; i < filter.size(); ++i)
            {
                if (!filter[i].empty())
                {
                    emptyFilter = false;
                    break;
                }
            }

            if (emptyFilter)
            {
                showMessageDialog("Ban chua chon thong tin de loc!", MessageBox_t::INFORMATION_MESSAGE);
                return;
            }

            Vector<Vector<string> > newData;
            table->rows = table->backUpRows;

            for (int rowId = 0; rowId < table->rows.size(); ++rowId)
            {
                bool pass = true;
                for (int colId = 0; colId < table->nColumn; ++colId)
                {
                    if (!filter[colId].empty())
                    {
                        bool matching = false;
                        for (int eachValueId = 0; eachValueId < filter[colId].size(); ++eachValueId)
                        {
                            if (table->rows[rowId][colId] == filter[colId][eachValueId])
                            {
                                matching = true;
                                break;
                            }
                        }
                        if (! matching)
                        {
                            pass = false;
                            break;
                        }
                    }
                }
                if (pass)
                {
                    newData.push_back(table->rows[rowId]);
                }
            }

            table->rows = newData;
            table->updateTableViewData();
            table->tableView.setDrawAllLinePort(true);

            resetAll();
            setCloseFlag(true);
        }

        void chooseBtnCancel(Graphics& g)
        {
            resetAll();
            setCloseFlag(true);
        }

        //=====================================================
        FilterPanel(Table_t *tbPtr) : UIForm(BorderFactory::DOUBLE, 55, 12,
        {
            80, 30
        }, "Filter Option"), table(tbPtr)
        {
            setDefaultColor();

            lbField = Label_t(left + 2, top + 3, "Fields: ");
            lbField.setColor(this->attribute);
            add(&lbField);

            //<lbField> -5-> <cmbField>
            ///no problem if table->header is empty
            cmbField = ComboBox_t(lbField.getLeft() + lbField.getCord().X + 5, lbField.getTop() - 1, CMB_FIELD_LINE_NUMBER, table->headers);
            cmbField.setFirstSelected();
            add(&cmbField);

            //<lbField>
            //|
            //CMB_FIELD_LINE_NUMBER + 1
            //|
            //<lbItemList>
            lbItemList = Label_t(lbField.getLeft(), cmbField.getTop() + CMB_FIELD_LINE_NUMBER + 5, "List of items: ");
            lbItemList.setColor(this->attribute);
            add(&lbItemList);

            //set itemLists
            //<lbItemList>
            //|
            //1
            //|
            //-3-> <itemLists>
            itemLists.resize(table->nColumn);
            int sameLeft = lbItemList.getLeft() + 3;
            int sameTop = lbItemList.getTop() + 1;

            for (int i = 0; i < itemLists.size(); ++i)
            {
                itemLists[i] = CheckList_t(sameLeft, sameTop, 10, CheckBox_t::BOX_STYLE, table->getColumnValueList(i).unique(cmp));
                itemLists[i].setMaxWidthItem(30);
                itemLists[i].cord = {31, itemLists[i].getCord().Y};

                chListManager.push_back(&itemLists[i]);
            }
            add(&chListManager);

            //button Toggle/Clear
            //+-----------+ -3-> <btnToggleAll>
            //| itemLists | -3-> <btnClearAll>
            //+-----------+

            int maxWidOfItemList = 0;
            for (int i = 0; i < itemLists.size(); ++i)
                maxWidOfItemList = std::max(maxWidOfItemList, (int) itemLists[i].getCord().X);

            btnToggleAll = Button_t(itemLists[0].getLeft() + maxWidOfItemList + 3, sameTop, "Select all");
            btnToggleAll.setColor(DEFAULT_BUTTON_UNFOCUS_ATTRIBUTES);
            add(&btnToggleAll);

            btnClearAll = Button_t(btnToggleAll.getLeft(), sameTop + 2, "Clear all");
            btnClearAll.setColor(DEFAULT_BUTTON_UNFOCUS_ATTRIBUTES);
            add(&btnClearAll);

            //button Cancel
            btnCancel = Button_t(btnToggleAll.getLeft() + btnToggleAll.getCord().X - 7, itemLists[0].getTop() + itemLists[0].getCord().Y + 2, "CANCEL");
            btnCancel.setColor(DEFAULT_BUTTON_UNFOCUS_ATTRIBUTES);
            add(&btnCancel);

            //button OK
            //<btnDefault> -3-> <btnOk>
            btnOk = Button_t(btnCancel.getLeft() - 5, btnCancel.getTop(), "OK");
            btnOk.setColor(DEFAULT_BUTTON_UNFOCUS_ATTRIBUTES);
            add(&btnOk);

            //button Default
            btnDefault = Button_t(btnOk.getLeft() - 10, btnCancel.getTop(), "Default");
            btnDefault.setColor(DEFAULT_BUTTON_UNFOCUS_ATTRIBUTES);
            add(&btnDefault);


            btnListManager.push_back(&btnDefault);
            btnListManager.push_back(&btnOk);
            btnListManager.push_back(&btnCancel);

            //set Panel coor
            setCoor(left, top, {btnToggleAll.getLeft() + btnToggleAll.getCord().X + 2 - left, btnDefault.getTop() - top + 2});
            setBuffer();
        }

        bool mouseClick(int l, int t, Graphics& g)
        {
            if (! Panel_t::mouseClick(l, t, g))
                return false;

            if (cmbField.isFocus())
            {
                //if cmbField is empty, newId is -1
                int newId = cmbField.getSelectedIndex();
                if (newId != -1 && newId != chListManager.getCurrentListIndex())
                {
                    chListManager.setCurrentListIndex(cmbField.getSelectedIndex());
                    chListManager.getCurrentList()->port->drawAllLineFlag = true;
                    chListManager.draw(g);
                }
                return true;
            }

            if (btnToggleAll.isFocus())
            {
                chooseBtnToggleAll(g);
            }
            else if (btnClearAll.isFocus())
            {
                chooseBtnClearAll(g);
            }
            else if (btnDefault.isFocus())
            {
                chooseBtnDefault(g);
            }
            else if (btnOk.isFocus())
            {
                chooseBtnOk(g);
            }
            else if (btnCancel.isFocus())
            {
                chooseBtnCancel(g);
            }
            return true;
        }

        FilterPanel& operator = (const FilterPanel& rhs)
        {
            UIForm::operator = (rhs);

            table = rhs.table;
            lbField = rhs.lbField;
            cmbField = rhs.cmbField;
            lbItemList = rhs.lbItemList;
            itemLists = rhs.itemLists;
            btnToggleAll = rhs.btnToggleAll;
            btnClearAll = rhs.btnClearAll;
            btnDefault = rhs.btnDefault;
            btnOk = rhs.btnOk;
            btnCancel = rhs.btnCancel;
            btnListManager = rhs.btnListManager;
            chListManager = rhs.chListManager;

            return *this;
        }
    };

    static const TableStyle style;

    //flag
    bool tfFindFocus;

    //number of columns
    int nColumn;

    //index of primary key column
    int keyIndex;

    //headers of table
    Vector<string> headers;
    string headerFormat[2];
    string emptyLineFormat;

    //data
    Vector<Vector<string> > rows;
    Vector<Vector<string> > backUpRows;

    //view port
    TextBox_t tableView;
    //back up port line num of table view after using find option
    int savePortLineNum;

    //fixed width of each column
    Vector<size_t> columnWidth;

    //sorting sign of each column
    Vector<SortingSign *> sortingSignList;
    SortingSign *currentSortingSign;

    //Find by key option
    Label_t lbFind;
    TextField_t tfFind;

    //find value in column Index
    int findOptionColumnIndex;

    //Detail option
    Button_t btnDetail;

    //Filter option
    Button_t btnFilter;

    //Full table option
    Button_t btnFullTable;

    void setHeaderFormat();

    string getRows(int id);

    void sort(int columnId, int cmpOption, int from, int to);

    void sort(int columnId);

    void setSortingSignCoor();

    void resetSortingSign();

    void updateTableViewData();

    string outputText();

public:
    Table_t();
    //keyId set to 0 th column
    Table_t(int l, int t, int lineNum, const Vector<string>& _headers);
    Table_t(int l, int t, int keyId, int lineNum, const Vector<string>& _headers);

    bool empty()
    {
        return tableView.empty();
    }

    bool isFocus();

    void setFirstDrawFlag(bool flag);

    void setDefaultColor();

    void setFirstSelected();

    void setFixedWidths(const Vector<size_t>& widths);

    int getRowIndexByKey(const string& keyValue);
    int getRowIndexByKey(const string& keyValue, int colId);

    Vector<Vector<string> > getDataOfTable()
    {
        return backUpRows;
    }

    void setFindOptionColumnIndex(int columnId);

    //load data to table
    void setData(const Vector<Vector<string> >& newData);

    //add transact restore
    void clearAll();

    void insertRow(const Vector<string>& newRow);
    void insertRowByIndex(int id, const Vector<string>& newRow);

    void removeRowByIndex(int id);
    void removeRowByKey(const string& key, int colId);

    void updateRowByIndex(int id, const Vector<string>& newRow);
    void updateRowByKey(const string& key, int colId, const Vector<string>& newRow);

    Vector<string> getColumnValueList(int columnIndex);

    void setSelectedIndex(int id);

    int getSelectedIndex();

    Vector<string> getSelectedValue();

    void draw(Graphics& g);

    bool mouseScroll(int state, Graphics& g);

    bool mouseClick(int l, int t, Graphics& g);

    void keyPress(int keyCode, char ch, Graphics& g);

    Table_t& operator = (const Table_t& o)
    {
        Rect_t::operator=(o);

        tfFindFocus = o.tfFindFocus;

        canUpdateBufferFlag = o.canUpdateBufferFlag;

        nColumn = o.nColumn;
        keyIndex = o.keyIndex;
        headers = o.headers;

        for (int i = 0; i < 2; ++i)
        {
            headerFormat[i] = o.headerFormat[i];
        }

        emptyLineFormat = o.emptyLineFormat;

        rows = o.rows;
        backUpRows = o.backUpRows;

        tableView = o.tableView;
        savePortLineNum = savePortLineNum;

        columnWidth = o.columnWidth;

        findOptionColumnIndex = o.findOptionColumnIndex;

        sortingSignList = o.sortingSignList;

        currentSortingSign = o.currentSortingSign;

        lbFind = o.lbFind;

        tfFind = o.tfFind;

        btnDetail = o.btnDetail;

        btnFilter = o.btnFilter;

        btnFullTable = o.btnFullTable;
        return *this;
    }
};

#endif // TABLE_T_H
