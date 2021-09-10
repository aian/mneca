#include "Local.h"

namespace mneca {

Rect::Rect()
  : m_nPosX1(INT()),
    m_nPosY1(INT()),
    m_nPosX2(INT()),
    m_nPosY2(INT())
{ }

Rect::Rect(const Rect& other)
  : m_nPosX1(other.m_nPosX1),
    m_nPosY1(other.m_nPosY1),
    m_nPosX2(other.m_nPosX2),
    m_nPosY2(other.m_nPosY2)
{ }

Rect::Rect(Rect&& other) noexcept
  : Rect() {
  this->Swap(other);
}

Rect::Rect(const RECT& rect)
  : m_nPosX1(rect.left),
    m_nPosY1(rect.top),
    m_nPosX2(rect.right),
    m_nPosY2(rect.bottom)
{ }

Rect::~Rect() { }

Rect&
Rect::operator=(const Rect& rhs) {
  if (this != &rhs) {
    Rect(rhs).Swap(*this);
  }
  return *this;
}

Rect&
Rect::operator=(Rect&& rhs) noexcept {
  rhs.Swap(*this);
  return *this;
}

Rect::operator RECT() const noexcept {
  return RECT{
    .left   = m_nPosX1,
    .top    = m_nPosY1,
    .right  = m_nPosX2,
    .bottom = m_nPosY2
  };
}

VOID
Rect::Set(INT nPosX1, INT nPosY1, INT nPosX2, INT nPosY2) noexcept {
  this->SetPosX1(nPosX1);
  this->SetPosY1(nPosY1);
  this->SetPosX2(nPosX2);
  this->SetPosY2(nPosY2);
}

VOID
Rect::SetPosX1(INT nPos) noexcept {
  this->m_nPosX1 = nPos;
}

VOID
Rect::SetPosY1(INT nPos) noexcept {
  this->m_nPosY1 = nPos;
}

VOID
Rect::SetPosX2(INT nPos) noexcept {
  this->m_nPosX2 = nPos;
}

VOID
Rect::SetPosY2(INT nPos) noexcept {
  this->m_nPosY2 = nPos;
}

INT
Rect::GetWidth() const noexcept {
  return this->m_nPosX2 - this->m_nPosX1;
}

INT
Rect::GetHeight() const noexcept {
  return this->m_nPosY2 - this->m_nPosY1;
}

INT
Rect::GetPosX1() const noexcept {
  return this->m_nPosX1;
}

INT
Rect::GetPosY1() const noexcept {
  return this->m_nPosY1;
}

INT
Rect::GetPosX2() const noexcept {
  return this->m_nPosX2;
}

INT
Rect::GetPosY2() const noexcept {
  return this->m_nPosY2;
}

VOID
Rect::Swap(Rect& other) noexcept {
  std::swap(this->m_nPosX1, other.m_nPosX1);
  std::swap(this->m_nPosY1, other.m_nPosY1);
  std::swap(this->m_nPosX2, other.m_nPosX2);
  std::swap(this->m_nPosY2, other.m_nPosY2);
}

}
