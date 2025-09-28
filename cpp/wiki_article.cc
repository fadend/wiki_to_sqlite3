
#include "wiki_article.h"

namespace revfad_wiki {

std::ostream &operator<<(std::ostream &out, const WikiArticle &article) {
  return out << "WikiArticle<title=" << article.title()
             << ", position=" << article.position()
             << ", length=" << article.length() << ">";
}

} // namespace revfad_wiki