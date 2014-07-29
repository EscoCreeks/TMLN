#ifndef TRIE_HH
# define TRIE_HH

#include <common.hh>
#include <dict.hh>
#include <tbb/concurrent_hash_map.h>


class TrieElement
{
public:
  char* GetStr();
private:
  static char* __base;
  int _str_id;
  int _trie_offset;
};


class Trie
{
public:
  Trie(std::string serializedPath);
  void Save(std::ostream& out);
private:
  int _count;
  TrieElement* _elt;
};

struct TrieNode
{
  std::vector<std::string> GetKeys();
  const TrieNode* GetChild(std::string symb);

  std::map<std::string, TrieNode> edges;
  bool isOutNode = false;
};

#include <totrash.hh>
#include <simpleTrie.hh>
#include <locklessTrie.hh>
#include <lockedTrie.hh>
#endif /* !TRIE_HH */
