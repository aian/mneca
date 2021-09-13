/*-
 * This file is part of mneca project. For license details, see the file
 * 'LICENSE' in this package.
 */
#ifndef MNECA_CARD_H
#define MNECA_CARD_H

#pragma once

#include <array>
#include <vector>

namespace mneca {

using IDType = uint32_t;

enum class LangType : uint32_t {
  LANG_TYPE_UNKNOWN  = 0,
  LANG_TYPE_ENGLISH  = 1,
  LANG_TYPE_JAPANESE = 2,
};

enum class POSType : uint32_t {
  POS_TYPE_UNKNOWN = 0,
  POS_TYPE_NOUN,
  POS_TYPE_VERB,
  POS_TYPE_ADJ,
  POS_TYPE_ADV,
  POS_TYPE_PREP,
  POS_TYPE_OTHER
};

class String final {
public:
  String();
  String(const String& other);
  String(String&& other) noexcept;
  ~String();
public:
  String& operator=(const String& other);
  String& operator=(String&& other) noexcept;
public:
  VOID Swap(String& other) noexcept;
private:
  LangType m_nLang;
  std::wstring m_strData;
};

class Semantics final {
public:
  Semantics();
  Semantics(const Semantics& other);
  Semantics(Semantics&& other);
  ~Semantics();
public:
  Semantics& operator=(const Semantics& rhs);
  Semantics& operator=(Semantics&& rhs);
private:
  IDType m_uID;
  POSType m_uPOS;
  std::vector<String> m_strDefs;
  std::vector<String> m_strExamples;
};

class Card final {
public:
  Card();
  Card(const Card& other);
  Card(Card&& other);
  ~Card();
public:
  Card& operator=(const Card& other);
  Card& operator=(Card&& other);
private:
  IDType m_uID;
  std::wstring m_strLemma;
  std::wstring m_strPron;
  std::vector<Semantics> m_SemanticsSet;
  double m_dRate;
};

}

#endif  // MNECA_CARD_H
