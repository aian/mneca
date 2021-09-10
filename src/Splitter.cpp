#include "Local.h"

namespace mneca {

Splitter::Splitter(INT nID, Widget* pParent)
  : Window(nID, pParent),
    m_rcSeparator{},
    m_fDirection(Direction::VERTICAL)
{ }

Splitter::~Splitter() { }

VOID
Splitter::SetDirection(Direction fDir) noexcept {
  this->m_fDirection = fDir;
}

}
