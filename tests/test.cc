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
    dict = ParseDict(dictStream);
    dictStream.close();
    EXPECT_FALSE(dict.empty());
    RecordProperty("EntryCount", dict.size());
    // SimpleTrieBuilder tb(dict);
    // tb.Build();
    // refTrie = tb.GetRoot();
  }

  std::vector<Entry> dict;
  TrieNode refTrie;

private:
  const std::string dictPath = "assignment/words.txt";
  //const std::string dictPath = "tests/dicts/dict10.txt";
};

template<class T1, class T2>
void TestTrie(T1 trieNodeRef, T2 trieNodeTest)
{
  ASSERT_EQ(trieNodeRef.isOutNode, trieNodeTest.isOutNode);
  auto refKeys = trieNodeRef.GetKeys();
  for (std::string symb : refKeys)
  {
    const T1* newTrieNodeRef = trieNodeRef.GetChild(symb);
    ASSERT_TRUE(newTrieNodeRef != nullptr); // this should never happen but just in case

    const T2* newTrieNodeTest = trieNodeTest.GetChild(symb);
    ASSERT_TRUE(newTrieNodeTest != nullptr);
    TestTrie(*newTrieNodeRef, *newTrieNodeTest);
  }
}

TEST_F(Base, SimpleBuild)
{
  SimpleTrieBuilder tb(dict);
  tb.Build();
  //TestTrie(refTrie, tb.GetRoot());
}

TEST_F(Base, LockedParralelBuild)
{
  LockedCpp11TrieBuilder tb(dict);
  tb.Build();
  TestTrie(refTrie, tb.GetRoot());
}

TEST_F(Base, LocklessParralelBuild)
{
  LocklessTrieBuilder tb(dict);
  tb.Build();
  //TestTrie(refTrie, tb.GetRoot());
}

TEST_F(Base, TbbParralelBuild)
{
  TbbParallelTrieBuilder tb(dict);
  tb.Build();
  TestTrie(refTrie, tb.GetRoot());
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
