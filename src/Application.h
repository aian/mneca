/*-
 * This file is part of mneca project. For license details, see the file
 * 'LICENSE' in this package.
 */
#ifndef MNECA_APPLICATION_H
#define MNECA_APPLICATION_H

#pragma once

#include <uiribbon.h>

namespace mneca {

class AppWindow final {
public:
  AppWindow();
  AppWindow(const AppWindow& other) = delete;
  AppWindow(AppWindow&& other) = delete;
  ~AppWindow();
public:
  AppWindow& operator=(const AppWindow& rhs) = delete;
  AppWindow& operator=(AppWindow&& rhs) = delete;
public:
  void Run(INT nCmdShow);
public:
  class Impl;
private:
  std::unique_ptr<Impl> impl_;
};

}

#endif  // MNECA_APPLICATION_H
