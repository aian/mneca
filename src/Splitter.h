#ifndef MNECA_SPLITTER_H
#define MNECA_SPLITTER_H

#pragma once

namespace mneca {

class Splitter final : public Window {
public:
  enum class Direction {
    VERTICAL   = 0,
    HORIZONTAL = 1,
  };
public:
  Splitter() = delete;
  Splitter(INT nID, Widget* pParent);
  Splitter(const Splitter& other) = delete;
  Splitter(Splitter&& other) noexcept = delete;
  virtual ~Splitter();
private:
  Splitter& operator=(const Splitter& rhs) = delete;
  Splitter& operator=(Splitter&& rhs) noexcept = delete;
public:
  VOID SetDirection(Direction fDir) noexcept;
private:
  Rect m_rcSeparator;
  Direction m_fDirection;
};

}

#endif  // MNECA_SPLITTER_H
