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

class TrieBuilder
{
public:
  TrieBuilder(const std::vector<Entry>& dict);
  Trie Build();
  Trie ParallelBuild();
private:
  void Merge();
  void Compact();

  std::vector<Entry> _dict;
};

struct TrieNode
{
  std::map<std::string, TrieNode> childs;
};

#endif /* !TRIE_HH */
