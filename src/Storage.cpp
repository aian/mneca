/*-
 * This file is part of mneca project. For license details, see the file
 * 'LICENSE' in this package.
 */
#include "Local.h"

#define SQLITE3_ERROR(Rc, Mess) \
  if ((Rc) != SQLITE_OK) { \
    throw std::runtime_error(Mess); \
  }

namespace mneca {

static void
CreateTables(sqlite3* pDB) {
  int nRC = 0;

  constexpr char lpSQL[] =
    "DROP TABLE IF EXISTS card; "
    "CREATE TABLE card ( "
    "  id INTEGER PRIMARY KEY AUTO_INCREMENT, "
    "  lemma TEXT NOT NULL, "
    "  pron TEXT, "
    "  pos INTEGER "
    "); "
    ;

  nRC = sqlite3_exec(pDB, lpSQL, nullptr, nullptr, nullptr);
  SQLITE3_ERROR(nRC, "Failed to create tables");
}

Storage::Storage() : m_pDB(nullptr) { }

Storage::~Storage() {
  this->Close();
}

VOID
Storage::Create(const std::wstring& strPath) {
  INT nRC = 0;
  sqlite3* pDB = nullptr;

  if (PathFileExists(strPath.c_str())) {
    throw std::runtime_error("Database file does not exist");
  }
  nRC = sqlite3_open16(strPath.c_str(), &pDB);
  SQLITE3_ERROR(nRC, "Failed to open database");
  this->m_pDB = pDB;

  CreateTables(this->m_pDB);
}

VOID
Storage::Open(const std::wstring& strPath) {
  INT nRC = 0;
  sqlite3* pDB = nullptr;

  if (!PathFileExists(strPath.c_str())) {
    throw std::runtime_error("Database file does not exist");
  }
  nRC = sqlite3_open16(strPath.c_str(), &pDB);
  SQLITE3_ERROR(nRC, "Failed to open database");
  this->m_pDB = pDB;
}

VOID
Storage::Close() {
  if (this->IsOpen()) {
    INT rc = sqlite3_close_v2(m_pDB);
    SQLITE3_ERROR(rc, "Failed to close database");
    m_pDB = nullptr;
  }
}

constexpr BOOL
Storage::IsOpen() const noexcept {
  return this->m_pDB ? TRUE : FALSE;
}

}
