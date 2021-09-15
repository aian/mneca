/*-
 * This file is part of mneca project. For license details, see the file
 * 'LICENSE' in this package.
 */
#ifndef MNECA_PARAM_H
#define MNECA_PARAM_H

#pragma once

namespace mneca {

class Window;

class Param final { 
private:
  Param();
  Param(const Param& other);
  Param(Param&& other) noexcept;
  ~Param();
public:
  static void Init();
  static Param* GetInstance();
  static void Finalize();
public:
  Param& operator=(const Param& other);
  Param& operator=(Param&& other) noexcept;
public:
  void Swap(Param&& other);
private:
  void Prepare();
public:
  void Read();
  void Write();
public:
  void SetWindowSize(const Window& wnd);
  BOOL IsWritable() const noexcept;
  INT GetWindowWidth() const noexcept;
  INT GetWindowHeight() const noexcept;
  INT GetWindowPosX() const noexcept;
  INT GetWindowPosY() const noexcept;
  BOOL IsWindowMaximized() const noexcept;
  INT GetWindowState() const noexcept;
public:
  VOID SetWritable() noexcept;
  VOID SetWindowWidth(INT nWidth) noexcept;
  VOID SetWindowHeight(INT nHeight) noexcept;
  VOID SetWindowPosX(INT nPosX) noexcept;
  VOID SetWindowPosY(INT nPosY) noexcept;
  VOID SetWindowMaximized(BOOL bIsMaximized) noexcept;
public:
  BOOL CreateConfigFile();
  UINT GetInt(LPCTSTR lpKeyName, INT nDefault) const noexcept;
  BOOL GetBool(LPCTSTR lpKeyName, BOOL nDefault) const noexcept;
public:
  VOID WriteInt(LPCTSTR lpKeyName, INT nValue) const noexcept;
  VOID WriteBool(LPCTSTR lpKeyName, BOOL nValue) const noexcept;
private:
  BOOL m_bWritable;
  int m_nWindowWidth;
  int m_nWindowHeight;
  int m_nWindowPosX;
  int m_nWindowPosY;
  BOOL m_bMaximized;
  std::wstring m_strConfigPath;
private:
  static Param* s_pParam;
  static BOOL s_bInit;
};

}

#endif  // MNECA_PARAM_H
