#include <trie.hh>

LocklessTrieBuilder::LocklessTrieBuilder(const std::vector<Entry>& dict)
  : TrieBuilder(dict)
{
}

void AddTrie(LocklessTrieNode& root, const Entry& entry)
{
  LocklessTrieNode* node = &root;
  for (const char symb : entry.str)
  {
    decltype(node->edges)::accessor acc;
    node->edges.insert(acc, std::string(symb, 1));
    if (acc->second == nullptr)
      node = (acc->second = new LocklessTrieNode());
    else
      node = acc->second;
    acc.release();
  }
  node->isOutNode = true;
}

void LocklessTrieBuilder::Build()
{
  for (const Entry& entry : _dict)
    AddTrie(_root, entry);
}


LocklessTrieNode::LocklessTrieNode()
  : edges()
{
}

LocklessTrieNode::~LocklessTrieNode()
{
  for (auto edge : edges)
    delete edge.second;
}

LocklessTrieNode::LocklessTrieNode(const LocklessTrieNode& base)
{
  tbb::concurrent_hash_map<std::string, LocklessTrieNode*> nedges;
  for (auto edge : base.edges)
  {
    decltype(nedges)::accessor acc;
    nedges.insert(acc, edge.first);
    acc->second = new LocklessTrieNode(*edge.second);
  }
  edges = nedges;
  isOutNode = base.isOutNode;
}

LocklessTrieNode& LocklessTrieNode::operator=(const LocklessTrieNode& base)
{
  tbb::concurrent_hash_map<std::string, LocklessTrieNode*> nedges;
  for (auto edge : base.edges)
  {
    decltype(nedges)::accessor acc;
    nedges.insert(acc, edge.first);
    acc->second = new LocklessTrieNode(*edge.second);
  }
  edges = nedges;
  isOutNode = base.isOutNode;
  return *this;
}

const std::vector<std::string> LocklessTrieNode::GetKeys() const
{
  std::vector<std::string> keys;
  for (auto edge : edges)
    keys.push_back(edge.first);
  return keys;
}

const LocklessTrieNode* LocklessTrieNode::GetChild(const std::string& key) const
{
  decltype(edges)::accessor acc;
  if (edges.find(acc, key))
    return acc->second;
  else
    return nullptr;
}
