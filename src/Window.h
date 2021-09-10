#ifndef MNECA_WINDOW_H
#define MNECA_WINDOW_H

#pragma once

#include <memory>
#include <vector>

#include <windows.h>

namespace mneca {

class Window : public Widget {
public:
  Window() = delete;
  Window(INT nID, Widget* pParent);
  Window(const Window& other) = delete;
  Window(Window&& other) noexcept = delete;
  virtual ~Window();
public:
  Window& operator=(const Window& rhs) = delete;
  Window& operator=(Window&& rhs) noexcept = delete;
public:
  HWND GetWindowHandle() noexcept;
  HWND GetWindowHandle() const noexcept;
  VOID SetWindowHandle(HWND hWnd) noexcept;
protected:
  VOID SetClientRect();
public:
  virtual RECT GetOffset() const noexcept;
  virtual VOID SetOffset(const RECT& rcOffset) noexcept;
public:
  LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override = 0;
protected:
  virtual VOID UpdateOffset();
private:
  HWND m_hWnd;
  RECT m_rcOffset;
};

}

#endif  // MNECA_WINDOW_H
