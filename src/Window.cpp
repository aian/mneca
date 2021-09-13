/*-
 * This file is part of mneca project. For license details, see the file
 * 'LICENSE' in this package.
 */
#include "Local.h"

namespace mneca {

Window::Window(INT nID, Widget* pParent)
  : Widget(nID, pParent), m_hWnd(nullptr), m_rcOffset{ 0 } { }

Window::~Window() { }

HWND
Window::GetWindowHandle() noexcept {
  return m_hWnd;
}

HWND
Window::GetWindowHandle() const noexcept {
  return m_hWnd;
}

VOID
Window::SetWindowHandle(HWND hWnd) noexcept {
  m_hWnd = hWnd;
}

VOID
Window::SetClientRect() {
  RECT rcClient ={ 0 };
  ::GetClientRect(this->GetWindowHandle(), &rcClient);
  this->SetRect(rcClient);
}

RECT
Window::GetOffset() const noexcept {
  return m_rcOffset;
}

VOID
Window::SetOffset(const RECT& rcOffset) noexcept {
  this->m_rcOffset.top    = rcOffset.top;
  this->m_rcOffset.right  = rcOffset.right;
  this->m_rcOffset.bottom = rcOffset.bottom;
  this->m_rcOffset.left   = rcOffset.left;
}

VOID
Window::UpdateOffset() {
  // Do nothing
}

}
