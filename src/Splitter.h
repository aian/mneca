#ifndef MNECA_SPLITTER_H
#define MNECA_SPLITTER_H

#pragma once

namespace mneca {

class Splitter : public Window {
public:
  enum class Direction {
    VERTICAL   = 0,
    HORIZONTAL = 1,
  };
public:
  Splitter() = delete;
  Splitter(INT nID, Direction fDir, Window* pParentWnd);
  Splitter(const Splitter& other) = delete;
  Splitter(Splitter&& other) noexcept = delete;
  virtual ~Splitter();
public:
  Splitter& operator=(const Splitter& rhs) = delete;
  Splitter& operator=(Splitter&& rhs) noexcept = delete;
public:
  VOID AddPane(Window* pWnd, SIZE_T index);
protected:
  LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
private:
  VOID UpdateRect();
  VOID OnSize(INT nWidth, INT nHeight);
  VOID OnMouseMove(INT nPosX, INT nPosY, UINT fKeyFlags);
  VOID OnLeftButtonDown(INT nPosX, INT nPosY, UINT fKeyFlags);
  VOID OnLeftButtonUp(INT nPosX, INT nPosY, UINT fKeyFlags);
private:
  INT m_nBorderWidth;
  INT m_nPos;
  Direction m_fDir;
  BOOL m_bHover;
  BOOL m_bGrab;
  std::array<Window*, 2> m_pPaneWnd;
};

class Pane final : public Window {
public:
  Pane() = delete;
  Pane(Window* pParentWnd);
  Pane(const Pane& other) noexcept = delete;
  Pane(Pane&& other) noexcept = delete;
  virtual ~Pane();
public:
  Pane& operator=(const Pane& rhs) = delete;
  Pane& operator=(Pane&& rhs) noexcept = delete;
protected:
  LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};

}

#endif  // MNECA_SPLITTER_H
