#ifndef TRIE_HH
# define TRIE_HH

#include <common.hh>
#include <dict.hh>

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

class TrieBuilder
{
public:
  TrieBuilder(const std::vector<Entry>& dict);
  void Build();
  void ParallelBuild();
  void ToGraphViz();
private:
  void Merge();
  void Compact();
  void ParallelCompact();
  std::vector<Entry> _dict;
  TrieNode _root;
};

class LockfreeCpp11TrieBuilder : public TrieBuilder
{
public:
  LockfreeCpp11TrieBuilder(std::vector<Entry>& dict);
  void Build();
};

#endif /* !TRIE_HH */
