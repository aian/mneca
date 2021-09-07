#ifndef MNECA_STORAGE_H
#define MNECA_STORAGE_H

#pragma once

#include <memory>

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
  VOID Create(const std::wstring& path);
  VOID Open(const std::wstring& path);
private:
  class Impl;
  std::unique_ptr<Impl> ph_;
};

}

#endif  // MNECA_STORAGE_H
