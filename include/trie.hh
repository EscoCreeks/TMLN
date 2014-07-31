#ifndef TRIE_HH
# define TRIE_HH

#include <common.hh>
#include <dict.hh>
#include <tbb/concurrent_hash_map.h>

class Trie;

class TrieElement
{
public:
  TrieElement(int strId, int freq);
  char* GetStr();
  Trie GetTrie();
  int GetFreq();
  bool IsOutNode();

  void SetTrieOffset(int off);
private:
  static char* __base;
  int _strId;
  int _trieOffset;
  int _freq;
};


class Trie
{
public:
  Trie(std::string serializedPath);
  Trie(void* raw);
  Trie(void* strBase, void* raw);

  int GetElementCount();
  TrieElement* GetElements();
  struct searchRes** SearchQuerry(char* querry, int error);
private:
  static char* _strBase_;
  void* _raw;
};

struct pNode {
  int count;
  TrieElement trieElt[0];
};

struct TrieNode
{
  std::vector<std::string> GetKeys();
  const TrieNode* GetChild(std::string symb);

  std::map<std::string, TrieNode> edges;
  bool isOutNode = false;
};

struct searchRes {
  int freq;
  unsigned char error;
  char* word;
};

#include <totrash.hh>
#include <simpleTrie.hh>
#include <locklessTrie.hh>
#include <lockedTrie.hh>
#endif /* !TRIE_HH */
