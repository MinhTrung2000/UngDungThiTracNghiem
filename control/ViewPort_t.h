//#ifndef VIEWPORT_T_H
//#define VIEWPORT_T_H
//
//#include <string>
//#include <stdint.h>
//
//#include "Label_t.h"
//#include "lib_vector.h"
//#include "lib_utility.h"
//
//using namespace std;
//
//class ViewPort_t : public Rect_t {
//protected:
////  Vector<Label_t *> *items;
//
////	int currentIndex;
////	int previousIndex;
//	int currentTopIndex;
//	int currentBottomIndex;
//
////  bool drawAllRowFlag;
////  bool firstDrawFlag;
//
//  //elements color focus, unfocus
////  uint8_t focusEleAttr;
////  uint8_t unFocusEleAttr;
//
//  virtual void hide(Graphics& g) {}
//
//  friend class CheckList_t;
//  friend class RadioBox_t;
//  friend class TextBox_t;
//
//public:
//	ViewPort_t();
//	ViewPort_t(int l, int t, int width, int lineNumber, Vector<Label_t *> *_items);
//	ViewPort_t(int borderType, int l, int t, int width, int lineNumber, Vector<Label_t *> *_items);
//
//	virtual void reset();
//
//	int getSelectedIndex();
//
//	virtual void setFirstSelected();
//
//	virtual void setDefaultColor();
//
//	virtual void setColorEle(uint8_t focusAttr, uint8_t unFocusAttr);
//
//	virtual void setCoor(int l, int t);
//
//  virtual void draw(Graphics& g);
//
//	virtual bool mouseClick(int l, int t, Graphics& g);
//
//	virtual bool mouseScroll(int state, Graphics& g);
//
//	virtual void keyPress(int keyCode, char ch, Graphics& g);
//
//	ViewPort_t& operator = (const ViewPort_t& o) {
//	  Rect_t::operator=(o);
//    items = o.items;
////    preSelectedIdBeforeKeyPress = o.preSelectedIdBeforeKeyPress;
////    selectedItemId = o.selectedItemId;
////    preSelectedItemId = o.preSelectedItemId;
////    currTopItemId = o.currTopItemId;
////    currBottomItemId = o.currBottomItemId;
//
//    focusEleAttr = o.focusEleAttr;
//    unFocusEleAttr = o.unFocusEleAttr;
//    return *this;
//	}
//};
//
//#endif
