/*-
 * This file is part of mneca project. For license details, see the file
 * 'LICENSE' in this package.
 */
#include "Local.h"

#define PARAM_MAXPATH 2048

#define APPNAME L"mneca"
#define CONFIG_FNAME L"mneca"
#define CONFIG_EXT L".ini"

#define PARAM_WIDTH L"Width"
#define DEFAULT_WIDTH  640

#define PARAM_HEIGHT L"Height"
#define DEFAULT_HEIGHT 480

#define PARAM_POS_X L"PosX"
#define DEFAULT_POS_X CW_USEDEFAULT

#define PARAM_POS_Y L"PosY"
#define DEFAULT_POS_Y CW_USEDEFAULT

#define PARAM_MAXIMIZED L"Maximized"
#define DEFAULT_MAXIMIZED 0

namespace mneca {

Param* Param::s_pParam = nullptr;
BOOL Param::s_bInit = FALSE;

static VOID
SetDefaultConfigPath(std::wstring& path) {
  DWORD dwRet = 0;
  WCHAR lpBuffer[PARAM_MAXPATH] = { 0 };

  WCHAR lpDrive[_MAX_DRIVE]   = { 0 };
  WCHAR lpDir[PARAM_MAXPATH]  = { 0 };
  WCHAR lpFile[PARAM_MAXPATH] = { 0 };
  WCHAR lpExt[_MAX_EXT]       = { 0 };

  dwRet = GetModuleFileName(nullptr, lpBuffer, PARAM_MAXPATH);
  if (dwRet <= 0) {
    throw std::runtime_error("Insufficient buffer size for config file path");
  }
  _wsplitpath_s(lpBuffer, lpDrive, lpDir, lpFile, lpExt);
  _wmakepath_s(lpBuffer, lpDrive, lpDir, CONFIG_FNAME, CONFIG_EXT);

  path = lpBuffer;
}

Param::Param()
  : m_bWritable(FALSE),
    m_nWindowWidth(DEFAULT_WIDTH),
    m_nWindowHeight(DEFAULT_HEIGHT),
    m_nWindowPosX(DEFAULT_POS_X),
    m_nWindowPosY(DEFAULT_POS_Y),
    m_bMaximized(DEFAULT_MAXIMIZED)
{
  SetDefaultConfigPath(this->m_strConfigPath);
  this->Prepare();
}

Param::Param(const Param& other)
  : m_bWritable(other.m_bWritable),
    m_nWindowWidth(other.m_nWindowWidth),
    m_nWindowHeight(other.m_nWindowHeight),
    m_nWindowPosX(other.m_nWindowPosX),
    m_nWindowPosY(other.m_nWindowPosY),
    m_bMaximized(other.m_bMaximized)
{ }

Param::Param(Param&& other) noexcept
  : Param()
{
  this->Swap(std::move(other));
}

Param::~Param() {
}

Param&
Param::operator=(const Param& other) {
  if (this != &other) {
    this->Swap(Param(other));
  }
  return *this;
}

Param&
Param::operator=(Param&& other) noexcept {
  this->Swap(std::move(other));
  return *this;
}

void
Param::Init() {
  if(!Param::s_pParam) {
    Param::s_pParam = new Param();
    assert(Param::s_pParam);
  }
  Param::s_bInit = TRUE;
}

Param*
Param::GetInstance() {
  if (!Param::s_bInit) {
    if (!Param::s_pParam) {
      Param::s_pParam = new Param();
    }
  }
  assert(Param::s_pParam);
  return Param::s_pParam;
}

void
Param::Finalize() {
  if (Param::s_pParam) {
    Param::s_pParam->Write();

    delete Param::s_pParam;
    Param::s_pParam = nullptr;
  }
}

void
Param::Swap(Param&& other) {
  std::swap(m_bWritable, other.m_bWritable);
  std::swap(m_nWindowWidth, other.m_nWindowWidth);
  std::swap(m_nWindowHeight, other.m_nWindowHeight);
  std::swap(m_nWindowPosX, other.m_nWindowPosX);
  std::swap(m_nWindowPosY, other.m_nWindowPosY);
  std::swap(m_bMaximized, other.m_bMaximized);
}

BOOL
Param::CreateConfigFile() {
  BOOL bRet = FALSE;

  if (!::PathFileExists(this->m_strConfigPath.c_str())) {
    errno_t err = 0;
    FILE* fp = nullptr;

    static const BYTE uBOM[2] ={ 0xFF, 0xFE };

    err = _wfopen_s(&fp, this->m_strConfigPath.c_str(), L"wb");
    if (err != 0 || !fp) {
      throw std::runtime_error("Failed to open the config file");
    }
    fwrite(uBOM, sizeof(BYTE), _countof(uBOM), fp);
    fclose(fp);

    bRet = TRUE;
  }

  return bRet;
}

UINT
Param::GetInt(LPCTSTR lpKeyName, INT nDefault) const noexcept {
  return ::GetPrivateProfileInt(
    APPNAME, lpKeyName, nDefault, this->m_strConfigPath.c_str());
}

BOOL
Param::GetBool(LPCTSTR lpKeyName, BOOL nDefault) const noexcept {
  return ::GetPrivateProfileInt(
    APPNAME, lpKeyName, (INT)nDefault, this->m_strConfigPath.c_str())
    ? TRUE : FALSE;
}

VOID
Param::WriteInt(LPCTSTR lpKeyName, INT nValue) const noexcept {
  std::wstringstream ss;
  ss << nValue;
  WritePrivateProfileString(
    APPNAME, lpKeyName, ss.str().c_str(), this->m_strConfigPath.c_str());
}

VOID
Param::WriteBool(LPCTSTR lpKeyName, BOOL nValue) const noexcept {
  std::wstringstream ss;
  ss << (nValue ? 1 : 0);
  WritePrivateProfileString(
    APPNAME, lpKeyName, ss.str().c_str(), this->m_strConfigPath.c_str());
}

void
Param::Prepare() {
  BOOL bCreated = FALSE;

  bCreated = this->CreateConfigFile();
  if (bCreated) {
    this->Write();
  } else {
    this->Read();
  }
}

void
Param::Read() {
  this->m_nWindowWidth  = this->GetInt(PARAM_WIDTH,  DEFAULT_WIDTH);
  this->m_nWindowHeight = this->GetInt(PARAM_HEIGHT, DEFAULT_HEIGHT);
  this->m_nWindowPosX   = this->GetInt(PARAM_POS_X, DEFAULT_POS_X);
  this->m_nWindowPosY   = this->GetInt(PARAM_POS_Y, DEFAULT_POS_Y);
  this->m_bMaximized    = this->GetBool(PARAM_MAXIMIZED, DEFAULT_MAXIMIZED);
}

void
Param::Write() {
  this->CreateConfigFile();
  this->WriteInt(PARAM_WIDTH,  this->m_nWindowWidth);
  this->WriteInt(PARAM_HEIGHT, this->m_nWindowHeight);
  this->WriteInt(PARAM_POS_X, this->m_nWindowPosX);
  this->WriteInt(PARAM_POS_Y, this->m_nWindowPosY);
  this->WriteBool(PARAM_MAXIMIZED, this->m_bMaximized);
}

void
Param::SetWindowSize(const Window& wnd) {
  RECT window_rect = { 0 };

  ::GetWindowRect(wnd.GetWindowHandle(), &window_rect);

  this->m_nWindowWidth = window_rect.right - window_rect.left;
  this->m_nWindowHeight = window_rect.bottom - window_rect.top;
}

BOOL
Param::IsWritable() const noexcept {
  return this->m_bWritable;
}

INT
Param::GetWindowWidth() const noexcept {
  return this->m_nWindowWidth;
}

INT
Param::GetWindowHeight() const noexcept {
  return this->m_nWindowHeight;
}

INT
Param::GetWindowPosX() const noexcept {
  return this->m_nWindowPosX;
}

INT
Param::GetWindowPosY() const noexcept {
  return this->m_nWindowPosY;
}

BOOL
Param::IsWindowMaximized() const noexcept {
  return this->m_bMaximized;
}

INT
Param::GetWindowState() const noexcept {
  return this->IsWindowMaximized() ? SW_MAXIMIZE : SW_SHOW;
}

VOID
Param::SetWritable() noexcept {
  this->m_bWritable = TRUE;
}

VOID
Param::SetWindowWidth(INT nWidth) noexcept {
  this->m_nWindowWidth = nWidth;
}

VOID
Param::SetWindowHeight(INT nHeight) noexcept {
  this->m_nWindowHeight = nHeight;
}

VOID
Param::SetWindowPosX(INT nPosX) noexcept {
  this->m_nWindowPosX = nPosX;
}

VOID
Param::SetWindowPosY(INT nPosY) noexcept {
  this->m_nWindowPosY = nPosY;
}

VOID
Param::SetWindowMaximized(BOOL bIsMaximized) noexcept {
  this->m_bMaximized = bIsMaximized;
}

}
