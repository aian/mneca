#ifndef MNECA_WIDGET_H
#define MNECA_WIDGET_H

#pragma once

namespace mneca {

class Widget {
public:
  Widget() = delete;
  Widget(INT nID, Widget* pParent);
  Widget(const Widget& other) = delete;
  Widget(Widget&& other) = delete;
  virtual ~Widget();
public:
  Widget& operator=(const Widget& rhs) = delete;
  Widget& operator=(Widget&& rhs) = delete;
public:
  VOID AddChild(Widget* pChild);
  Widget* GetParent() noexcept;
  Widget* GetChild(INT nID);
public:
  virtual VOID SetRect(const Rect& rect);
  virtual Rect GetRect() const noexcept;
  virtual INT GetPosX1() const noexcept;
  virtual INT GetPosY1() const noexcept;
  virtual INT GetPosX2() const noexcept;
  virtual INT GetPosY2() const noexcept;
  virtual INT GetWidth() const noexcept;
  virtual INT GetHeight() const noexcept;
protected:
  virtual INT GetID() const noexcept;
  LRESULT Dispatch(UINT nMsg, WPARAM wParam, LPARAM lParam);
public:
  virtual LRESULT WndProc(UINT nMsg, WPARAM wParam, LPARAM lParam) = 0;
public:
  static LRESULT CALLBACK WndProc(HWND hWnd, UINT nMeg, WPARAM wParam, LPARAM lParam);
protected:
  std::vector<Widget*> m_children;
private:
  Widget* m_pParent;
  Rect m_rcPos;
  INT m_nID;
};

}

#endif  // MNECA_WIDGET_H
