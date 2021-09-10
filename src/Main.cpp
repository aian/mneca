#include "Local.h"

void
ErrorMessage(const std::string& strMessage) {
  ::MessageBoxA(
    nullptr, strMessage.c_str(), "mneca: ERROR", MB_OK | MB_ICONERROR);
}

INT
Process(HINSTANCE hInstance, INT nCmdShow) {
  INT ret = EXIT_SUCCESS;

  mneca::GlobalInitialize(hInstance);

  try {
    mneca::AppWindow app;

    app.Run(nCmdShow);

    ret = EXIT_SUCCESS;
  } catch(const std::exception& e) {
    ErrorMessage(e.what());
    ret = EXIT_FAILURE;
  }

  mneca::GlobalFinalize();

  return ret;
}

INT WINAPI
wWinMain(
  _In_ HINSTANCE hInstance, [[maybe_unused]] _In_opt_ HINSTANCE hPrevInstance,
  [[maybe_unused]] _In_ PWSTR lpCmdLine, _In_ INT nCmdShow) {
  return Process(hInstance, nCmdShow);
}
