
#ifndef COM_REVFAD_KMP_MATCHER_H_
#define COM_REVFAD_KMP_MATCHER_H_

#include <string>
#include <vector>

namespace revfad_wiki {

class KmpMatcher {
public:
  KmpMatcher(std::string pattern);

  int size() const { return pattern_len_; }
  bool consume(char c);
  void reset();

private:
  const std::string pattern_;
  const int pattern_len_;
  std::vector<int> next_;
  int position_ = 0;

  void initialize_next_table();
};

} // namespace revfad_wiki

#endif // COM_REVFAD_KMP_MATCHER_H_