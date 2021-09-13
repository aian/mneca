/*-
 * This file is part of mneca project. For license details, see the file
 * 'LICENSE' in this package.
 */
#ifndef MNECA_LOCAL_H
#define MNECA_LOCAL_H

#pragma once

#define WIN32_LEAN_AND_MEAN

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <array>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <shlwapi.h>
#include <uiribbon.h>

#include "sqlite3.h"

#include "Resource.h"

// This header file is generated from a ribbon.xml
#include "RibbonUI.h"

#include "Param.h"
#include "Global.h"
#include "Rect.h"
#include "Card.h"
#include "Storage.h"
#include "Widget.h"
#include "Window.h"
#include "Splitter.h"
#include "DlgWord.h"
#include "StatusBar.h"
#include "Ribbon.h"
#include "ListView.h"
#include "Application.h"

#endif  // MNECA_LOCAL_H
