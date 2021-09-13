/*-
 * This file is part of mneca project. For license details, see the file
 * 'LICENSE' in this package.
 */
#ifndef MNECA_LISTVIEW_H
#define MNECA_LISTVIEW_H

#pragma once

namespace mneca {

class ListColumn {
public:
  enum class ATTRIBUTE {
    FIXED_WIDTH,
    VARIABLE_WIDTH,
  };
public:
  ListColumn() = delete;
  ListColumn(const std::wstring& strTitle, int nWidth, ATTRIBUTE fAttribute);
  ListColumn(const ListColumn& other);
  ListColumn(ListColumn&& other) noexcept;
  virtual ~ListColumn();
public:
  ListColumn& operator=(const ListColumn& rhs);
  ListColumn& operator=(ListColumn&& rhs) noexcept;
public:
  const WCHAR* GetTitle() const noexcept;
  int GetWidth() const noexcept;
  int GetMask() const noexcept;
  int GetFormat() const noexcept;
  BOOL IsFixed() const noexcept;
  BOOL IsVariable() const noexcept;
  VOID SetWidth(const int width) noexcept;
public:
  VOID Swap(ListColumn& other) noexcept;
private:
  std::wstring m_strTitle;
  int m_nWidth;
  int m_fMask;
  int m_fFormat;
  ATTRIBUTE m_fAttribute;
};

class ListItem final {
public:
  using RecordItem = std::array<std::wstring, 4>;
public:
  ListItem();
  ListItem(const ListItem& other);
  ListItem(ListItem&& other) noexcept;
  ~ListItem();
public:
  ListItem& operator=(const ListItem& rhs);
  ListItem& operator=(ListItem&& rhs) noexcept;
public:
  mneca::IDType GetID() const;
  std::wstring GetLemma() const;
  mneca::POSType GetPOS() const;
  std::wstring GetDef() const;
  double GetRate() const;
  RecordItem GetRecordItem() const;
public:
  VOID SetID(mneca::IDType id);
  VOID SetLemma(const std::wstring& lemma);
  VOID SetPOS(mneca::POSType pos);
  VOID SetDef(const std::wstring& def);
  VOID SetRate(double rate);
public:
  VOID Swap(ListItem&& other) noexcept;
private:
  mneca::IDType m_nID;
  std::wstring m_strLemma;
  mneca::POSType m_nPOS;
  std::wstring m_strDef;
  double m_dRate;
};

class ListWindow final : public Window {
public:
  ListWindow() = delete;
  ListWindow(Window* pParentWnd);
  ListWindow(const ListWindow& other) = delete;
  ListWindow(const ListWindow&& other) = delete;
  virtual ~ListWindow();
public:
  ListWindow& operator=(const ListWindow& rhs) = delete;
  ListWindow& operator=(const ListWindow&& rhs) = delete;
public:
  VOID AddColumn(const ListColumn& col);
  BOOL InsertColumn();
  BOOL AdjustColumn();
  BOOL UpdateColumn();
public:
  BOOL AddItem(const ListItem& item);
private:
  VOID OnEndTrack(NMHEADER* header);
  VOID OnSize(UINT width, UINT height);
protected:
  LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
private:
  std::vector<ListColumn> cols_;
};

}

#endif  // MNECA_LISTVIEW_H
