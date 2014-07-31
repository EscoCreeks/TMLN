#include <gtest/gtest.h>
#include <common.hh>
#include <trie.hh>
#include <search.hh>

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
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
  const std::string dictPath = "tests/dicts/10000.md5.txt";
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

TEST_F(Base, LockedBuild)
{
  LockedTrieBuilder tb(dict);
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
  const std::string dictPath = "tests/dicts/dict10000.txt";
};

TEST_F(Compare, SimpleBuild)
{
  SimpleTrieBuilder tb(dict);
  tb.Build();
  TestTrie(refTrie, tb.GetRoot());
}

TEST_F(Compare, TbbPBuild)
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
  const std::string dictPath = "tests/dicts/10000.md5.txt";
};

TEST_F(CompareCompact, SimpleCompact)
{
  SimpleTrieBuilder tb(dict);
  tb.Build();
  tb.Compact();
}



TEST_F(CompareCompact, LockedCompact)
{
  LockedTrieBuilder tb(dict);
  tb.Build();
  tb.Compact();
}

TEST_F(CompareCompact, TbbPCompact)
{
  LocklessTrieBuilder tb(dict);
  tb.Build();
  tb.Compact();
}

class Serialize : public testing::Test
{
protected:
  virtual void SetUp()
  {
  }

private:
};

TEST_F(Serialize, Serialization)
{
  const std::string dictPath = "tests/dicts/toosimple.txt";
  std::ifstream dictStream(dictPath);
  ASSERT_TRUE(dictStream.is_open());
  std::vector<Entry> dict = ParseDict(dictStream);
  dictStream.close();
  EXPECT_FALSE(dict.empty());
  RecordProperty("EntryCount", dict.size());

  SimpleTrieBuilder tb(dict);
  tb.Build();
  tb.Compact();

  Trie trie(tb.Serialize());
}

TEST_F(Serialize, Count)
{
  const std::string dictPath = "tests/dicts/toosimple.txt";
  std::ifstream dictStream(dictPath);
  ASSERT_TRUE(dictStream.is_open());
  std::vector<Entry> dict = ParseDict(dictStream);
  dictStream.close();
  EXPECT_FALSE(dict.empty());
  RecordProperty("EntryCount", dict.size());

  SimpleTrieBuilder tb(dict);
  tb.Build();
  tb.Compact();

  Trie trie(tb.Serialize());

  ASSERT_EQ(trie.GetElementCount(), 2);
}

TEST_F(Serialize, GotoChild)
{
  const std::string dictPath = "tests/dicts/toosimple.txt";
  std::ifstream dictStream(dictPath);
  ASSERT_TRUE(dictStream.is_open());
  std::vector<Entry> dict = ParseDict(dictStream);
  dictStream.close();
  EXPECT_FALSE(dict.empty());
  RecordProperty("EntryCount", dict.size());

  SimpleTrieBuilder tb(dict);
  tb.Build();
  tb.Compact();

  Trie trie(tb.Serialize());
  ASSERT_EQ(trie.GetElementCount(), 2);
  int count = trie.GetElements()[1].GetTrie().GetElementCount();
  ASSERT_EQ(count, 3);
}

TEST_F(Serialize, OutNode)
{
  const std::string dictPath = "tests/dicts/toosimple.txt";
  std::ifstream dictStream(dictPath);
  ASSERT_TRUE(dictStream.is_open());
  std::vector<Entry> dict = ParseDict(dictStream);
  dictStream.close();
  EXPECT_FALSE(dict.empty());
  RecordProperty("EntryCount", dict.size());

  SimpleTrieBuilder tb(dict);
  tb.Build();
  tb.Compact();

  Trie trie(tb.Serialize());
  ASSERT_EQ(trie.GetElementCount(), 2);
  ASSERT_EQ(trie.GetElements()[0].IsOutNode(), true);
  ASSERT_EQ(trie.GetElements()[1].IsOutNode(), false);
  Trie sub = trie.GetElements()[1].GetTrie();
  ASSERT_EQ(trie.GetElements()[0].IsOutNode(), true);
}

TEST_F(Serialize, Leaf)
{
  const std::string dictPath = "tests/dicts/remove.txt";
  std::ifstream dictStream(dictPath);
  ASSERT_TRUE(dictStream.is_open());
  std::vector<Entry> dict = ParseDict(dictStream);
  dictStream.close();
  EXPECT_FALSE(dict.empty());
  RecordProperty("EntryCount", dict.size());

  SimpleTrieBuilder tb(dict);
  tb.Build();
  tb.Compact();
  Trie t = tb.Serialize();
  ASSERT_EQ(t.GetElementCount(), 2);
  ASSERT_EQ(t.GetElements()[0].GetTrie().GetElementCount(),2);
  ASSERT_EQ(t.GetElements()[0].IsLeaf(),false);
  ASSERT_EQ(t.GetElements()[0].GetTrie().GetElements()[0].IsLeaf(),true);
  ASSERT_EQ(t.GetElements()[0].GetTrie().GetElements()[1].IsLeaf(),true);
}

class Searching : public testing::Test
{
  void SetUp()
  {
    std::ifstream dictStream(dictPath);
    ASSERT_TRUE(dictStream.is_open());
    dict = ParseDict(dictStream);
    dictStream.close();
    EXPECT_FALSE(dict.empty());
    RecordProperty("EntryCount", dict.size());
    SimpleTrieBuilder tb(dict);
    tb.Build();
    tb.Compact();
    trie = new Trie(tb.Serialize());
  }
  void TearDown()
  {
    delete trie;
  }
protected:
  Trie* trie;
  std::vector<Entry> dict;
  const std::string dictPath = "tests/dicts/remove.txt";
};

void trieprinter(Trie t)
{
  static int index  = 0;
  int curr_index = index++;
  for (int i = 0; i < t.GetElementCount(); ++i)
  {
    std::cerr << curr_index << " -> " << t.GetElements()[i].GetStr() << " --> ";
    if (!t.GetElements()[i].IsLeaf())
      trieprinter(t.GetElements()[i].GetTrie());
    std::cerr << std::endl;
  }
}

TEST_F(Searching, Remove)
{
  char* word = "abcdefg";
  char* buff = "";
  char** stack = new char*[1024];
  stack[0] = nullptr;
  std::priority_queue<ResultElement> results;
  for (int i = 0; i < trie->GetElementCount(); ++i)
  {
    stack[1] = trie->GetElements()[i].GetStr();
    Search(results, trie->GetElements()[i], word, stack[1], 0, 1, stack+1);
  }
  ASSERT_EQ(results.size(), 3);
  //PrintResults(results);
  delete[] stack;
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
