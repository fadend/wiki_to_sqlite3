
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>

#include "defer.h"
#include "wiki_article.h"
#include "wiki_iter.h"
#include <absl/flags/flag.h>
#include <absl/flags/parse.h>
#include <sqlite3.h>

ABSL_FLAG(std::string, wiki_xml_path, "", "Path to Wikipedia .xml file");
ABSL_FLAG(std::string, output_path, "", "Path at which to store the output");
ABSL_FLAG(bool, overwrite, false, "Whether to overwrite existing output");
ABSL_FLAG(int, max_count, -1,
          "If positive, limit the number of articles to be processed");

constexpr char kTableCreate[] =
    R"(CREATE TABLE WikiIndex (title TEXT, start INTEGER, length INTEGER))";
constexpr char kInsert[] = R"(INSERT INTO WikiIndex VALUES (?, ?, ?))";

int main(int argc, char **argv) {
  absl::ParseCommandLine(argc, argv);
  const std::string wiki_xml_path = absl::GetFlag(FLAGS_wiki_xml_path);
  if (wiki_xml_path.empty()) {
    std::cerr << "Missing --wiki_xml_path\n";
    std::exit(1);
  }
  const std::string output_path = absl::GetFlag(FLAGS_output_path);
  if (output_path.empty()) {
    std::cerr << "Missing --output_path\n";
    std::exit(1);
  }
  const bool overwrite = absl::GetFlag(FLAGS_overwrite);
  if (std::filesystem::exists(output_path)) {
    if (!overwrite) {
      std::cerr << "Output path exists with --overwrite=false: " << output_path
                << '\n';
      std::exit(1);
    } else {
      if (!std::filesystem::remove(output_path)) {
        std::cerr << "Unable to remove existing output: " << output_path
                  << '\n';
        std::exit(1);
      }
    }
  }
  const int max_count = absl::GetFlag(FLAGS_max_count);
  int count = 0;
  std::ifstream input(wiki_xml_path, std::ios::binary);
  revfad_wiki::WikiIter wiki_iter(input);
  // TODO: factor out SQLite code.
  // Following this recommendations from this great StackOverflow discussion:
  // https://stackoverflow.com/questions/1711631/improve-insert-per-second-performance-of-sqlite
  sqlite3 *db = nullptr;
  // Note: it's safe to call sqlite3_close on nullptr:
  // https://www.sqlite.org/c3ref/close.html.
  revfad_wiki::Defer close_db([db]() { sqlite3_close(db); });
  if (sqlite3_open(output_path.c_str(), &db) != 0) {
    std::cerr << "Couldn't open " << output_path << ": " << sqlite3_errmsg(db)
              << '\n';
    return 1;
  }
  char *error = nullptr;
  if (sqlite3_exec(db, kTableCreate, nullptr, nullptr, &error) != 0) {
    std::cerr << "Couldn't create table in " << output_path << ": " << error
              << '\n';
    return 1;
  }
  if (sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, &error) != 0) {
    std::cerr << "Couldn't BEGIN TRANSACTION in " << output_path << ": "
              << error << '\n';
    return 1;
  }
  revfad_wiki::Defer end_transaction([db]() {
    sqlite3_exec(db, "END TRANSACTION", nullptr, nullptr, nullptr);
  });
  sqlite3_stmt *insert_stmt = nullptr;
  if (const int prepare_return =
          sqlite3_prepare_v2(db, kInsert, -1, &insert_stmt, nullptr);
      prepare_return != SQLITE_OK) {
    std::cerr << "Couldn't prepare insert statement: "
              << sqlite3_errstr(prepare_return) << '\n';
    return 1;
  }
  revfad_wiki::Defer finalize_insert_stmt(
      [insert_stmt]() { sqlite3_finalize(insert_stmt); });
  while (true) {
    if (max_count > -1 && count >= max_count) {
      break;
    }
    auto article = wiki_iter.next();
    if (!article.has_value()) {
      break;
    } else {
      sqlite3_bind_text(insert_stmt, 1, article->title().c_str(),
                        article->title().size(), SQLITE_TRANSIENT);
      sqlite3_bind_int64(insert_stmt, 2, article->position());
      sqlite3_bind_int64(insert_stmt, 3, article->length());
      sqlite3_step(insert_stmt);
      sqlite3_clear_bindings(insert_stmt);
      sqlite3_reset(insert_stmt);
    }
    ++count;
  }
  // TODO: add index.
}