#include <gtest/gtest.h>
#include <common.hh>
#include <trie.hh>

/*
 * we suppose that the test are only run by the makefile and
 * from the project root directory
 */

const std::string dictPath = "tests/dicts/dict05.txt";

TEST(Base, ParralelBuild)
{
  std::ifstream dictStream(dictPath);
  ASSERT_TRUE(dictStream.is_open());
  std::vector<Entry> dict(ParseDict(dictStream));
  EXPECT_FALSE(dict.empty());

  TrieBuilder tb(dict);
  tb.ParallelBuild();
}

TEST(Base, SimpleBuild)
{
  std::ifstream dictStream(dictPath);
  ASSERT_TRUE(dictStream.is_open());
  std::vector<Entry> dict(ParseDict(dictStream));
  EXPECT_FALSE(dict.empty());

  TrieBuilder tb(dict);
  tb.Build();
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
