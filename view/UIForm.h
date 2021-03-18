#ifndef UIFORM_H_INCLUDED
#define UIFORM_H_INCLUDED

#include "../control/EventEngine.h"
#include "../control/Rect_t.h"
#include "../control/Panel_t.h"
#include "../control/MessageBox_t.h"

#include <cassert>

class UIForm : public Panel_t
{
protected:
    bool alreadyInProcessFlag;

    bool reRunFlag;

    friend class MenuBar_t;

public:
    UIForm() : Panel_t(), alreadyInProcessFlag(false), reRunFlag(false) {}

    UIForm(int l, int t, COORD cor)
        : UIForm(BorderFactory::DOUBLE, l, t, cor, "") {}

    UIForm(int borderType, int l, int t, COORD cor)
        : UIForm(borderType, l, t, cor, "") {}

    UIForm(int borderType, int l, int t, COORD cor, const string& title)
        : Panel_t(borderType, l, t, cor, title),
          alreadyInProcessFlag(false),
          reRunFlag(false)
    {

        setBuffer();
        setColor(0x0f);
    }

    virtual ~UIForm() {}

    bool getReRunFlag()
    {
        return reRunFlag;
    }
    void setReRunFlag(bool flag)
    {
        reRunFlag = flag;
    }

    static void drawForm(UIForm *form, bool clearScreenFlag = true)
    {
        EventEngine process(EventEngine::getCurrentProcess());
        process.setMainFrame(*form);
        process.run(clearScreenFlag);
        delete form;
    }

    UIForm& operator = (const UIForm& rhs)
    {
        Panel_t::operator=(rhs);
        alreadyInProcessFlag = rhs.alreadyInProcessFlag;
        reRunFlag = rhs.reRunFlag;
        return *this;
    }
};


#endif // UIFORM_H_INCLUDED
