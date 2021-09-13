#ifndef MNECA_STORAGE_H
#define MNECA_STORAGE_H

#pragma once

namespace mneca {

class Storage final {
public:
  Storage();
  Storage(const Storage& other) = delete;
  Storage(Storage&& other) = delete;
  ~Storage();
public:
  Storage& operator=(const Storage& rhs) = delete;
  Storage& operator=(Storage&& rhs) = delete;
public:
  VOID Create(const std::wstring& strPath);
  VOID Open(const std::wstring& strPath);
  VOID Close();
public:
  BOOL IsOpen() const noexcept;
private:
  sqlite3* m_pDB;
  std::wstring m_strPath;
};

}

#endif  // MNECA_STORAGE_H
