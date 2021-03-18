#include "Table_t.h"

const string Table_t::SortingSign::INC_SORT_SIGN = " \x1e";

const string Table_t::SortingSign::DEC_SORT_SIGN = " \x1f";

const string Table_t::SortingSign::NONE_SORT_SIGN = "\x1e\x1f";

const string Table_t::DEFAULT_TEMP_FILE_FULLTABLE_PATH = "fullTable.txt";

const Table_t::TableStyle Table_t::style = {'\xCD', '\xBA', {'\xC9', '\xCB', '\xBB'}, {'\xC8', '\xCA', '\xBC'}, {'\xC8', '\xCF', '\xBC'}};

Table_t::Table_t()
    : Rect_t(),
      nColumn(0),
      currentSortingSign(NULL),
      findOptionColumnIndex(0),
      tfFindFocus(false) {}

Table_t::Table_t(int l, int t, int lineNum, const Vector<string>& _headers)
    : Table_t(l, t, 0, lineNum, _headers) {}

Table_t::Table_t(int l, int t, int keyId,  int lineNum, const Vector<string>& _headers)
    : Rect_t(l, t),
      nColumn(_headers.size()),
      keyIndex(keyId),
      headers(_headers),
      tableView(BorderFactory::DOUBLE, l, t + 3, lineNum, Vector<string>()),
      currentSortingSign(NULL),
      findOptionColumnIndex(keyIndex),
      tfFindFocus(false)
{

    columnWidth.resize(nColumn);
    for (int i = 0; i < nColumn; ++i)
        columnWidth[i] = _headers[i].size() + 5;

    setHeaderFormat();

    sortingSignList.resize(nColumn, NULL);

    int nextLeft = left;
    for (int i = 0; i < nColumn; ++i)
    {
        nextLeft += 1 + columnWidth[i];
        sortingSignList[i] = new SortingSign(nextLeft - 2, top + 2);
    }

    currentSortingSign = sortingSignList[keyIndex];

    savePortLineNum = lineNum;
}

void Table_t::setHeaderFormat()
{
    stringstream line;
    line << style.top.left;
    for (int i = 0; i < nColumn; ++i)
    {
        line << string(columnWidth[i], style.horizontal);
        line << (i == nColumn - 1 ? style.top.right : style.top.intersect);
    }

    headerFormat[0] = line.str();
    line.str(string());
    line.clear();

    line << style.vertical;
    for (int i = 0; i < nColumn; ++i)
    {
        string text = headers[i];
        size_t remaining = columnWidth[i] - text.size();
        size_t half = remaining / 2;
//    assert(remaining >= half);
        line << string(half, ' ') << text << string(remaining - half, ' ');
        line << style.vertical;
    }

    headerFormat[1] = line.str();

    //set empty line format
    line.str(string());
    line.clear();

    for (int i = 0; i < nColumn; ++i)
    {
        line << string(columnWidth[i], ' ') << style.vertical;
    }

    emptyLineFormat = line.str();
    tableView.setEmptyLineFormatTable(emptyLineFormat);
    //set table cord
    tableView.cord.X = headerFormat[0].size() - 2;

    lbFind = Label_t(left, top, "Find by " + headers[findOptionColumnIndex] + ": ");
    tfFind = TextField_t(BorderFactory::NONE, left + lbFind.getCord().X + 3, top, 10, 30);
    tfFind.setColor(0x79);

    int nextLeft = left + headerFormat[0].size() - 30;
    int nextTop = top;

    if (nextLeft < tfFind.getLeft() + tfFind.getCord().X)
    {
        nextTop += tableView.getCord().Y + 4;
    }

    btnDetail = Button_t(left + headerFormat[0].size() - 30, nextTop, "Detail");
    btnFilter = Button_t(btnDetail.getLeft() + btnDetail.getCord().X + 1, nextTop, "Filter");
    btnFullTable = Button_t(btnFilter.getLeft() + btnFilter.getCord().X + 1, nextTop, "Full Table");
}

