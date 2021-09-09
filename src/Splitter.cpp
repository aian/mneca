#include "Local.h"

namespace mneca {

Splitter::Splitter(INT nID, Direction fDir, Window* pParentWnd)
  : Window(nID, pParentWnd),
    m_nBorderWidth(2),
    m_nPos(100),
    m_fDir(fDir),
    m_bHover(FALSE),
    m_bGrab(FALSE),
    m_pPaneWnd{ nullptr, nullptr }
{
  HWND hWnd = nullptr;

  static const DWORD dwStyle = WS_CHILD | WS_VISIBLE;
  static const DWORD dwExStyle = 0;

  hWnd = CreateWindowEx(
    dwExStyle,
    WC_STATIC,
    0,
    dwStyle,
    0, 0, 0, 0,
    pParentWnd->GetWindowHandle(),
    reinterpret_cast<HMENU>(static_cast<UINT64>(nID)),
    GetInstance(),
    nullptr
  );
  if (!hWnd) {
    DWORD dwError = GetLastError();
    throw std::runtime_error("Failed to create a splitter window");
  }
  this->SetWindowHandle(hWnd);
  this->SetClientRect();

}

Splitter::~Splitter() { }

VOID
Splitter::UpdateRect() {
  Widget* pParentWnd = this->GetParent();
  Widget* pRibbonWnd = nullptr;
  Widget* pStatusBarWnd = nullptr;
  Rect rcRect;
  RECT rcOffset = { 0 };

  pRibbonWnd = pParentWnd->GetChild(IDR_RIBBON);
  if (!pRibbonWnd) {
    throw std::runtime_error("Failed to get the Ribbon window");
  }
  pStatusBarWnd = pParentWnd->GetChild(IDR_STATUS_BAR);
  if (!pStatusBarWnd) {
    throw std::runtime_error("Failed to get the StatusBar window");
  }

  rcOffset.top = pRibbonWnd->GetHeight();
  rcOffset.right = 0;
  rcOffset.bottom = pStatusBarWnd->GetHeight();
  rcOffset.left = 0;

  rcRect = pParentWnd->GetRect();
  rcRect.SetPosY1(rcRect.GetPosY1() + rcOffset.top);
  rcRect.SetPosY2(rcRect.GetPosY2() - rcOffset.bottom);
  this->SetRect(rcRect);
}

VOID
Splitter::OnSize(INT nWidth, INT nHeight) {
  Rect rcWnd;
  Widget* pParent = nullptr;

  this->UpdateRect();

  pParent = this->GetParent();
  if (pParent) {
    Rect rcRect = this->GetRect();
    RECT rcParent = {};

    if (m_fDir == Splitter::Direction::HORIZONTAL) {
      rcWnd.SetPosX1(this->m_nPos);
      rcWnd.SetPosY1(rcRect.GetPosY1());
      rcWnd.SetPosX2(this->m_nPos + this->m_nBorderWidth);
      rcWnd.SetPosY2(rcRect.GetPosY2());
    } else {
      assert(m_fDir == Splitter::Direction::VERTICAL);
      rcWnd.SetPosX1(rcRect.GetPosY1());
      rcWnd.SetPosY1(this->m_nPos);
      rcWnd.SetPosX2(rcRect.GetPosY2());
      rcWnd.SetPosY2(this->m_nPos + this->m_nBorderWidth);
    }
    ::MoveWindow(
      this->GetWindowHandle(),
      rcWnd.GetPosX1(), rcWnd.GetPosY1(), rcWnd.GetWidth(), rcWnd.GetHeight(),
      FALSE);

    rcParent = static_cast<RECT>(pParent->GetRect());
    ::InvalidateRect(this->GetWindowHandle(), &rcParent, TRUE);
  }
}

VOID
Splitter::OnMouseMove(INT nPosX, INT nPosY, UINT fKeyFlags) {
  POINT ptCursor = {};
  HCURSOR hCursor = nullptr;

  ::GetCursorPos(&ptCursor);

  if (this->m_fDir == Direction::HORIZONTAL) {
    hCursor = LoadCursor(nullptr, IDC_SIZEWE);
  } else {
    hCursor = LoadCursor(nullptr, IDC_SIZENS);
  }

  if (m_bGrab) {
    RECT rcClient = {};
    RECT rcParent = {};
    INT nWidth = 0;
    INT nHeight = 0;
    Window* pParent = dynamic_cast<Window*>(this->GetParent());

    this->UpdateRect();

    if (pParent) {
      ::GetClientRect(this->GetWindowHandle(), &rcClient);
      nWidth = this->m_nBorderWidth;
      nHeight = rcClient.bottom - rcClient.top;
      ::MoveWindow(this->GetWindowHandle(), nPosX, this->GetRect().GetPosY1(), nWidth, nHeight, FALSE);

      rcParent = static_cast<RECT>(pParent->GetRect());
      ::InvalidateRect(pParent->GetWindowHandle(), &rcParent, TRUE);
      ::InvalidateRect(this->GetWindowHandle(), &rcClient, TRUE);
      ::SetCursor(hCursor);

      this->m_nPos = nPosX;
    }
  } else {
    RECT rcWnd = {};

    ::GetWindowRect(this->GetWindowHandle(), &rcWnd);
    if (PtInRect(&rcWnd, ptCursor)) {
      ::SetCursor(hCursor);
      m_bHover = TRUE;
    } else {
      ::SetCursor(LoadCursor(nullptr, IDC_ARROW));
      m_bHover = FALSE;
    }
  }
}

VOID
Splitter::OnLeftButtonDown(INT nPosX, INT nPosY, UINT fKeyFlags) {
  if (this->m_bHover) {
    HCURSOR hCursor = nullptr;

    if (this->m_fDir == Direction::HORIZONTAL) {
      hCursor = LoadCursor(nullptr, IDC_SIZEWE);
    } else {
      hCursor = LoadCursor(nullptr, IDC_SIZENS);
    }
    ::SetCursor(hCursor);

    this->m_bGrab = TRUE;
  }
}

VOID
Splitter::OnLeftButtonUp(INT nPosX, INT nPosY, UINT fKeyFlags) {
  this->m_bGrab = FALSE;
  this->OnMouseMove(nPosX, nPosY, fKeyFlags);
}

LRESULT
Splitter::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
  case WM_SIZE:
    OnSize(LOWORD(lParam), HIWORD(lParam));
    break;
  case WM_MOUSEMOVE:
    this->OnMouseMove(
      GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam),
      static_cast<UINT>(static_cast<UINT16>(wParam)));
    break;
  case WM_LBUTTONDOWN:
    this->OnLeftButtonDown(
      GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam),
      static_cast<UINT>(static_cast<UINT16>(wParam)));
    break;
  case WM_LBUTTONUP:
    this->OnLeftButtonUp(
      GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam),
      static_cast<UINT>(static_cast<UINT16>(wParam)));
    break;
  default:
    break;
  }
  return static_cast<LRESULT>(0);
}


}
