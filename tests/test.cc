#include <gtest/gtest.h>
#include <common.hh>
#include <trie.hh>

/*
 * we suppose that the test are only run by the makefile and
 * from the project root directory
 */


class Base : public testing::Test
{
protected:
  virtual void SetUp()
  {
    std::ifstream dictStream(dictPath);
    ASSERT_TRUE(dictStream.is_open());
    std::vector<Entry> dict(ParseDict(dictStream));
    dictStream.close();
    EXPECT_FALSE(dict.empty());
    RecordProperty("EntryCount", dict.size());
  }

  std::vector<Entry> dict;

private:
  const std::string dictPath = "tests/dicts/dict05.txt";
};

TEST_F(Base, SimpleBuild)
{
  SimpleTrieBuilder tb(dict);
  tb.Build();
}

TEST_F(Base, LockedParralelBuild)
{
  LockedCpp11TrieBuilder tb(dict);
  tb.Build();
}

// TEST_F(Base, LockFreeCpp11ParralelBuild)
// {
//   LockfreeCpp11TrieBuilder tb(dict);
//   tb.Build();
// }

// TEST_F(Base, TbbParralelBuild)
// {
//   TbbParallelTrieBuilder tb(dict);
//   tb.Build();
// }

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