void Table_t::setFirstDrawFlag(bool flag)
{
    if (flag == true)
    {
        firstDrawFlag = true;
        tableView.setFirstDrawFlag(true);
    }
}

void Table_t::setFindOptionColumnIndex(int columnId)
{
    assert(0 <= columnId && columnId < nColumn);
    findOptionColumnIndex = columnId;
    lbFind = Label_t(left, top, "Find by " + headers[findOptionColumnIndex] + ": ");
}

int Table_t::getRowIndexByKey(const string& keyValue)
{
    for (int i = 0; i < backUpRows.size(); ++i)
    {
        if (backUpRows[i][keyIndex] == keyValue)
            return i;
    }
    return -1;
}

int Table_t::getRowIndexByKey(const string& keyValue, int colId)
{
    for (int i = 0; i < backUpRows.size(); ++i)
    {
        if (backUpRows[i][colId] == keyValue)
            return i;
    }
    return -1;
}

string Table_t::getRows(int id)
{
    assert(0 <= id && id < rows.size());

    stringstream line;
    for (int i = 0; i < nColumn; ++i)
    {
        string text = rows[id][i];
        StringUtility::adjustStringToFit(text, columnWidth[i]);
        size_t remaining = columnWidth[i] - text.size();
        size_t half = ceil(remaining / 2);
        line << string(half, ' ') << text << string(remaining - half, ' ');
        if (i != nColumn - 1)
            line << style.vertical;
    }
    return line.str();
}

void Table_t::setDefaultColor()
{
    setColor(DEFAULT_TABLE_ATTRIBUTES);
    tableView.setColor(DEFAULT_UNFOCUS_ROW_ATTRIBUTES);
    tableView.setColorEle(DEFAULT_FOCUS_ROW_ATTRIBUTES, DEFAULT_UNFOCUS_ROW_ATTRIBUTES);
    lbFind.setColor(this->attribute);
    btnDetail.setColor(DEFAULT_BUTTON_UNFOCUS_ATTRIBUTES);
    btnFilter.setColor(DEFAULT_BUTTON_UNFOCUS_ATTRIBUTES);
    btnFullTable.setColor(DEFAULT_BUTTON_UNFOCUS_ATTRIBUTES);
}

void Table_t::setFirstSelected()
{
    tableView.setFirstSelected();
}

void Table_t::setFixedWidths(const Vector<size_t>& widths)
{
    columnWidth = widths;
    setHeaderFormat();
}

void Table_t::setSelectedIndex(int id)
{
    tableView.setSelectedIndex(id);
}

Vector<string> Table_t::getColumnValueList(int columnId)
{
    assert(0 <= columnId && columnId < nColumn);

    Vector<string> res;
    for (int i = 0; i < backUpRows.size(); ++i)
        res.push_back(backUpRows[i][columnId]);
    return res;
}

void Table_t::resetSortingSign()
{
    currentSortingSign = sortingSignList[keyIndex];
    currentSortingSign->state = 1;
    currentSortingSign->setValue(SortingSign::INC_SORT_SIGN);
    currentSortingSign->setColor(SortingSign::INC_SORT_ATTRIBUTES);
}

void Table_t::setData(const Vector<Vector<string> >& newData)
{
    if (! newData.empty())
    {
        assert(newData[0].size() == nColumn);
    }

    backUpRows = newData;
    rows = backUpRows;

    resetSortingSign();
    sort(keyIndex);

    updateTableViewData();

//  int maxWidKeyColumn = 10;
//  for (int i = 0; i < backUpRows.size(); ++i) {
//    maxWidKeyColumn = std::max(maxWidKeyColumn, (int) backUpRows[i][findOptionColumnIndex].size());
//  }
//
//  tfFind = TextField_t(left + lbFind.getCord().X, top, maxWidKeyColumn);

    firstDrawFlag = true;
}

int Table_t::getSelectedIndex()
{
    //if text box is empty, return -1
    return tableView.getSelectedIndex();
}

