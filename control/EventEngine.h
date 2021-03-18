#ifndef EVENTENGINE_H
#define EVENTENGINE_H

#include <windows.h>

#include "Rect_t.h"
#include "Panel_t.h"
#include "Graphics.h"

//Event Processing Thread
class EventEngine
{
private:
    static EventEngine *currentPocess;
    static Graphics _graphics;

    static bool stopAllProcessFlag;

protected:
    HANDLE _console;
    DWORD _consoleMode;

    EventEngine *preProcess;
    Panel_t *mainFrame;     //ui form

    bool reRun;

    bool findAndSetFocusedItemInPanel(Panel_t *panel, Rect_t *focused);

    void setFirstFocus(Panel_t *panel);

    bool isPanel(Rect_t* rect);

public:
//	EventEngine(EventEngine *_preProc = NULL, DWORD input = STD_INPUT_HANDLE, DWORD output = STD_OUTPUT_HANDLE);
    EventEngine(EventEngine *_preProc = NULL, DWORD input = STD_INPUT_HANDLE);

    virtual ~EventEngine();

//	void setPreProcess(EventEngine &eng) { preProcess = &eng; }

//	EventEngine *getPreProcess() { return preProcess; }

    void setMainFrame(Panel_t &panel)
    {
        mainFrame = &panel;
    }

    Rect_t *getMainFrame()
    {
        return mainFrame;
    }

    void run(bool clearScreen);

    //=============================
    static void setOnStopAllProcessFlag()
    {
        stopAllProcessFlag = true;
    }

    static EventEngine *getCurrentProcess()
    {
        return currentPocess;
    }

    static void setCurrentProcess(EventEngine *proc)
    {
        currentPocess = proc;
    }
};

#endif
