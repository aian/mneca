/*-
 * This file is part of mneca project. For license details, see the file
 * 'LICENSE' in this package.
 */
#include "Local.h"

namespace mneca {

Widget::Widget(INT nID, Widget* pParent)
  : m_children{},
    m_pParent(pParent),
    m_rcPos(),
    m_nID(nID)
{ }

Widget::~Widget() {
  for(auto pChildWnd : m_children) {
    delete pChildWnd;
  }
  m_children.clear();
}

Widget*
Widget::GetParent() noexcept {
  return this->m_pParent;
}

VOID
Widget::AddChild(Widget* pChild) {
  if (!pChild) {
    throw std::runtime_error("Invalid child widget");
  }
  this->m_children.push_back(pChild);
}

Widget*
Widget::GetChild(INT nID) {
  Widget* pRetWnd = nullptr;

  for(auto pChildWnd : this->m_children) {
    const INT nWndID = pChildWnd->GetID();
    if(nWndID == nID) {
      pRetWnd = pChildWnd;
      break;
    }
  }

  return pRetWnd;
}

VOID
Widget::SetRect(const Rect& rect) {
  this->m_rcPos = rect;
}

Rect
Widget::GetRect() const noexcept {
  return m_rcPos;
}

INT
Widget::GetPosX1() const noexcept {
  return m_rcPos.GetPosX1();
}

INT
Widget::GetPosY1() const noexcept {
  return m_rcPos.GetPosY1();
}

INT
Widget::GetPosX2() const noexcept {
  return m_rcPos.GetPosX2();
}

INT
Widget::GetPosY2() const noexcept {
  return m_rcPos.GetPosY2();
}

INT
Widget::GetWidth() const noexcept {
  return m_rcPos.GetWidth();
}

INT
Widget::GetHeight() const noexcept {
  return m_rcPos.GetHeight();
}

INT
Widget::GetID() const noexcept {
  return m_nID;
}

LRESULT
Widget::Dispatch(UINT nMsg, WPARAM wParam, LPARAM lParam) {
  LRESULT res = 0;

  res = this->WndProc(nMsg, wParam, lParam);
  for(auto pChildWnd : m_children) {
    // We discard the return values of children ...
    (void)pChildWnd->WndProc(nMsg, wParam, lParam);
  }

  return res;
}

LRESULT CALLBACK
Widget::WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam) {
  static Window* pWnd = nullptr;

  if(nMsg == WM_CREATE) {
    CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
    pWnd = static_cast<Window*>(cs->lpCreateParams);
  }
  else {
    if(pWnd) {
      return pWnd->Dispatch(nMsg, wParam, lParam);
    }
  }

  return ::DefWindowProc(hWnd, nMsg, wParam, lParam);
}

}
