#ifndef TIMECOUNTER_H_INCLUDED
#define TIMECOUNTER_H_INCLUDED

#include "Label_t.h"
#include "lib_vector.h"

using namespace std;

class TimeCounter_t : public Rect_t
{
private:
    static const uint8_t halfTimeAttr = 0xae;
    static const uint8_t twoThreeTimeAttr = 0xe2;
    static const uint8_t tenSecAttr = 0xce;

    Label_t lbTime;

    int hour;
    int minute;
    int second;
    int totalSecond;
    int remainingSecond;

public:
    TimeCounter_t();
//  TimeCounter_t(int l, int t, int h, int m, int s);
    TimeCounter_t(int l, int t);

    void setInitTime(int h, int m, int s);

    void countDown(Graphics& g, Rect_t *currentActivePanel);

    bool canGetFocus()
    {
        return false;
    }

    TimeCounter_t& operator = (const TimeCounter_t &rhs)
    {
        Rect_t::operator=(rhs);
        hour = rhs.hour;
        minute = rhs.minute;
        second = rhs.second;
        totalSecond = rhs.totalSecond;
        remainingSecond = rhs.remainingSecond;
        lbTime = rhs.lbTime;
        return *this;
    }
};

class GlobalTimeCounter_t
{
private:
    static GlobalTimeCounter_t *globalTimeCounter;

    Vector<TimeCounter_t *> allClocks;

    int counter;

    GlobalTimeCounter_t() : counter(0) {}

public:
    void add(TimeCounter_t *newTimeCounter);

    void countDown(Graphics& g, Rect_t *currentActivePanel);

    static GlobalTimeCounter_t *getGlobalTimeCounter()
    {
        return globalTimeCounter;
    }
};

#endif // TIMECOUNTER_H_INCLUDED
