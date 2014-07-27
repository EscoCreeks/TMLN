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