Vector<string> Table_t::getSelectedValue()
{
    int id = tableView.getSelectedIndex();
//  assert (id != -1); //debug
    if (id == -1)
        return Vector<string>();
    return rows[id];
}

void Table_t::clearAll()
{
    if (tableView.empty())
        return;

    backUpRows.clear();
    rows = backUpRows;

    tableView.clearAll();
    tableView.draw(*Graphics::getCurrentActiveGraphics());
    Rect_t::setFocus(tableView);
}

void Table_t::insertRow(const Vector<string>& newRow)
{
    assert(newRow.size() == nColumn);

    backUpRows.push_back(newRow);
    rows = backUpRows;

    //
    tableView.insertNewItem(getRows(rows.size() - 1));
    tableView.draw(*Graphics::getCurrentActiveGraphics());
    Rect_t::setFocus(tableView);
}

void Table_t::insertRowByIndex(int id, const Vector<string>& newRow)
{
    assert(0 <= id && id < backUpRows.size());
    assert(newRow.size() == nColumn);

    backUpRows.insert(id, newRow);
    rows = backUpRows;

    tableView.insertNewItem(id, getRows(id));
    tableView.draw(*Graphics::getCurrentActiveGraphics());
    Rect_t::setFocus(tableView);
}

void Table_t::removeRowByIndex(int id)
{
    assert(0 <= id && id < backUpRows.size());

    backUpRows.remove(id);
    rows = backUpRows;

    tableView.deleteItem(id);
    tableView.draw(*Graphics::getCurrentActiveGraphics());
    Rect_t::setFocus(tableView);
}

void Table_t::removeRowByKey(const string& keyValue, int colId)
{
    assert(0 <= colId && colId < nColumn);

    for (int i = 0; i < backUpRows.size(); ++i)
    {
        if (backUpRows[i][colId] == keyValue)
        {
            backUpRows.remove(i);
            rows = backUpRows;

            tableView.deleteItem(i);
            tableView.draw(*Graphics::getCurrentActiveGraphics());
            Rect_t::setFocus(tableView);
            return;
        }
    }
}

void Table_t::updateRowByIndex(int id, const Vector<string>& newRow)
{
    assert(0 <= id && id < backUpRows.size());
    assert(newRow.size() == nColumn);

    backUpRows[id] = newRow;
    rows = backUpRows;

    tableView.replaceItem(id, getRows(id));
    tableView.draw(*Graphics::getCurrentActiveGraphics());
    Rect_t::setFocus(tableView);
}

void Table_t::updateRowByKey(const string& key, int colId, const Vector<string>& newRow)
{
    assert(0 <= colId && colId < nColumn);
    assert(newRow.size() == nColumn);

    for (int i = 0; i < backUpRows.size(); ++i)
    {
        if (backUpRows[i][colId] == key)
        {
            backUpRows[i] = newRow;
            rows = backUpRows;

            tableView.replaceItem(i, getRows(i));
            tableView.draw(*Graphics::getCurrentActiveGraphics());
            Rect_t::setFocus(tableView);
            return;
        }
    }
}

void Table_t::draw(Graphics& g)
{
    if (! canDrawFlag)
    {
        canDrawFlag = true;
        return;
    }

    if (firstDrawFlag)
    {
        setSortingSignCoor();

        //draw header
        g.setColor(DEFAULT_HEADER_ATTRIBUTES);

        g.write(left, top + 1, headerFormat[0]);
        g.write(left, top + 2, headerFormat[1]);

        for (int i = 0; i < nColumn; ++i)
        {
            sortingSignList[i]->draw(g);
        }

        setDefaultColor();

        //Find option
        lbFind.draw(g);
        tfFind.draw(g);

        //detail option
        btnDetail.draw(g);

        //filter option
        btnFilter.draw(g);

        //full table option
        btnFullTable.draw(g);

        firstDrawFlag = false;
    }

    if (tfFindFocus)
    {
        tfFind.draw(g);
        return;
    }

    tableView.draw(g);
}

