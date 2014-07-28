#ifndef TRIE_HH
# define TRIE_HH

#include <common.hh>
#include <dict.hh>
#include <tbb/concurrent_hash_map.h>

class Trie
{
public:
  Trie(std::string serializedPath);

  void Save(std::ostream out);
};

struct TrieNode
{
  std::vector<std::string> GetKeys();
  const TrieNode* GetChild(std::string symb);

  std::map<std::string, TrieNode> edges;
  bool isOutNode = false;
};

struct TbbTrieNode
{
  std::vector<std::string> GetKeys();
  const TbbTrieNode* GetChild(std::string symb);

  tbb::concurrent_hash_map<std::string, TbbTrieNode> edges;
  bool isOutNode = false;
};

class TrieBuilder
{
public:
  TrieBuilder(const std::vector<Entry>& dict);
  virtual void Build();
  void ToGraphViz(std::string fileName);
  const TrieNode& GetRoot();
  virtual void Compact();
private:
  void Merge();
protected:
  const std::vector<Entry> _dict;
  TrieNode _root;
};

class TbbParallelTrieBuilder : public TrieBuilder
{
public:
  TbbParallelTrieBuilder(const std::vector<Entry>& dict) : TrieBuilder(dict) {};
  void Build();
  void TbbToGraphViz(std::string fileName);
  const TbbTrieNode& GetRoot();
  void Compact();
private:
  TbbTrieNode _tbbRoot;
};

class LockedCpp11TrieBuilder : public TrieBuilder
{
public:
  LockedCpp11TrieBuilder(const std::vector<Entry>& dict) : TrieBuilder(dict) {};
  void Build();
  void Compact();
};


#include <simpleTrie.hh>
#include <locklessTrie.hh>
#include <lockedTrie.hh>

#endif /* !TRIE_HH */
