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
  std::map<std::string, TrieNode> edges;
  bool isOutNode = false;
};

struct TbbTrieNode
{
  tbb::concurrent_hash_map<std::string, TbbTrieNode> edges;
  bool isOutNode = false;
};

class TrieBuilder
{
public:
  TrieBuilder(const std::vector<Entry>& dict);
  virtual void Build();
  void ToGraphViz(std::string fileName);
private:
  void Merge();
  virtual void Compact();
protected:
  std::vector<Entry> _dict;
  TrieNode _root;
};

class SimpleTrieBuilder : public TrieBuilder
{
public:
  SimpleTrieBuilder(const std::vector<Entry>& dict) : TrieBuilder(dict) {};
  void Build();
private:
  void Compact();
};

class LockedCpp11TrieBuilder : public TrieBuilder
{
public:
  LockedCpp11TrieBuilder(const std::vector<Entry>& dict) : TrieBuilder(dict) {};
  void Build();
private:
  void Compact();
};

class LockfreeCpp11TrieBuilder : public TrieBuilder
{
public:
  LockfreeCpp11TrieBuilder(std::vector<Entry>& dict);
  void Build();
};

class TbbParallelTrieBuilder : public TrieBuilder
{
public:
  TbbParallelTrieBuilder(const std::vector<Entry>& dict) : TrieBuilder(dict) {};
  void Build();
  void TbbToGraphViz(std::string fileName);
private:
  void Compact();
  TbbTrieNode _tbbRoot;
};

#endif /* !TRIE_HH */
