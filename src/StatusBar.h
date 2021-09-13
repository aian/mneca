/*-
 * This file is part of mneca project. For license details, see the file
 * 'LICENSE' in this package.
 */
#ifndef MNECA_STATUS_BAR
#define MNECA_STATUS_BAR

#pragma once

namespace mneca {

class StatusBar final : public Window {
public:
  StatusBar() = delete;
  StatusBar(Window* pParentWnd);
  StatusBar(const StatusBar& other) = delete;
  StatusBar(StatusBar&& other) = delete;
  ~StatusBar();
public:
  StatusBar& operator=(const StatusBar& rhs) = delete;
  StatusBar& operator=(StatusBar&& rhs) = delete;
public:
  VOID OnSize(UINT nWidth, UINT nHeight);
protected:
  LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

}


#endif  // MNECA_STATUS_BAR
