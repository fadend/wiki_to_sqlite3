#ifndef COM_REVFAD_WIKI_ITER_H_
#define COM_REVFAD_WIKI_ITER_H_

#include "kmp_matcher.h"
#include "wiki_article.h"
#include <istream>
#include <optional>
#include <string>

namespace revfad_wiki {
class WikiIter {
public:
  WikiIter(std::istream &input)
      : input_(input), page_start_("<page>"), page_end_("</page>"),
        title_start_("<title>"), title_end_("</title>") {}
  WikiIter(const WikiIter &) = delete;
  virtual ~WikiIter() = default;

  std::optional<WikiArticle> next();

private:
  std::istream &input_;
  KmpMatcher page_start_;
  KmpMatcher page_end_;
  KmpMatcher title_start_;
  KmpMatcher title_end_;
  // Number of chars read from input_.
  long int len_ = 0;

  // Returns true if matcher matched.
  //
  // If buffer is provided, it will be filled with the chars up to
  // the point of the match but excluding the pattern.
  //
  // The matcher will be reset.
  //
  // Returns false if input_ is invalid.
  bool consume(KmpMatcher &matcher, std::string *buffer = nullptr);
};
} // namespace revfad_wiki

#endif // COM_REVFAD_WIKI_ITER_H_