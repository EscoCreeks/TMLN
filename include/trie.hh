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
  std::vector<Entry> _dict;
  TrieNode _root;
};

#endif /* !TRIE_HH */
