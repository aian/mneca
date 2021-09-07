#ifndef MNECA_RIBBON_H
#define MNECA_RIBBON_H

#pragma once

#include <memory>

namespace mneca {

class  Ribbon final : public Window {
public:
  Ribbon() = delete;
  Ribbon(Window* pParentWnd);
  Ribbon(const Ribbon& other) = delete;
  Ribbon(Ribbon&& other) noexcept = delete;
  ~Ribbon();
public:
  Ribbon& operator=(const Ribbon& other) = delete;
  Ribbon& operator=(Ribbon&& other) noexcept = delete;
public:
  INT GetID() const noexcept override; 
  INT GetWidth() const noexcept override;
  INT GetHeight() const noexcept override;
protected:
  LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
private:
  class Impl;
  std::unique_ptr<Impl> m_pImpl;
};

}

#endif  // MNECA_RIBBON_H
