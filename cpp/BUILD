cc_library(
  name="kmp_matcher",
  srcs=["kmp_matcher.cc"],
  hdrs=["kmp_matcher.h"],
)

cc_library(
  name="wiki_article",
  srcs=["wiki_article.cc"],
  hdrs=["wiki_article.h"],
)

cc_library(
  name="wiki_iter",
  srcs=["wiki_iter.cc"],
  hdrs=["wiki_iter.h"],
  deps=[":kmp_matcher", ":wiki_article"]
)

# Tests

cc_test(
  name="kmp_matcher_test",
  srcs=["kmp_matcher_test.cc"],
  deps=[
    ":kmp_matcher",
    "@googletest//:gtest",
    "@googletest//:gtest_main",
    ]
)

cc_test(
  name="wiki_iter_test",
  srcs=["wiki_iter_test.cc"],
  deps=[
    ":wiki_iter",
    ":wiki_article",
    "@googletest//:gtest",
    "@googletest//:gtest_main",
    ]
)