#include <gtest/gtest.h>

#include "kmp_matcher.h"

namespace revfad_wiki {
namespace {
TEST(KmpMatcherTest, EmptyPattern) {
  KmpMatcher matcher("");
  ASSERT_TRUE(matcher.consume('x'));
  matcher.reset();
  ASSERT_TRUE(matcher.consume('y'));
}

TEST(KmpMatcherTest, CharSequence) {
  KmpMatcher matcher("abc");
  ASSERT_FALSE(matcher.consume('a'));
  ASSERT_FALSE(matcher.consume('b'));
  ASSERT_FALSE(matcher.consume('d'));
  ASSERT_FALSE(matcher.consume('a'));
  ASSERT_FALSE(matcher.consume('b'));
  ASSERT_TRUE(matcher.consume('c'));
  ASSERT_FALSE(matcher.consume('c'));
}

TEST(KmpMatcherTest, Mama) {
  KmpMatcher matcher("mama");
  ASSERT_FALSE(matcher.consume('a'));
  ASSERT_FALSE(matcher.consume('m'));
  ASSERT_FALSE(matcher.consume('m'));
  ASSERT_FALSE(matcher.consume('a'));
  ASSERT_FALSE(matcher.consume('m'));
  ASSERT_TRUE(matcher.consume('a'));
  ASSERT_FALSE(matcher.consume('a'));
}

TEST(KmpMatcherTest, SingleChar) {
  KmpMatcher matcher("x");
  ASSERT_FALSE(matcher.consume('a'));
  ASSERT_TRUE(matcher.consume('x'));
  ASSERT_FALSE(matcher.consume('d'));
  ASSERT_TRUE(matcher.consume('x'));
  ASSERT_FALSE(matcher.consume('b'));
  ASSERT_TRUE(matcher.consume('x'));
  ASSERT_FALSE(matcher.consume('c'));
}

TEST(KmpMatcherTest, RepeatedChars) {
  KmpMatcher matcher("aaa");
  ASSERT_FALSE(matcher.consume('a'));
  ASSERT_FALSE(matcher.consume('a'));
  ASSERT_TRUE(matcher.consume('a'));
  ASSERT_TRUE(matcher.consume('a'));
  ASSERT_TRUE(matcher.consume('a'));
}

} // namespace
} // namespace revfad_wiki