/*-
 * This file is part of mneca project. For license details, see the file
 * 'LICENSE' in this package.
 */
#include "Local.h"

namespace mneca {

constexpr int LIST_COLUMN_MASK =
LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

constexpr int LIST_COLUMN_FORMAT = LVCFMT_LEFT;

ListColumn::ListColumn(
  const std::wstring& strTitle, int nWidth, ATTRIBUTE fAttribute)
  : m_strTitle(strTitle), m_nWidth(nWidth), m_fMask(LIST_COLUMN_MASK),
    m_fFormat(LIST_COLUMN_FORMAT), m_fAttribute(fAttribute) { }

ListColumn::ListColumn(const ListColumn& other)
  : m_strTitle(other.m_strTitle), m_nWidth(other.m_nWidth),
    m_fMask(other.m_fMask), m_fFormat(other.m_fFormat),
    m_fAttribute(other.m_fAttribute) { }

ListColumn::ListColumn(ListColumn&& other) noexcept {
  this->Swap(other);
}

ListColumn::~ListColumn() { }

ListColumn&
ListColumn::operator=(const ListColumn& rhs) {
  if(&rhs != this) {
    ListColumn tmp(rhs);
    tmp.Swap(*this);
  }
  return *this;
}

ListColumn&
ListColumn::operator=(ListColumn&& rhs) noexcept {
  rhs.Swap(*this);
  return *this;
}

const WCHAR*
ListColumn::GetTitle() const noexcept {
  return m_strTitle.c_str();
}

int
ListColumn::GetWidth() const noexcept {
  return m_nWidth;
}

int
ListColumn::GetMask() const noexcept {
  return m_fMask;
}

int
ListColumn::GetFormat() const noexcept {
  return m_fFormat;
}

BOOL
ListColumn::IsFixed() const noexcept {
  return m_fAttribute == ListColumn::ATTRIBUTE::FIXED_WIDTH ? TRUE : FALSE;
}

BOOL
ListColumn::IsVariable() const noexcept {
  return m_fAttribute == ListColumn::ATTRIBUTE::VARIABLE_WIDTH ? TRUE : FALSE;
}

VOID
ListColumn::SetWidth(const int width) noexcept {
  m_nWidth = width < 0 ? 0 : width;
}

VOID
ListColumn::Swap(ListColumn& other) noexcept {
  m_strTitle.swap(other.m_strTitle);
  std::swap(m_nWidth, other.m_nWidth);
  std::swap(m_fMask, other.m_fMask);
  std::swap(m_fFormat, other.m_fFormat);
  std::swap(m_fAttribute, other.m_fAttribute);
}

/* ------------------------------------------------------------------------- */

ListItem::ListItem()
  : m_nID(0),
    m_strLemma(L""),
    m_nPOS(POSType::POS_TYPE_UNKNOWN),
    m_strDef(L""),
    m_dRate(0.f)
{ }

ListItem::ListItem(const ListItem& other)
  : m_nID(other.m_nID),
    m_strLemma(other.m_strLemma),
    m_nPOS(other.m_nPOS),
    m_strDef(other.m_strDef),
    m_dRate(other.m_dRate)
{ }

ListItem::ListItem(ListItem&& other) noexcept {
  this->Swap(std::move(other));
}

ListItem::~ListItem() { }

ListItem&
ListItem::operator=(const ListItem& rhs) {
  if (this != &rhs) {
    this->Swap(ListItem(rhs));
  }
  return *this;
}

ListItem&
ListItem::operator=(ListItem&& rhs) noexcept {
  this->Swap(std::move(rhs));
  return *this;
}

mneca::IDType
ListItem::GetID() const {
  return this->m_nID;
}

std::wstring
ListItem::GetLemma() const {
  return this->m_strLemma;
}

mneca::POSType
ListItem::GetPOS() const {
  return this->m_nPOS;
}

std::wstring
ListItem::GetDef() const {
  return this->m_strDef;
}

double
ListItem::GetRate() const {
  return this->m_dRate;
}

ListItem::RecordItem
ListItem::GetRecordItem() const {
  RecordItem item;

  item[0] = this->GetLemma();
  {
    switch (this->GetPOS()) {
    case POSType::POS_TYPE_NOUN:  item[1] = L"n.";    break;
    case POSType::POS_TYPE_VERB:  item[1] = L"v.";    break;
    case POSType::POS_TYPE_ADJ:   item[1] = L"adj.";  break;
    case POSType::POS_TYPE_ADV:   item[1] = L"adv.";  break;
    case POSType::POS_TYPE_PREP:  item[1] = L"prep."; break;
    case POSType::POS_TYPE_OTHER: item[1] = L"*";     break;
    default:                      item[1] = L"";      break;
    }
  }
  item[2] = this->GetDef();
  {
    std::wstringstream ss;
    ss << std::setprecision(3) << this->GetRate();
    item[3] = ss.str();
  }

  return item;
}

VOID
ListItem::SetID(mneca::IDType id) {
  this->m_nID = id;
}

VOID
ListItem::SetLemma(const std::wstring& lemma) {
  this->m_strLemma = lemma;
}

VOID
ListItem::SetPOS(mneca::POSType pos) {
  this->m_nPOS = pos;
}

VOID
ListItem::SetDef(const std::wstring& def) {
  this->m_strDef = def;
}

VOID
ListItem::SetRate(double rate) {
  this->m_dRate = rate;
}

VOID
ListItem::Swap(ListItem&& other) noexcept {
  std::swap(this->m_nID, other.m_nID);
  this->m_strLemma.swap(other.m_strLemma);
  std::swap(this->m_nPOS, other.m_nPOS);
  this->m_strDef.swap(other.m_strDef);
  std::swap(this->m_dRate, other.m_dRate);
}

/* ------------------------------------------------------------------------- */

ListWindow::ListWindow(Window* pParentWnd) : Window(IDR_LISTVIEW, pParentWnd) {
  HWND hWnd = nullptr;

  DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_VSCROLL | LVS_REPORT | LVS_NOCOLUMNHEADER;
  DWORD dwExStyle = WS_EX_COMPOSITED;
  DWORD dwListViewExStyle = 0;

  hWnd = ::CreateWindowEx(
    dwExStyle,
    WC_LISTVIEW,
    0,
    dwStyle,
    0, 0, 0, 0,
    pParentWnd->GetWindowHandle(),
    reinterpret_cast<HMENU>(IDR_LISTVIEW),
    GetInstance(),
    nullptr
  );
  if(hWnd == nullptr) {
    throw std::runtime_error("Failed to create a listview window");
  }

  dwListViewExStyle = ListView_GetExtendedListViewStyle(hWnd);
  dwListViewExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
  ListView_SetExtendedListViewStyle(hWnd, dwListViewExStyle);

  this->SetWindowHandle(hWnd);
  this->SetClientRect();

  {
    /* aliases */
    using _ = ListColumn;
    constexpr _::ATTRIBUTE FIXED    = _::ATTRIBUTE::FIXED_WIDTH;
    constexpr _::ATTRIBUTE VARIABLE = _::ATTRIBUTE::VARIABLE_WIDTH;

    this->AddColumn(ListColumn(L"Lemma", 150, FIXED));
    this->AddColumn(ListColumn(L"POS", 35, FIXED));
    this->AddColumn(ListColumn(L"Definition", 100, VARIABLE));
    this->AddColumn(ListColumn(L"Rate", 100, FIXED));

    this->InsertColumn();
    this->AdjustColumn();
    this->UpdateColumn();
  }
}

ListWindow::~ListWindow() { }

BOOL
ListWindow::AddItem(const ListItem& item) {
  LVITEM lvItem = { 0 };
  ListItem::RecordItem rcItem;

  lvItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
  lvItem.stateMask = 0;
  lvItem.iItem = 0;
  lvItem.state = 0;

  rcItem = item.GetRecordItem();
  for (size_t i = 0; i < rcItem.size(); ++i) {
    lvItem.iSubItem = static_cast<int>(i);
    lvItem.pszText = const_cast<LPWSTR>(rcItem[i].c_str());

    // Insert items into the list.
    if (i == 0) {
      if(ListView_InsertItem(this->GetWindowHandle(), &lvItem) == -1) {
        return FALSE;
      }
    } else {
      if(ListView_SetItem(this->GetWindowHandle(), &lvItem) == -1) {
        return FALSE;
      }
    }
  }

  return TRUE;
}

VOID
ListWindow::OnEndTrack(NMHEADER* header) {
  ListColumn& col = this->cols_[header->iItem];
  col.SetWidth(header->pitem->cxy);

  this->AdjustColumn();
  this->UpdateColumn();
}

VOID
ListWindow::OnSize(UINT nWidth, UINT nHeight) {
  Rect rcThis = this->GetRect();
  RECT rc = static_cast<RECT>(rcThis);

  ::MoveWindow(
    this->GetWindowHandle(),
    rcThis.GetPosX1(), rcThis.GetPosY1(),
    rcThis.GetWidth(), rcThis.GetHeight(),
    FALSE);

  this->AdjustColumn();
  this->UpdateColumn();

  ::InvalidateRect(this->GetWindowHandle(), &rc, TRUE);
}

BOOL
ListWindow::InsertColumn() {
  LVCOLUMN  lvc ={ 0 };

  for(auto itr = this->cols_.begin(); itr != this->cols_.end(); ++itr) {
    size_t index = std::distance(this->cols_.begin(), itr);

    lvc.iSubItem = static_cast<int>(index);
    lvc.pszText = const_cast<LPWSTR>(itr->GetTitle());
    lvc.cx = itr->GetWidth();
    lvc.mask = itr->GetMask();
    lvc.fmt = itr->GetFormat();

    int ret = ListView_InsertColumn(this->GetWindowHandle(), index, &lvc);
    if(ret == -1) {
      return FALSE;
    }
  }

  return TRUE;
}

BOOL
ListWindow::AdjustColumn() {
  UINT uVariableWidth = 0;
  UINT uVariableColumnCount = 0;
  UINT uFixedColumnSize = 0;

  for(auto itr : this->cols_) {
    if(itr.IsVariable()) {
      uVariableColumnCount++;
    }
    else {
      uFixedColumnSize += itr.GetWidth();
    }
  }
  if(uVariableColumnCount > 0) {
    Rect rcThis;
    INT nRemains = 0;

    rcThis = this->GetRect();
    nRemains = rcThis.GetWidth() - uFixedColumnSize;
    if(nRemains > 0) {
      uVariableWidth = nRemains / uVariableColumnCount;
    }
    else {
      uVariableWidth = 0;
    }
  }
  for(auto& itr : this->cols_) {
    if(itr.IsVariable()) {
      itr.SetWidth(uVariableWidth);
    }
  }

  return TRUE;
}

BOOL
ListWindow::UpdateColumn() {
  for (auto itr = this->cols_.begin(); itr != this->cols_.end(); ++itr) {
    size_t index = std::distance(this->cols_.begin(), itr);
    HWND hwnd = this->GetWindowHandle();

    ListView_SetColumnWidth(hwnd, index, itr->GetWidth());
  }
  return TRUE;
}

VOID
ListWindow::AddColumn(const ListColumn& col) {
  this->cols_.push_back(col);
}

LRESULT
ListWindow::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch(uMsg) {
  case WM_NOTIFY:
    switch(reinterpret_cast<LPNMHDR>(lParam)->code) {
    case HDN_ENDTRACK:
      this->OnEndTrack(reinterpret_cast<NMHEADER*>(lParam));
      break;
    default:
      break;
    }
    break;
  case WM_SIZE:
    this->OnSize(LOWORD(lParam), HIWORD(lParam));
    break;
  default:
    // do nothing
    break;
  }
  return static_cast<LRESULT>(0);
}

}
