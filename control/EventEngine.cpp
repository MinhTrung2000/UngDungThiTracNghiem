#include "EventEngine.h"

#include <windows.h>
#include <conio.h>
#include <cassert>
#include <string>
#include <iostream>

#include "lib_vector.h"

#include "RadioBox_t.h"
#include "Checklist_t.h"
#include "Panel_t.h"
#include "TimeCounter.h"

#include "../view/UIForm.h"

using namespace std;

EventEngine *EventEngine::currentPocess = NULL;
Graphics EventEngine::_graphics = *Graphics::getCurrentActiveGraphics();

bool EventEngine::stopAllProcessFlag = false;

//EventEngine::EventEngine(EventEngine *_preProc, DWORD input, DWORD output)
EventEngine::EventEngine(EventEngine *_preProc, DWORD input)
//	: _console(GetStdHandle(input)), _graphics(*Graphics::getCurrentActiveGraphics()), preProcess(_preProc), focusObjBackUp(NULL) {
    : _console(GetStdHandle(input)), preProcess(_preProc), reRun(false)
{
    GetConsoleMode(_console, &_consoleMode);
    SetConsoleMode(_console, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
}

void EventEngine::run(bool clearScreen)
{
//  if (preProcess != NULL) {
//    preProcess->setFocusObjBackUp(*Rect_t::getFocus());
//  }

    EventEngine::setCurrentProcess(this);

    if (clearScreen == true)
    {
        _graphics.setColor(0xf0);
        _graphics.clearScreen();
    }

//  set 1st child as focused by default
//  if (focusObjBackUp == NULL) {
//    setFirstFocus(mainFrame);

    Rect_t::setFocus(*mainFrame->getCurrentFocus());

//  } else {
//    Rect_t::setFocus(*focusObjBackUp);
//  }

    //debug
    assert(mainFrame != NULL);

    mainFrame->draw(_graphics);

    Rect_t *previousFocus = NULL;

    //debug
//  assert(previousFocus != NULL);

    //safe
    //if (Rect_t::getFocus() == NULL) return;

    bool redraw = true;

    while (1)
    {
        GlobalTimeCounter_t::getGlobalTimeCounter()->countDown(_graphics, mainFrame);

        if (mainFrame->getReRunFlag())
        {
            mainFrame->setReRunFlag(false);
            run(clearScreen);
            return;
        }

        if (stopAllProcessFlag == true)
            return;

        if (mainFrame->getCloseFlag())
        {
            mainFrame->getBuffer(_graphics);
            return;
        }

        if (redraw)
        {
            previousFocus = Rect_t::getFocus();
//			assert(Rect_t::getFocus() != NULL);
//
//			_graphics.setCursorVisibility(Rect_t::getFocus()->IsCursorVisible());
//			_graphics.moveTo(Rect_t::getFocus()->getCurrentPosition().X, Rect_t::getFocus()->getCurrentPosition().Y);
//
//			Rect_t::getFocus()->draw(_graphics);

            assert(previousFocus != NULL);

            _graphics.setCursorVisibility(previousFocus->IsCursorVisible());
            _graphics.moveTo(previousFocus->getCurrentPosition().X, previousFocus->getCurrentPosition().Y);

            previousFocus->draw(_graphics);
            redraw = false;
        }
//
        INPUT_RECORD record;
        DWORD count;
//		ReadConsoleInput(_console, &record, 1, &count);
        if (GetNumberOfConsoleInputEvents(_console, &count) && count > 0)
        {
            ReadConsoleInput(_console, &record, 1, &count);
        }
        else
            continue;

        if (record.EventType == KEY_EVENT)
        {
            if (record.Event.KeyEvent.bKeyDown == true)
            {
                WORD code = record.Event.KeyEvent.wVirtualKeyCode;
                CHAR chr = record.Event.KeyEvent.uChar.AsciiChar;

                // checking if we moved to next object
                if (code == VK_TAB)
                {
                    if (! findAndSetFocusedItemInPanel(mainFrame, previousFocus))
                    {
                        //Cant find the next item can be set focus in current panel
                        //set focus to the first item in this panel
                        setFirstFocus(mainFrame);
                    }
                    Rect_t *f = Rect_t::getFocus();
                    if (previousFocus != f)
                    {
                        previousFocus->releaseFocus(_graphics);
                        previousFocus = f;
                    }
                }
                else
                {
//          previousFocus = Rect_t::getFocus();
                    _graphics.setCursorVisibility(previousFocus->IsCursorVisible());
                    previousFocus->keyPress(code,  chr, _graphics);
                    if (previousFocus->getPanelContainer() != NULL)
                        previousFocus->getPanelContainer()->keyPress(code, chr, _graphics);
//          if (dynamic_cast<UIForm *>(mainFrame) != NULL)
//            mainFrame->keyPress(code, chr, _graphics);
                }
                redraw = true;
            }
        }
        else if (record.EventType == MOUSE_EVENT)
        {
            DWORD button = record.Event.MouseEvent.dwButtonState;
            DWORD evFlag = record.Event.MouseEvent.dwEventFlags;
            COORD coord = record.Event.MouseEvent.dwMousePosition;

            if (evFlag == 4)
            {
                redraw = Rect_t::getFocus()->mouseScroll(((int) button >= 0 ? 1 : 0), _graphics);
            }

            if (button == FROM_LEFT_1ST_BUTTON_PRESSED)
            {
//        _graphics.setCursorVisibility(true);
                previousFocus = Rect_t::getFocus();
                redraw = mainFrame->mouseClick((int) coord.X, (int) coord.Y, _graphics);
                if (redraw && previousFocus != Rect_t::getFocus())
                {
                    previousFocus->releaseFocus(_graphics);
                    previousFocus = Rect_t::getFocus();
                    assert(previousFocus != NULL);
                }
            }
        }
    }
}

EventEngine::~EventEngine()
{
    if (preProcess == NULL)
    {
        SetConsoleMode(_console, _consoleMode);
        _graphics.setColor();
        _graphics.clearScreen();
        _graphics.moveTo(0, 0);
        _graphics.setCursorVisibility(true);
    }
    else
    {
        EventEngine::setCurrentProcess(preProcess);
//    if (dynamic_cast<UIForm *> (currentPocess->mainFrame) != NULL) {
//      currentPocess->reRun = true;
//    } else {
//      Rect_t::setFocus(*(currentPocess->mainFrame->getCurrentFocus()));
//    }
        Rect_t::setFocus(*(currentPocess->mainFrame->getCurrentFocus()));
    }
}

/* this function find the current object(even inside panels) and move the focus to the next object*/
bool EventEngine::findAndSetFocusedItemInPanel(Panel_t *panel, Rect_t *focused)
{
    Vector<Rect_t *> rects;
    panel->getAllRects(rects);

    Rect_t *it = focused;

    //move over all Rects to find the current focused
    for (int i = 0; i < rects.size(); ++i)
    {
        // if it is a panel -> recursive search
        if (isPanel(rects[i]))
        {
            if (findAndSetFocusedItemInPanel(((Panel_t *) rects[i]), focused))
                return true;
        }

        // if we find the current focused
        if (rects[i] == it)
        {
            // checking what is the next Item to set focus
            for (int j = i + 1; j < rects.size(); ++j)
            {
                if (rects[j]->canGetFocus())
                {
                    // if the next Item is panel - set the current focus item inside the panel to be in focus
//					if (isPanel(rects[j])) {
//						Rect_t::setFocus(*((Panel_t *) rects[j])->getCurrentFocus());
//					} else {
                    //else update next focus item to this panel (top of recursive stack)
                    Rect_t::setFocus(*(rects[j]->getCurrentFocus()));
//						panel->setCurrentFocus(j);
                    panel->setCurrentFocus(rects[j]);
//					}
                    return true;
                }
            }
        }
    }
    return false;
}

bool EventEngine::isPanel(Rect_t *rect)
{
//	return (dynamic_cast<Panel_t *> (rect) != NULL);
    return (rect != NULL && dynamic_cast<Panel_t *> (rect) != NULL);
}

void EventEngine::setFirstFocus(Panel_t *panel)
{
    Vector<Rect_t *> items;
    panel->getAllRects(items);
    for (int i = 0; i < items.size(); ++i)
    {
        if (items[i]->canGetFocus())
        {
            Rect_t::setFocus(*(items[i]->getCurrentFocus()));
//      panel->setCurrentFocus(i);
            panel->setCurrentFocus(items[i]);
            return;
        }
    }
    Rect_t::setFocus(*panel);
}
