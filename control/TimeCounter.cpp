#include "TimeCounter.h"

TimeCounter_t::TimeCounter_t()
: Rect_t(),
  hour(0),
  minute(0),
  second(0),
  totalSecond(0),
  remainingSecond(0) {

  attribute = tenSecAttr;
}
//
TimeCounter_t::TimeCounter_t(int l, int t)
: Rect_t(BorderFactory::SINGLE, l, t, ""),
  hour(0),
  minute(0),
  second(0),
  totalSecond(0),
  remainingSecond(0) {

  attribute = tenSecAttr;
  lbTime = Label_t(BorderFactory::SINGLE, left, top, 2, "");
}

void TimeCounter_t::setInitTime(int h, int m, int s) {
  hour = h;
  minute = m;
  second = s;
  remainingSecond = totalSecond = hour*60*60 + minute*60 + second;
}

void TimeCounter_t::countDown(Graphics& g, Rect_t *currentActivePanel) {
  if (getPanelContainer() != currentActivePanel) return;

  if (!canDrawFlag) return;

  if (remainingSecond > 0) {
    if (second > 0) --second;
    else {
      if (minute > 0) { --minute; second = 59; }
      else {
        if (hour > 0) { --hour; minute = 59; second = 59; }
      }
    }

    --remainingSecond;
    if (remainingSecond > totalSecond / 2) {
      attribute = halfTimeAttr;
    } else if (remainingSecond > 200) {
      attribute = twoThreeTimeAttr;
    } else {
      attribute = tenSecAttr;
    }
  } else {
    canDrawFlag = false;
    getPanelContainer()->getActionTimeEnding();
  }

  lbTime.setColor(this->attribute);

  string hourStr = StringUtility::to_string(hour);
  if (hourStr.size() == 1) hourStr = "0" + hourStr;

  string minStr = StringUtility::to_string(minute);
  if (minStr.size() == 1) minStr = "0" + minStr;

  string secStr = StringUtility::to_string(second);
  if (secStr.size() == 1) secStr = "0" + secStr;

  lbTime.setValue(hourStr + " : " + minStr + " : " + secStr);

  lbTime.draw(g);
}

//===========================================================================================================================
GlobalTimeCounter_t *GlobalTimeCounter_t::globalTimeCounter = new GlobalTimeCounter_t;

void GlobalTimeCounter_t::add(TimeCounter_t *newTimeCounter) {
  allClocks.push_back(newTimeCounter);
}

void GlobalTimeCounter_t::countDown(Graphics& g, Rect_t *currentActivePanel) {
  if (allClocks.empty()) return;

  ++counter;
  if (counter == 30000) {
    for (int i = 0; i < allClocks.size(); ++i) {
      allClocks[i]->countDown(g, currentActivePanel);
    }
    counter = 0;
  }
}
