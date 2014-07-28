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
  }

  std::vector<Entry> dict;
  TrieNode refTrie;

private:
  //const std::string dictPath = "assignment/words.txt";
  const std::string dictPath = "tests/dicts/dict10000.txt";
};

template<class T1, class T2>
void TestTrie(const T1& trieNodeRef, const T2& trieNodeTest)
{
  ASSERT_EQ(trieNodeRef.isOutNode, trieNodeTest.isOutNode);
  auto refKeys = trieNodeRef.GetKeys();
  auto testKeys = trieNodeTest.GetKeys();
  ASSERT_EQ(refKeys.size(), testKeys.size());
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
}

TEST_F(Base, LocklessParralelBuild)
{
  LocklessTrieBuilder tb(dict);
  tb.Build();
}

TEST_F(Base, LockedParralelBuild)
{
  LockedTrieBuilder tb(dict);
  tb.Build();
}

TEST_F(Base, TbbParralelBuild)
{
  TbbParallelTrieBuilder tb(dict);
  tb.Build();
}

class Compare : public testing::Test
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

    SimpleTrieBuilder rtb(dict);
    rtb.Build();
    refTrie = rtb.GetRoot();
  }

  std::vector<Entry> dict;
  SimpleTrieNode refTrie;

private:
  //const std::string dictPath = "assignment/words.txt";
  const std::string dictPath = "tests/dicts/dict10000.txt";
};

TEST_F(Compare, SimpleBuild)
{
  SimpleTrieBuilder tb(dict);
  tb.Build();
  TestTrie(refTrie, tb.GetRoot());
}

TEST_F(Compare, LockLessBuild)
{
  LocklessTrieBuilder tb(dict);
  tb.Build();
  TestTrie(refTrie, tb.GetRoot());
}

TEST_F(Compare, LockedBuild)
{
  LockedTrieBuilder tb(dict);
  tb.Build();
  TestTrie(refTrie, tb.GetRoot());
}

TEST_F(Compare, TbbBuild)
{
  LocklessTrieBuilder tb(dict);
  tb.Build();
  TestTrie(refTrie, tb.GetRoot());
}

class CompareCompact : public testing::Test
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

    SimpleTrieBuilder rtb(dict);
    rtb.Build();
    rtb.Compact();
    refTrie = rtb.GetRoot();
  }

  std::vector<Entry> dict;
  SimpleTrieNode refTrie;

private:
  //const std::string dictPath = "assignment/words.txt";
  const std::string dictPath = "tests/dicts/dict10.txt";
};

TEST_F(CompareCompact, SimpleCompact)
{
  SimpleTrieBuilder tb(dict);
  tb.Build();
  tb.Compact();
  TestTrie(refTrie, tb.GetRoot());
}

TEST_F(CompareCompact, LockLessCompact)
{
  LocklessTrieBuilder tb(dict);
  tb.Build();
  tb.Compact();
  TestTrie(refTrie, tb.GetRoot());
}

TEST_F(CompareCompact, TbbCompact)
{
  LocklessTrieBuilder tb(dict);
  tb.Build();
  tb.Compact();
  TestTrie(refTrie, tb.GetRoot());
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
