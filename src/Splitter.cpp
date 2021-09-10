#include "Local.h"

namespace mneca {

Splitter::Splitter(Window* pParentWnd)
  : Window(IDR_SPLITTER, pParentWnd),
    m_nBorderWidth(2),
    m_nPos(300),
    m_bGrab(FALSE),
    m_pPaneWnd{ nullptr, nullptr }
{
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
      reinterpret_cast<HMENU>(IDR_SPLITTER),
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
  {
    this->m_pPaneWnd[0] = new ListWindow(dynamic_cast<Window*>(this->GetParent()));
    this->m_pPaneWnd[1] = new Pane(this);
  }
}

Splitter::~Splitter() {
  for (auto pPane : this->m_pPaneWnd) {
    if (pPane) {
      delete pPane;
    }
  }
}

VOID
Splitter::AddPane(Window* pWnd, SIZE_T index) {
  if (index < 0 || index >= m_pPaneWnd.size()) {
    assert(false);
    return;
  }
  this->m_pPaneWnd[index]->AddChild(pWnd);
}

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
  Rect rcSeparatorWnd;
  Widget* pParent = this->GetParent();

  this->UpdateRect();

  if (!pParent) {
    return;
  }

  std::array<Rect, 2> rcChild;
  Rect rcThis = this->GetRect();
  const Rect rcParent = static_cast<RECT>(pParent->GetRect());

  rcSeparatorWnd.SetPosX1(this->m_nPos);
  rcSeparatorWnd.SetPosY1(rcThis.GetPosY1());
  rcSeparatorWnd.SetPosX2(this->m_nPos + this->m_nBorderWidth);
  rcSeparatorWnd.SetPosY2(rcThis.GetPosY2());

  rcChild[0].SetPosX1(rcParent.GetPosX1());
  rcChild[0].SetPosY1(rcSeparatorWnd.GetPosY1());
  rcChild[0].SetPosX2(this->m_nPos - 1);
  rcChild[0].SetPosY2(rcSeparatorWnd.GetPosY2());

  rcChild[1].SetPosX1(this->m_nPos + this->m_nBorderWidth + 1);
  rcChild[1].SetPosY1(rcSeparatorWnd.GetPosY1());
  rcChild[1].SetPosX2(rcParent.GetPosX2());
  rcChild[1].SetPosY2(rcSeparatorWnd.GetPosY2());

  {
    const RECT rc = static_cast<RECT>(rcParent);

    ::MoveWindow(
      this->GetWindowHandle(),
      rcSeparatorWnd.GetPosX1(), rcSeparatorWnd.GetPosY1(),
      rcSeparatorWnd.GetWidth(), rcSeparatorWnd.GetHeight(),
      FALSE);
    ::InvalidateRect(this->GetWindowHandle(), &rc, TRUE);
  }
  // Update child panes
  for (SIZE_T i = 0; i < rcChild.size(); i++) {
    const RECT rc = static_cast<RECT>(rcParent);

    m_pPaneWnd[i]->SetRect(rcChild[i]);
    m_pPaneWnd[i]->WndProc(
      WM_SIZE, 0, MAKELPARAM(rcChild[i].GetHeight(), rcChild[i].GetWidth()));
  }
}

VOID
Splitter::OnMouseMove(INT nPosX, INT nPosY, UINT fKeyFlags) {
  Window* pParentWnd = dynamic_cast<Window*>(this->GetParent());
  POINT ptCursor = {};
  RECT rcClient ={};
  RECT rcHitRegion ={};

  ::GetCursorPos(&ptCursor);
  ::GetClientRect(this->GetWindowHandle(), &rcClient);

  if(!pParentWnd) {
    return;
  }

  if (m_bGrab && (fKeyFlags & MK_LBUTTON)) {
    RECT rcParent = {};
    INT nWidth = 0;
    INT nHeight = 0;

    this->UpdateRect();

    nWidth = this->m_nBorderWidth;
    nHeight = rcClient.bottom - rcClient.top;
    ::MoveWindow(
      this->GetWindowHandle(),
      nPosX, this->GetRect().GetPosY1(),
      nWidth, nHeight, FALSE);

    rcParent = static_cast<RECT>(pParentWnd->GetRect());
    ::InvalidateRect(pParentWnd->GetWindowHandle(), &rcParent, TRUE);
    ::InvalidateRect(this->GetWindowHandle(), &rcClient, TRUE);
    ::SetCursor(LoadCursor(nullptr, IDC_SIZEWE));

    this->m_nPos = nPosX;
    this->OnSize(nWidth, nHeight);
  } else {
    RECT rcWnd = {};

    ::GetWindowRect(this->GetWindowHandle(), &rcWnd);
    if (PtInRect(&rcWnd, ptCursor)) {
      ::SetCursor(LoadCursor(nullptr, IDC_SIZEWE));
    } else {
      ::SetCursor(LoadCursor(nullptr, IDC_ARROW));
    }
  }
}

VOID
Splitter::OnLeftButtonDown(INT nPosX, INT nPosY, UINT fKeyFlags) {
  Window* pParentWnd = dynamic_cast<Window*>(this->GetParent());
  POINT ptCursor ={};
  RECT rcWnd ={};

  ::GetCursorPos(&ptCursor);
  ::GetWindowRect(this->GetWindowHandle(), &rcWnd);

  if(pParentWnd) {
    if (PtInRect(&rcWnd, ptCursor)) {
      this->m_bGrab = TRUE;
      ::SetCursor(LoadCursor(nullptr, IDC_SIZEWE));
      SetCapture(pParentWnd->GetWindowHandle());
    } else {
      ReleaseCapture();
    }
  }
}

VOID
Splitter::OnLeftButtonUp(INT nPosX, INT nPosY, UINT fKeyFlags) {
  this->m_bGrab = FALSE;
  ReleaseCapture();
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
  // Call the WndProc of each panes explicitly.
  for (auto pPane : this->m_pPaneWnd) {
    pPane->WndProc(uMsg, wParam, lParam);
  }

  return static_cast<LRESULT>(0);
}

/* ------------------------------------------------------------------------- */

Pane::Pane(Window* pParentWnd) : Window(IDR_PANE, pParentWnd) {
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
    reinterpret_cast<HMENU>(static_cast<UINT64>(IDR_PANE)),
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

Pane::~Pane() {}

VOID
Pane::OnSize(INT nWidth, INT nHeight) {
  int a = 0;
}

LRESULT
Pane::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
  case WM_SIZE:
    this->OnSize(HIWORD(lParam), LOWORD(lParam));
    break;
  default:
    break;
  }
  for (auto pChild : this->m_children) {
    pChild->WndProc(uMsg, wParam, lParam);
  }
  return static_cast<LRESULT>(0);
}

}
