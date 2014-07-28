#include <trie.hh>
#include <tbb/parallel_for.h>

void AddTrie(LockedTrieNode& root, const Entry& entry)
{
  std::string word = entry.str;
  LockedTrieNode* node = &root;
  for (int i = 0; i < word.size(); ++i){
    LockedTrieNode*& nnode = node->edges[std::string(1,word[i])];
    if (nnode == nullptr)
      nnode = new LockedTrieNode();

    nnode->mutex.lock();
    node->mutex.unlock();
    node = nnode;
  }
  node->isOutNode = true;
  node->mutex.unlock();
}

void StartAddTrie(LockedTrieNode& root, const Entry& entry)
{
  root.mutex.lock();
  AddTrie(root, entry);
}
LockedTrieNode::LockedTrieNode()
  : edges(), mutex()
{
}

void LockedTrieBuilder::Build()
{
  LockedTrieNode& root = _root;

  tbb::parallel_for(tbb::blocked_range<size_t>(0,_dict.size()),
                    [&](const tbb::blocked_range<size_t>& r){
                      for (size_t i = r.begin(); i != r.end(); ++i)
                        StartAddTrie(_root, _dict[i]);
                    });
}

const std::vector<std::string> LockedTrieNode::GetKeys() const
{
  std::vector<std::string> keys;
  for (auto edge : edges)
    keys.push_back(edge.first);
  return keys;
}

const LockedTrieNode* LockedTrieNode::GetChild(const std::string& key) const
{
  decltype(edges)::const_iterator it = edges.find(key);
  if (it == edges.end())
    return nullptr;
  else
    return it->second;
}
