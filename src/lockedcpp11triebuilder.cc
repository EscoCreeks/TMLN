#include <trie.hh>
#include <tbb/parallel_for.h>

void ParallelAddTrie(TrieNode& root, const Entry& entry)
{
  std::string word = entry.str;
  TrieNode* node = &root;
  for (int i = 0; i < word.size(); ++i){
    node = &node->edges[std::string(1,word[i])];
  }
  node->isOutNode = true;
}

void LockedCpp11TrieBuilder::Build()
{
  TrieNode& root = _root;
  for (const Entry& entry : _dict){
    ParallelAddTrie(root, entry);
  }
}

void ParallelCompactNode(TrieNode& prec, const std::string keyFather, TrieNode& curr, bool& toDelete)
{
  if (curr.edges.size() == 1 && !curr.isOutNode)
  {
    std::string newKey = keyFather + curr.edges.begin()->first;
    prec.edges[newKey] = curr.edges.begin()->second;
    prec.edges.erase(keyFather);
    toDelete = true;
    bool toDeleteRec = false;
    ParallelCompactNode(prec, newKey, prec.edges[newKey], toDeleteRec);
  }
  else {
    std::vector<std::string> keys;
    for (auto item : curr.edges)
      keys.push_back(item.first);
    tbb::parallel_for(tbb::blocked_range<size_t>(0,keys.size()),
        [=,&curr](const tbb::blocked_range<size_t>& r){
          for (size_t i = r.begin(); i != r.end(); ++i)
          {
            std::string myKey = keys[i];
            bool toDeleteRec = false;
            ParallelCompactNode(curr, myKey, curr.edges[myKey], toDeleteRec);
            if (toDelete)
              curr.edges.erase(myKey);
          }
      });
  }
}

void LockedCpp11TrieBuilder::Compact()
{
  std::vector<std::string> keys;
  for (auto& item : _root.edges)
    keys.push_back(item.first);

  TrieNode& root = _root;
  tbb::parallel_for(tbb::blocked_range<size_t>(0,keys.size()),
      [=,&root](const tbb::blocked_range<size_t>& r){
        for (size_t i = r.begin(); i != r.end(); ++i)
        {
          std::string myKey = keys[i];
          bool toDelete = false;
          ParallelCompactNode(root, myKey, root.edges[myKey], toDelete);
          if (toDelete)
            root.edges.erase(myKey);
        }
    });
}
