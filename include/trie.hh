#ifndef TRIE_HH
# define TRIE_HH

#include <common.hh>
#include <dict.hh>
#include <tbb/concurrent_hash_map.h>

class Trie;

class TrieElement
{
public:
  char* GetStr();
  void SetStrId(int id);
  void SetTrieOffset(int off);
  Trie GetTrie();
private:
  static char* __base;
  int _strId;
  int _trieOffset;
};


class Trie
{
public:
  Trie(std::string serializedPath);
  Trie(void* raw);

  int GetElementCount();
  TrieElement* GetElements();
private:
  void* _raw;
};

struct pNode {
  int count;
  TrieElement trieElt[];
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
