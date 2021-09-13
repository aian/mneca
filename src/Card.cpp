/*-
 * This file is part of mneca project. For license details, see the file
 * 'LICENSE' in this package.
 */
#include "Local.h"

namespace mneca {

String::String() { }

String::String(const String& other) { }

String::String(String&& other) noexcept { }

String::~String() { }

String&
String::operator=(const String& other) {
  return *this;
}

String&
String::operator=(String&& other) noexcept {
  return *this;
}

VOID
String::Swap(String& other) noexcept { }

/* ------------------------------------------------------------------------- */


}