bool Table_t::mouseScroll(int state, Graphics& g)
{
    return tableView.mouseScroll(state, g);
}

bool Table_t::mouseClick(int l, int t, Graphics& g)
{
    //click at sorting signs
    if (g.isInside(l, t, left + 1, top + 2, tableView.getCord().X - 1, 0))
    {
        for (int i = 0; i < nColumn; ++i)
        {
            if (sortingSignList[i]->mouseClick(l, t, g))
            {
                tfFindFocus = false;
                if (currentSortingSign->state == 0)
                {
                    //sort key column inc
                    sortingSignList[keyIndex]->state = 1;
                    sortingSignList[keyIndex]->setValue(SortingSign::INC_SORT_SIGN);
                    sortingSignList[keyIndex]->setColor(SortingSign::INC_SORT_ATTRIBUTES);
                    currentSortingSign = sortingSignList[keyIndex];
                    currentSortingSign->draw(g);
                    sort(keyIndex);
                }
                else if (currentSortingSign != sortingSignList[i])
                {
                    currentSortingSign->setNoneState(g);
                    currentSortingSign = sortingSignList[i];
                    sort(i);
                }
                else
                {
                    //click at current sign, modify this sign
                    sort(i);
                }

                updateTableViewData();

                tableView.port->drawAllLineFlag = true;
                tableView.draw(g);
                canDrawFlag = false;
                Rect_t::setFocus(*this);
                return true;
            }
        }
    }
    if (tfFind.mouseClick(l, t, g))
    {
        tfFindFocus = true;
        tfFind.draw(g);
        g.setCursorVisibility(true);
        Rect_t::setFocus(*this);
        return true;
    }
    if (btnDetail.mouseClick(l, t, g))
    {
        tfFindFocus = false;
        if (! rows.empty())
        {
            stringstream line;
            int rowId = tableView.getSelectedIndex();
            for (int i = 0; i < nColumn; ++i)
            {
                line << headers[i] << ": " << rows[rowId][i] << "\n";
            }
            showMessageDialog("Information", line.str(), MessageBox_t::INFORMATION_MESSAGE);

            canDrawFlag = false;
            Rect_t::setFocus(*this);
            return true;
        }
        return false;
    }
    if (btnFilter.mouseClick(l, t, g))
    {
        if (backUpRows.empty())
            return false;

        tfFindFocus = false;

        //clear tfFind data
        tfFind.clear();
        tfFind.draw(g);

        //restore data after press OK in filter panel
        //not back up here for case press CANCEL in filter panel

        FilterPanel *pnFilter = new FilterPanel(this);
        UIForm::drawForm(pnFilter, false);

        Rect_t::setFocus(tableView);
        return true;
    }
    if (btnFullTable.mouseClick(l, t, g))
    {
        tfFindFocus = false;

        ofstream fw(DEFAULT_TEMP_FILE_FULLTABLE_PATH.c_str());
        if (!fw.good())
        {
            showMessageDialog("Loi He Thong", "Khong the su dung chuc nang nay!", MessageBox_t::ERROR_MESSAGE);
            fw.close();
            return false;
        }
        fw << outputText();
        fw.close();

        Rect_t::setFocus(*this);

        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory( &si, sizeof(si) );
        si.cb = sizeof(si);
        ZeroMemory( &pi, sizeof(pi) );

        string cmd = " " + DEFAULT_TEMP_FILE_FULLTABLE_PATH;
        char cmdChrs[100];
        strcpy(cmdChrs, cmd.c_str());
        assert(CreateProcess("C:\\Windows\\System32\\notepad.exe", cmdChrs, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi));
        CloseHandle( pi.hProcess );
        CloseHandle( pi.hThread );

        Rect_t::setFocus(*this);
        return true;
    }

    return tableView.mouseClick(l, t, g);
}

