#ifndef MNECA_DLG_WORD_H
#define MNECA_DLG_WORD_H

#pragma once

namespace mneca {

class DlgWord final : public Window {
public:
  DlgWord() = delete;
  DlgWord(Window* pParentWnd);
  DlgWord(const DlgWord& other) = delete;
  DlgWord(DlgWord&& other) noexcept = delete;
  ~DlgWord();
public:
  DlgWord& operator=(const DlgWord& rhs) = delete;
  DlgWord& operator=(DlgWord&& rhs) noexcept = delete;
public:

private:
  IDType  m_nID;
};


}


#endif  // MNECA_DLG_WORD_H
