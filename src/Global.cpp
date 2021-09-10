#include "Local.h"

namespace mneca {

static HINSTANCE g_hInstance = nullptr;

VOID
GlobalInitialize(HINSTANCE hInstance) noexcept {
  g_hInstance = hInstance;
  Param::Init();
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
