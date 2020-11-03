//#include "ViewPort_t.h"
//
//ViewPort_t::ViewPort_t()
//: Rect_t(),
//  items(NULL),
//  preSelectedIdBeforeKeyPress(-1),
//  selectedItemId(-1),
//  preSelectedItemId(-1),
//  currTopItemId(-1),
//  currBottomItemId(-1),
//  drawAllRow(true),
//  firstDraw(true) {
//
//  setDefaultColor();
//}
//
//ViewPort_t::ViewPort_t(int l, int t, int width, int lineNumber, Vector<Label_t*> *_items)
//: Rect_t(l, t, {width + 1, lineNumber + 1}, ""),
//  items(_items),
//  drawAllRow(true),
//  firstDraw(true) {
//
//  setBorder(BorderFactory::SINGLE);
//  setDefaultColor();
//}
//
//ViewPort_t::ViewPort_t(int borderType, int l, int t, int width, int lineNumber, Vector<Label_t*> *_items)
//: Rect_t(borderType, l, t, {width + 1, lineNumber + 1}, ""),
//  items(_items),
//  drawAllRow(true),
//  firstDraw(true) {
//
//  setDefaultColor();
//}
//
//void ViewPort_t::resetFlag() {
//  preSelectedIdBeforeKeyPress = -1;
//  selectedItemId = -1;
//  preSelectedItemId = -1;
//  currTopItemId = 0;
//	currBottomItemId = cord.Y - 2;
//}
//
//void ViewPort_t::setFirstSelected() {
//  preSelectedIdBeforeKeyPress = preSelectedItemId = selectedItemId = 0;
//}
//
//void ViewPort_t::setDefaultColor() {
//  setColor(0xf0);
//  setColorEle(0x79, 0xf9);
//}
//
//void ViewPort_t::setCoor(int l, int t) {
//  Rect_t::setCoor(l, t, cord);
//}
//
//void ViewPort_t::setColorEle(uint8_t focusAttr, uint8_t unFocusAttr) {
//  focusEleAttr = focusAttr;
//  unFocusEleAttr = unFocusAttr;
//}
//
//int ViewPort_t::getSelectedIndex() {
//  return preSelectedIdBeforeKeyPress;
//}
//
//void ViewPort_t::draw(Graphics& g) {
//  if (! canDraw) {
//    canDraw = true;
//    return;
//  }
//
//  if (closeFlag == true) {
//    hide(g);
//    return;
//  }
//
//  if (firstDraw) {
//    g.setColor(attribute);
//    border->draw(left, top, cord, g);
//    firstDraw = false;
//  }
//
//  if (selectedItemId == -1) {
//    preSelectedItemId = selectedItemId = 0;
//    (*items)[selectedItemId]->setColor(focusEleAttr);
//  }
//
//  if (drawAllRow) {
//    Rect_t *ele;
//    for (int i = currTopItemId; i <= currBottomItemId; ++i) {
//      ele = (*items)[i];
//      ele->setCoor(left + 1, top + 1 + i - currTopItemId);
//      ele->draw(g);
//      g.setColor(ele->getAttribute());
//      g.write(left + 1 + ele->getCord().X, top + 1 + i - currTopItemId, string(cord.X - ele->getCord().X, ' '));
//    }
//  } else {
//    Rect_t *ele = (*items)[preSelectedItemId];
//    ele->setCoor(left + 1, top + 1 + preSelectedItemId - currTopItemId);
//    ele->draw(g);
//    g.setColor(ele->getAttribute());
//    g.write(left + 1 + ele->getCord().X, top + 1 + preSelectedItemId - currTopItemId, string(cord.X - ele->getCord().X, ' '));
//
//    ele = (*items)[selectedItemId];
//    ele->setCoor(left + 1, top + 1 + selectedItemId - currTopItemId);
//    ele->draw(g);
//    g.setColor(ele->getAttribute());
//    g.write(left + 1 + ele->getCord().X, top + 1 + selectedItemId - currTopItemId, string(cord.X - ele->getCord().X, ' '));
//
//    drawAllRow = true;
//  }
//}
//
//bool ViewPort_t::mouseClick(int l, int t, Graphics &g) {
//  if (g.isInside(l, t, left + 1, top + 1, cord.X - 1, cord.Y - 2)) {
//    for (int i = 0; i < cord.Y - 1; ++i) {
//      if (t == top + 1 + i) {
//        if (selectedItemId != -1) {
//          (*items)[selectedItemId]->setColor(unFocusEleAttr);
//          (*items)[selectedItemId]->setIsChecked(false);
//        }
//
//        preSelectedIdBeforeKeyPress = selectedItemId = currTopItemId + i;
//
//        (*items)[selectedItemId]->setColor(focusEleAttr);
//        (*items)[selectedItemId]->setIsChecked(true);
//        return true;
//      }
//    }
//  }
//  return false;
//}
//
//bool ViewPort_t::mouseScroll(int state, Graphics& g) {
//  if (state == 1) {
//    keyPress(KEY_UP, ' ', g);
//  } else {
//    keyPress(KEY_DOWN, ' ', g);
//  }
//  Rect_t::setFocus(*this);
//  return true;
//}
//
//void ViewPort_t::keyPress(int keyCode, char ch, Graphics& g) {
//  switch(keyCode) {
//    case KEY_UP: {
//      if (selectedItemId == 0) {
//        canDraw = false;
//        break;
//      }
//      (*items)[selectedItemId]->setColor(unFocusEleAttr);
//      --selectedItemId;
//      (*items)[selectedItemId]->setColor(focusEleAttr);
//
//      if (selectedItemId < currTopItemId) {
//        currTopItemId = selectedItemId;
//        --currBottomItemId;
//      } else {
//        preSelectedItemId = selectedItemId + 1;
//        drawAllRow = false;
//      }
//      break;
//    }
//    case KEY_DOWN: {
//      if (selectedItemId == items->size() - 1) {
//        canDraw = false;
//        break;
//      }
//      (*items)[selectedItemId]->setColor(unFocusEleAttr);
//      ++selectedItemId;
//      (*items)[selectedItemId]->setColor(focusEleAttr);
//
//      if (selectedItemId > currBottomItemId) {
//        currBottomItemId = selectedItemId;
//        ++currTopItemId;
//      } else {
//        preSelectedItemId = selectedItemId - 1;
//        drawAllRow = false;
//      }
//      break;
//    }
//    case KEY_ENTER: {
//      (*items)[selectedItemId]->keyPress(keyCode, ch, g);
//      preSelectedIdBeforeKeyPress = selectedItemId;
//      canDraw = false;
//      break;
//    }
//  }
//}
