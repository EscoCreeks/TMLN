#ifndef TRIE_HH
# define TRIE_HH

#include <common.hh>
#include <dict.hh>

class Trie;

class TrieElement
{
public:
  TrieElement(int strId, int freq);
  char* GetStr();
  Trie GetTrie();
  int GetFreq();
  bool IsOutNode();
  bool IsLeaf();
  void SetTrieOffset(int off);
private:
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
  static char* _strBase_;
private:
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
  unsigned char distance;
  char* word;
};

#include <simpleTrie.hh>
#endif /* !TRIE_HH */
