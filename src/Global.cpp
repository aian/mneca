/*-
 * This file is part of mneca project. For license details, see the file
 * 'LICENSE' in this package.
 */
#include "Local.h"

namespace mneca {

static HINSTANCE g_hInstance = nullptr;

VOID
GlobalInitialize(HINSTANCE hInstance) {
  // Initialize application parameters
  {
    g_hInstance = hInstance;
  }
  // Initialize application parameters
  {
    Param::Init();
  }
  // Initialize COM component
  {
    HRESULT hr = S_OK;
    hr = CoInitialize(nullptr);
    if (FAILED(hr)) {
      throw std::runtime_error("Failed to initialize a COM framework");
    }
  }
  // Initialize common contorols
  {
    InitCommonControls();
  }
};

HINSTANCE
GetInstance() noexcept {
  return g_hInstance;
}

Param*
GetParam() {
  return Param::GetInstance();
}

VOID
GlobalFinalize() {
  g_hInstance = nullptr;
  Param::Finalize();
}

}
