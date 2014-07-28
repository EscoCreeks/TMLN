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

void StartCompactTrie(LockedTrieNode& root, const std::string keyFather, LockedTrieNode* curr, bool& toDelete);

void ParallelCompactNode(LockedTrieNode& prec, const std::string keyFather, LockedTrieNode* curr, bool& toDelete)
{
  if (curr->edges.size() == 1 && !curr->isOutNode)
  {
    LockedTrieNode* nnode = curr;
    LockedTrieNode* precnode = &prec;
    std::string newKey = keyFather;
    std::string precKey = keyFather;
    do
    {
      nnode->mutex.lock();
      precnode->mutex.unlock();
      if (!toDelete)
        toDelete = true;
      else {
        auto range = precnode->edges.equal_range(precKey);
        if (range.first != precnode->edges.end()){
          range.first->second = nullptr;
          precnode->edges.erase(precKey);
        }
      }
      precnode = nnode;
      precKey = precnode->edges.begin()->first;
      newKey += nnode->edges.begin()->first;
      nnode = nnode->edges.begin()->second;
    } while (nnode->edges.size() == 1 && !nnode->isOutNode);
    prec.edges[newKey] = nnode;
    toDelete = true;
    bool toDeleteRec = false;
    nnode->mutex.unlock();
    StartCompactTrie(prec, newKey, nnode, toDeleteRec);
    if (toDeleteRec)
      prec.edges.erase(newKey);
  }
  else {
    prec.mutex.unlock();
    std::vector<std::string> keys;
    for (auto item : curr->edges)
      keys.push_back(item.first);
    tbb::parallel_for(tbb::blocked_range<size_t>(0,keys.size()),
        [=,&curr](const tbb::blocked_range<size_t>& r){
        for (size_t i = r.begin(); i != r.end(); ++i)
        {
          bool toDeleteRec = false;
            StartCompactTrie(*curr, keys[i], curr->edges[keys[i]], toDeleteRec);
            if (toDeleteRec)
              curr->edges.erase(keys[i]);
        }
        });
  }
}

void StartCompactTrie(LockedTrieNode& root, const std::string keyFather, LockedTrieNode* curr, bool& toDelete)
{
  root.mutex.lock();
  ParallelCompactNode(root, keyFather, root.edges[keyFather], toDelete);
}

void LockedTrieBuilder::Compact()
{
  std::vector<std::string> keys;
  for (auto& item : _root.edges)
    keys.push_back(item.first);

  LockedTrieNode& root = _root;
  tbb::parallel_for(tbb::blocked_range<size_t>(0,keys.size()),
      [=,&root](const tbb::blocked_range<size_t>& r){
      for (size_t i = r.begin(); i != r.end(); ++i)
      {
          bool toDelete = false;
          StartCompactTrie(root, keys[i], root.edges[keys[i]], toDelete);
          if (toDelete)
            root.edges.erase(keys[i]);
      }
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