void Table_t::keyPress(int keyCode, char ch, Graphics& g)
{
    if (tfFindFocus)
    {
        string oldValue = tfFind.getValue();
        tfFind.keyPress(keyCode, ch, g);
        tfFind.draw(g);
        string newValue = tfFind.getValue();

        if (backUpRows.empty())
            return;

        if (oldValue != newValue)
        {
            //reset
            rows = backUpRows;

            if (newValue == "")
            {
                updateTableViewData();

            }
            else
            {
                Vector<Vector<string> > newData;

                for (int i = 0; i < rows.size(); ++i)
                {
                    if (newValue.size() > rows[i][findOptionColumnIndex].size())
                        continue;
                    if (rows[i][findOptionColumnIndex].substr(0, newValue.size()) == newValue)
                        newData.push_back(rows[i]);
                }

                rows = newData;
                updateTableViewData();
            }

            tableView.setDrawAllLinePort(true);
            tableView.draw(g);
        }
        return;
    }

    tableView.keyPress(keyCode, ch, g);
}

string Table_t::outputText()
{
    Vector<size_t> maxWidEachColumn = columnWidth;

    for (int i = 0; i < nColumn; ++i)
    {
        for (int j = 0; j < rows.size(); ++j)
            maxWidEachColumn[i] = std::max(maxWidEachColumn[i], rows[j][i].size());
    }

    //top line
    stringstream line;
    line << '+';
    for (int i = 0; i < nColumn; ++i)
    {
        line << string(maxWidEachColumn[i], '-') << '+';
    }
    line << '\n';

    string frameLine = line.str();

    //headers
    line << '|';
    for (int i = 0; i < nColumn; ++i)
    {
        string text = headers[i];
        size_t remaining = maxWidEachColumn[i] - text.size();
        size_t half = remaining / 2;
        line << string(half, ' ') << text << string(remaining - half, ' ') << '|';
    }
    line << '\n';

    //middle line
    line << frameLine;

    //rows
    for (int id = 0; id < rows.size(); ++id)
    {
        line << '|';
        for (int i = 0; i < nColumn; ++i)
        {
            string text = rows[id][i];
            size_t remaining = maxWidEachColumn[i] - text.size();
            size_t half = remaining / 2;
            line << string(half, ' ') << text << string(remaining - half, ' ') << '|';
        }
        line << '\n';
    }

    line << frameLine;

    return line.str();
}

void Table_t::sort(int columnId, int cmpOption, int from, int to)
{
    if (rows.empty())
        return;

    int i = from;
    int j = to;
    Vector<string> key = rows[(from + to) / 2];
    while (i < j)
    {
        while (i < to && (cmp(rows[i][columnId], key[columnId]) == cmpOption))
            ++i;
        while (j > from && (cmp(rows[j][columnId], key[columnId]) == -cmpOption))
            --j;
        if (i <= j)
        {
            Vector<string> tmp = rows[i];
            rows[i] = rows[j];
            rows[j] = tmp;
            ++i;
            --j;
        }
    }
    if (from < j)
        sort(columnId, cmpOption, from, j);
    if (i < to)
        sort(columnId, cmpOption, i, to);
}

void Table_t::sort(int columnId)
{
    if (rows.empty())
        return;

    if (sortingSignList[columnId]->state == 1)
    {
        //dec.
        sort(columnId, 1, 0, rows.size() - 1);
    }
    else
    {
        //inc.
        sort(columnId, -1, 0, rows.size() - 1);
    }
}

void Table_t::setSortingSignCoor()
{
    int nextLeft = left;
    for (int i = 0; i < nColumn; ++i)
    {
        nextLeft += 1 + columnWidth[i];
        sortingSignList[i]->setCoor(nextLeft - 2, top + 2);
    }
}

void Table_t::updateTableViewData()
{
    Vector<string> data;
    for (int i = 0; i < rows.size(); ++i)
    {
        data.push_back(getRows(i));
    }
    tableView.setData(data);
}

bool Table_t::isFocus()
{
    return tableView.isFocus();
}
