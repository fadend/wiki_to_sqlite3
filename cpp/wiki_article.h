#ifndef COM_REVFAD_WIKI_ARTICLE_H_
#define COM_REVFAD_WIKI_ARTICLE_H_

#include <iostream>
#include <string>
#include <utility>

namespace revfad_wiki {
class WikiArticle {
public:
  WikiArticle(std::string title, long int position, long int length)
      : title_(std::move(title)), position_(position), length_(length) {}

  /**
   * The contents within "<title>...</title>".
   */
  const std::string &title() const { return title_; }

  /**
   * The byte offset of the "<" in "<page>" that opens the article.
   */
  const long int position() const { return position_; }

  /**
   * The number of bytes from `position()` to the end of "</page>".
   */
  const long int length() const { return length_; }

  bool operator==(const WikiArticle &other) const {
    return other.title() == title_ && other.position() == position_ &&
           other.length() == length_;
  }

  bool operator!=(const WikiArticle &other) const { return !(*this == other); }

private:
  std::string title_;
  long int position_ = 0;
  long int length_ = 0;
};

std::ostream &operator<<(std::ostream &out, const WikiArticle &article);
} // namespace revfad_wiki

#endif // COM_REVFAD_WIKI_ARTICLE_H_