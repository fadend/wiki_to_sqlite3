#include "wiki_iter.h"

namespace revfad_wiki {

WikiIter::WikiIter(std::istream &input)
    : input_(input), page_start_("<page>"), page_end_("</page>"),
      title_start_("<title>"), title_end_("</title>") {}

std::optional<WikiArticle> WikiIter::next() {
  if (!consume(page_start_)) {
    return std::nullopt;
  }
  long int page_index = len_ - page_start_.size();
  if (!consume(title_start_)) {
    return std::nullopt;
  }
  std::string title;
  title.reserve(1000);
  if (!consume(title_end_, &title)) {
    return std::nullopt;
  }
  if (!consume(page_end_)) {
    return std::nullopt;
  }
  long int page_len = len_ - page_index;
  return WikiArticle(std::move(title), page_index, page_len);
}

bool WikiIter::consume(KmpMatcher &matcher, std::string *buffer) {
  while (input_) {
    char c = input_.get();
    ++len_;
    if (!input_) {
      --len_;
      return false;
    }
    if (buffer != nullptr) {
      buffer->push_back(c);
    }
    if (matcher.consume(c)) {
      if (buffer != nullptr) {
        buffer->resize(buffer->size() - matcher.size());
      }
      matcher.reset();
      return true;
    }
  }
  return false;
}
} // namespace revfad_wiki