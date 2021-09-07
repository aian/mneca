#ifndef MNECA_GLOBAL_H
#define MNECA_GLOBAL_H

#pragma once

#include <windows.h>

namespace mneca {

extern VOID GlobalInitialize(HINSTANCE hInstance) noexcept;

extern HINSTANCE GetInstance() noexcept;

extern Param* GetParam();

extern VOID GlobalFinalize();

}

#endif  // MNECA_GLOBAL_H
