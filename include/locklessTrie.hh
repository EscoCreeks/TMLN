#ifndef LOCKLESSTRIE_HH
# define LOCKLESSTRIE_HH

#include <tbb/concurrent_hash_map.h>

struct LocklessTrieNode
{
  tbb::concurrent_hash_map<std::string, LocklessTrieNode> edges;
  bool isOutNode = false;
};

class LocklessTrieBuilder : TrieBuilder
{
public:
  LocklessTrieBuilder(const std::vector<Entry>& dict);
  void Build();
private:
  LocklessTrieNode _root;
};



#endif /* !LOCKLESSTRIE_HH */
