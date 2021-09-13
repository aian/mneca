/*-
 * This file is part of mneca project. For license details, see the file
 * 'LICENSE' in this package.
 */
#include "Local.h"

namespace mneca {

StatusBar::StatusBar(Window* pParentWnd) : Window(IDR_STATUS_BAR, pParentWnd) {
  HWND hWnd = nullptr;

  hWnd = ::CreateWindowEx(
    0,
    STATUSCLASSNAME,
    nullptr,
    WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
    0, 0, 0, 0,
    pParentWnd->GetWindowHandle(),
    reinterpret_cast<HMENU>(IDR_STATUS_BAR),
    GetInstance(),
    nullptr
  );
  if(hWnd == nullptr) {
    throw std::runtime_error("Failed to create a StatusBar");
  }
  this->SetClientRect();
  this->SetWindowHandle(hWnd);
}

StatusBar::~StatusBar() { }

VOID
StatusBar::OnSize(UINT nWidth, UINT nHeight) {
  Widget* pParentWnd = this->GetParent();

  Rect rcClient;
  RECT rcWnd ={ 0 };
  UINT cx, cy, cw, ch;

  this->SetClientRect();
  this->UpdateOffset();

  ::GetClientRect(this->GetWindowHandle(), &rcWnd);

  // TODO: rcWnd.bottom is to be converted to Rect object
  rcClient = pParentWnd->GetRect();
  cx = 0;
  cy = rcClient.GetPosY2() - (rcWnd.bottom - rcWnd.top);
  cw = nWidth;
  ch = this->GetHeight();

  ::MoveWindow(this->GetWindowHandle(), cx, cy, cw, ch, FALSE);
  ::InvalidateRect(this->GetWindowHandle(), &rcWnd, TRUE);
}

LRESULT
StatusBar::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch(uMsg) {
  case WM_SIZE:
    this->OnSize(LOWORD(lParam), HIWORD(lParam));
    break;
  default:
    // do nothing
    break;
  }
  return static_cast<LRESULT>(0);
}

}
