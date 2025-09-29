#ifndef COM_REVFAD_DEFER_H_
#define COM_REVFAD_DEFER_H_

#include <functional>

namespace revfad_wiki {

class Defer {
public:
  Defer(std::function<void(void)> deferred) : deferred_(deferred) {}
  ~Defer() { deferred_(); }
  Defer(const Defer &) = delete;
  Defer &operator=(const Defer &) = delete;

private:
  std::function<void(void)> deferred_;
};

} // namespace revfad_wiki

#endif // COM_REVFAD_DEFER_H_