/*-
 * This file is part of mneca project. For license details, see the file
 * 'LICENSE' in this package.
 */
#ifndef MNECA_RECT_H
#define MNECA_RECT_H

#pragma once

namespace mneca {

class Rect {
public:
  Rect();
  Rect(const Rect& other);
  Rect(Rect&& other) noexcept;
  Rect(const RECT& rect);
  virtual ~Rect();
public:
  Rect& operator=(const Rect& rhs);
  Rect& operator=(Rect&& rhs) noexcept;
  explicit operator RECT() const noexcept;
public:
  VOID Set(INT nPosX1, INT nPosY1, INT nPosX2, INT nPosY2) noexcept;
  VOID SetPosX1(INT nPos) noexcept;
  VOID SetPosY1(INT nPos) noexcept;
  VOID SetPosX2(INT nPos) noexcept;
  VOID SetPosY2(INT nPos) noexcept;
public:
  INT GetWidth() const noexcept;
  INT GetHeight() const noexcept;
  INT GetPosX1() const noexcept;
  INT GetPosY1() const noexcept;
  INT GetPosX2() const noexcept;
  INT GetPosY2() const noexcept;
public:
  VOID Swap(Rect& other) noexcept;
private:
  INT m_nPosX1;
  INT m_nPosY1;
  INT m_nPosX2;
  INT m_nPosY2;
};

}

#endif  // MNECA_RECT_H
