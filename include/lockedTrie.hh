#ifndef LOCKEDTRIE_HH
# define LOCKEDTRIE_HH

struct LockedTrieNode
{
  LockedTrieNode();
  // ~LockedTrieNode();
  // LockedTrieNode(const LockedTrieNode& base);
  // LockedTrieNode& operator=(const LockedTrieNode& base);

  const std::vector<std::string> GetKeys() const;
  const LockedTrieNode* GetChild(const std::string& key) const;

  std::map<std::string, LockedTrieNode*> edges;
  bool isOutNode = false;
  std::mutex mutex;
};

class LockedTrieBuilder
{
public:
  LockedTrieBuilder(const std::vector<Entry>& dict) : _dict(dict) {};
  void Build();
  void Compact();
  LockedTrieNode& GetRoot() { return _root; }
private:
  std::vector<Entry> _dict;
  LockedTrieNode _root;
};

#endif /* !LOCKEDTRIE_HH */
