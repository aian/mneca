#include "Local.h"

namespace mneca {

class AppWindow::Impl final : public Window {
public:
  Impl() = delete;
  Impl(Window* pParentWnd);
  Impl(const Impl& other) = delete;
  Impl(const Impl&& other) = delete;
  virtual ~Impl();
public:
  Impl& operator=(const Impl& rhs) = delete;
  Impl& operator=(const Impl&& rhs) = delete;
protected:
  LRESULT WndProc(UINT msg, WPARAM wp, LPARAM lp) override;
protected:
  VOID OnSize(UINT fState, INT nWidth, INT nHeight);
  VOID OnMove(INT nPosX, INT nPosY);
  VOID OnDestroy();
private:
  Storage storage_;
};

AppWindow::Impl::Impl(Window* parent) : Window(IDR_APPWINDOW, parent) {
  constexpr wchar_t CLASS_NAME[] = L"mneca";
  Param* param = nullptr;

  {
    param = GetParam();
  }
  {
    WNDCLASSEX wc ={ 0 };

    wc.cbSize        = sizeof(wc);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = Widget::WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetInstance();
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND);
    wc.lpszMenuName  = nullptr;
    wc.lpszClassName = CLASS_NAME;
    wc.hIconSm       = nullptr;

    RegisterClassEx(&wc);
  }
  {
    HWND hwnd = nullptr;
    
    const DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_EX_COMPOSITED;
    const DWORD dwExStyle = 0;

    const INT nWidth  = param->GetWindowWidth();
    const INT nHeight = param->GetWindowHeight();
    const INT nPosX   = param->GetWindowPosX();
    const INT nPosY   = param->GetWindowPosY();

    hwnd = CreateWindowEx(
      dwExStyle,                     // extended window style
      CLASS_NAME,                    // window class
      CLASS_NAME,                    // window text
      dwStyle,                       // window style
      nPosX, nPosY,                  // poisition
      nWidth, nHeight,               // size
      nullptr,                       // parent window
      nullptr,                       // menu
      mneca::GetInstance(),          // instance handle
      this                           // application data
    );
    if(hwnd == nullptr) {
      throw std::runtime_error("Failed to create an application window");
    }

    this->SetClientRect();
    this->SetWindowHandle(hwnd);
  }
  {
    this->AddChild(new StatusBar(this));
    this->AddChild(new Ribbon(this));
//  this->AddChild(new ListWindow(this));
    this->AddChild(new Splitter(this));
  }
  {
    ::ShowWindow(this->GetWindowHandle(), param->GetWindowState());
  }
  {
    // Set Writable flag
    param->SetWritable();
  }
}

AppWindow::Impl::~Impl() { }

VOID
AppWindow::Impl::OnSize(UINT fState, INT nWidth, INT nHeight) {
  Param* param = GetParam();

  UNREFERENCED_PARAMETER(nWidth);
  UNREFERENCED_PARAMETER(nHeight);

  this->SetClientRect();

  switch (fState) {
  case SIZE_RESTORED:
  {
    if(param->IsWritable()) {
      RECT rcWnd = { 0 };

      ::GetWindowRect(this->GetWindowHandle(), &rcWnd);

      param->SetWindowWidth(rcWnd.right - rcWnd.left);
      param->SetWindowHeight(rcWnd.bottom - rcWnd.top);
      param->SetWindowMaximized(FALSE);
    }
    break;
  }
  case SIZE_MINIMIZED:
    if(param->IsWritable()) {
      param->SetWindowMaximized(FALSE);
    }
    break;
  case SIZE_MAXIMIZED:
    if(param->IsWritable()) {
      param->SetWindowMaximized(TRUE);
    }
    break;
  case SIZE_MAXSHOW:// fall-through
  case SIZE_MAXHIDE:// fall-through
  default:
    if(param->IsWritable()) {
      param->SetWindowMaximized(FALSE);
    }
    break;
  }
}

VOID
AppWindow::Impl::OnMove(INT nPosX, INT nPosY) {
  Param* param = GetParam();
  RECT rcWnd ={ 0 };

  // We update window positions for params even if the writable-flag of Param
  // object is still FALSE.
  ::GetWindowRect(this->GetWindowHandle(), &rcWnd);

  param->SetWindowPosX(rcWnd.left);
  param->SetWindowPosY(rcWnd.top);
}

VOID
AppWindow::Impl::OnDestroy() {
  ::PostQuitMessage(EXIT_SUCCESS);
}

LRESULT
AppWindow::Impl::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) {
  HWND hWnd = this->GetWindowHandle();

  switch(uMsg) {
  case WM_SIZE:
  {
    const UINT fState = (UINT)wParam;
    const INT nWidth  = (INT)(short)LOWORD(lParam);
    const INT nHeight = (INT)(short)HIWORD(lParam);
    this->OnSize(fState, nWidth, nHeight);
    break;
  }
  case WM_MOVE:
  {
    const INT nPosX = (INT)(short)LOWORD(lParam);
    const INT nPosY = (INT)(short)HIWORD(lParam);
    this->OnMove(nPosX, nPosY);
    break;
  }
  case WM_DESTROY:
  {
    this->OnDestroy();
    break;
  }
  default:
    break;
  }
  return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/* ------------------------------------------------------------------------- */

AppWindow::AppWindow() : impl_(new Impl(nullptr)) { }

AppWindow::~AppWindow() { }

void
AppWindow::Run(INT nCmdShow) {
  BOOL ret = TRUE;
  MSG msg ={ 0 };

  while((ret = ::GetMessage(&msg, nullptr, 0, 0)) != 0) {
    if(ret < 0) {
      throw std::runtime_error("GetMessage() has been failed");
    }
    else {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }
  }
}

}
