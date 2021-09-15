/*-
 * This file is part of mneca project. For license details, see the file
 * 'LICENSE' in this package.
 */
#include "Local.h"

#define SQLITE3_ERROR(Rc, Mess) \
  if ((Rc) != SQLITE_OK) { \
    throw std::runtime_error(Mess); \
  }

namespace mneca {

class Storage::Impl final {
public:
  Impl();
  Impl(const Impl& other) = delete;
  Impl(const Impl&& other) = delete;
  ~Impl();
public:
  Impl& operator=(const Impl& rhs) = delete;
  Impl& operator=(const Impl&& rhs) = delete;
public:
  void create(const std::string& path);
  void open(const std::string& path);
  void close();
  bool is_open() const;
private:
  sqlite3* dbh_;
};

Storage::Impl::Impl() : dbh_(nullptr) {}

Storage::Impl::~Impl() {
  if (this->is_open()) {
    this->close();
  }
}

static void
create_tables_(sqlite3* dbh) {
  int rc = 0;
  constexpr char sql[] =
    "DROP TABLE IF EXISTS card; "
    "CREATE TABLE card ( "
    "  id INTEGER PRIMARY KEY AUTO_INCREMENT, "
    "  lemma TEXT NOT NULL, "
    "  pron TEXT, "
    "  pos INTEGER "
    "); "
    
    ;

  rc = sqlite3_exec(dbh, sql, nullptr, nullptr, nullptr);
  SQLITE3_ERROR(rc, "Failed to create tables");
}

void
Storage::Impl::create(const std::string& path) {
  int rc = 0;
  sqlite3* dbh = nullptr;
  constexpr int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;

  if(PathFileExistsA(path.c_str())) {
    throw std::runtime_error("Database file does not exist");
  }
  rc = sqlite3_open_v2(path.c_str(), &dbh, flags, nullptr);
  SQLITE3_ERROR(rc, "Failed to open database");
  this->dbh_ = dbh;

  create_tables_(this->dbh_);

}

void
Storage::Impl::open(const std::string& path) {
  int rc = 0;
  sqlite3* dbh = nullptr;
  constexpr int flags = SQLITE_OPEN_READWRITE;

  if (!PathFileExistsA(path.c_str())) {
    throw std::runtime_error("Database file does not exist");
  }
  rc = sqlite3_open_v2(path.c_str(), &dbh, flags, nullptr);
  SQLITE3_ERROR(rc, "Failed to open database");
  this->dbh_ = dbh;
}

void
Storage::Impl::close() {
  if (this->is_open()) {
    int rc = sqlite3_close_v2(dbh_);
    SQLITE3_ERROR(rc, "Failed to close database");
  }
}

bool
Storage::Impl::is_open() const {
  return this->dbh_ ? true : false;
}

/* ------------------------------------------------------------------------- */

Storage::Storage() : ph_(new Storage::Impl()) { }

Storage::~Storage() { }



}
