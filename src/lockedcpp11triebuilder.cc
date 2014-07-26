#include <trie.hh>

void ParallelAddTrie(TrieNode& root, const Entry& entry)
{
  static std::mutex mutex;
  std::string word = entry.str;
  TrieNode* node = &root;
  for (int i = 0; i < word.size(); ++i){
    mutex.lock();
    node = &node->edges[std::string(1,word[i])];
    mutex.unlock();
  }
  node->isOutNode = true;
}

void LockedCpp11TrieBuilder::Build()
{
  {
    time_guard tg("locked construction: ");
    TrieNode& root = _root;
    std::vector<std::thread> threads;
    for (const Entry& entry : _dict)
    {
      threads.push_back(std::thread([=,&root](){
            ParallelAddTrie(root, entry);
            }));
    }
    for (auto& th: threads)
      th.join();
  }
  {
    time_guard tg("locked compaction: ");
    Compact();
  }
}

void ParallelCompactNode(TrieNode& prec, const std::string keyFather, TrieNode& curr)
{
  if (curr.edges.size() == 1 && !curr.isOutNode)
  {
    std::string newKey = keyFather + curr.edges.begin()->first;
    prec.edges[newKey] = curr.edges.begin()->second;
    prec.edges.erase(keyFather);
    ParallelCompactNode(prec, newKey, prec.edges[newKey]);
  }
  else {
    std::vector<std::thread> threads;
    std::vector<std::string> keys;
    for (auto item : curr.edges)
      keys.push_back(item.first);
    for (int i = 0; i < keys.size(); ++i)
    {
      const std::string myKey = keys[i];
      threads.push_back(std::thread([=,&curr](){
            ParallelCompactNode(curr, myKey, curr.edges.at(myKey));
            }));
    }
    for (auto& th : threads)
      th.join();
  }
}

void LockedCpp11TrieBuilder::Compact()
{
  std::map<std::string, TrieNode>::iterator it;
  std::vector<std::thread> threads;
  std::vector<std::string> keys;
  for (it = _root.edges.begin(); it != _root.edges.end(); ++it)
    keys.push_back(it->first);

  for (int i = 0; i < keys.size(); ++i)
  {
    const std::string myKey = keys[i];
    threads.push_back(std::thread([=](){
          ParallelCompactNode(_root, myKey, _root.edges[myKey]);
          }));
  }
  for (auto& th : threads)
    th.join();
}
