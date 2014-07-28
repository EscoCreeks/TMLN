#ifndef LOCKLESSTRIE_HH
# define LOCKLESSTRIE_HH

#include <tbb/concurrent_hash_map.h>

struct LocklessTrieNode
{
  LocklessTrieNode();
  ~LocklessTrieNode();
  LocklessTrieNode(const LocklessTrieNode& base);
  LocklessTrieNode& operator=(const LocklessTrieNode& base);

  const std::vector<std::string> GetKeys() const;
  const LocklessTrieNode* GetChild(const std::string& key) const;

  tbb::concurrent_hash_map<std::string, LocklessTrieNode*> edges;
  bool isOutNode = false;
};

class LocklessTrieBuilder : TrieBuilder
{
public:
  LocklessTrieBuilder(const std::vector<Entry>& dict);
  void Build();
  void Compact();
  LocklessTrieNode& GetRoot() { return _root; }
private:
  LocklessTrieNode _root;
};



#endif /* !LOCKLESSTRIE_HH */
