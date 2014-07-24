#include <trie.hh>

Trie::Trie(std::string serializedPath)
{
  NOT_IMPLEMENTED();
}

void Trie::Save(std::ostream out)
{
  NOT_IMPLEMENTED();
}

TrieBuilder::TrieBuilder(const std::vector<Entry>& dict)
  : _dict(dict)
{
}

void PAddTrie(TrieNode& root, const Entry& entry)
{
  std::string word = entry.str;
  TrieNode* node = &root;
  for (int i = 0; i < word.size(); ++i){
    node = &node->edges[std::string(1,word[i])];
  }
  node->isOutNode = true;
}

void TrieBuilder::Build()
{
  TrieNode& root = _root;
  for (const Entry& entry : _dict)
  {
    PAddTrie(root, entry);
  }
  Compact();
}

void TrieBuilder::ParallelBuild()
{
  TrieNode& root = _root;
  for (const Entry& entry : _dict)
  {
    PAddTrie(root, entry);
  }
  ParallelCompact();
}

void TrieBuilder::Merge()
{
  NOT_IMPLEMENTED();
}

void CompactNode(TrieNode& prec, std::string keyFather, TrieNode& curr, std::map<std::string, TrieNode>::iterator &it)
{
  if (curr.edges.size() == 1 && !curr.isOutNode)
  {
    auto next = it;
    ++next;
    std::string newKey = keyFather + curr.edges.begin()->first;
    prec.edges[newKey] = curr.edges.begin()->second;
    prec.edges.erase(keyFather);
    CompactNode(prec, newKey, prec.edges[newKey], it);
    it = --next;
  }
  else {
    for (auto item : curr.edges)
      CompactNode(curr, item.first, item.second, it);
  }
}

void ParallelCompactNode(TrieNode& prec, std::string keyFather, TrieNode& curr)
{
  static std::mutex mutex;
  mutex.lock();
  std::cout << std::this_thread::get_id() << std::endl;
  mutex.unlock();
  if (curr.edges.size() == 1 && !curr.isOutNode)
  {
    std::string newKey = keyFather + curr.edges.begin()->first;
    prec.edges[newKey] = curr.edges.begin()->second;
    prec.edges.erase(keyFather);
    mutex.unlock();
    ParallelCompactNode(prec, newKey, prec.edges[newKey]);
  }
  else {
    std::vector<std::thread> threads;
    std::vector<std::string> keys;
    for (auto item : curr.edges)
      keys.push_back(item.first);
    for (auto key : keys)
    {
      threads.push_back(std::thread([&](){
            ParallelCompactNode(curr, key, curr.edges[key]);
            }));
    }
    for (auto& th : threads)
      th.join();
  }
}

void TrieBuilder::Compact()
{
  std::map<std::string, TrieNode>::iterator it;
  for (it = _root.edges.begin(); it != _root.edges.end(); ++it)
    CompactNode(_root, it->first, it->second, it);
}

void TrieBuilder::ParallelCompact()
{
  std::map<std::string, TrieNode>::iterator it;
  std::vector<std::thread> threads;
  std::vector<std::string> keys;
  for (it = _root.edges.begin(); it != _root.edges.end(); ++it)
    keys.push_back(it->first);
  for (auto key : keys)
  {
    threads.push_back(std::thread([&](){
          ParallelCompactNode(_root, key, _root.edges[key]);
          }));
  }
  for (auto& th : threads)
    th.join();
}

void NodeToGraphViz(std::ofstream& os, std::string prec, const TrieNode& node, int& nb, bool root)
{
  const int n = root ? 0 : nb;
  bool used = false;
  for (auto item : node.edges){
    used = true;
    if (node.isOutNode){
      os << ++nb << " [shape=doublecircle]" << std::endl;
      os << n << " -> " << nb << " [label=" << prec << "]" << std::endl;
    }
    else {
      os << n << " -> " << ++nb << " [label=" << prec << "]" << std::endl;
    }
    NodeToGraphViz(os, item.first, item.second, nb, false);
  }
  if (node.isOutNode && !used){
    os << ++nb << " [shape=doublecircle]" << std::endl;
    os << n << " -> " << nb << " [label=" << prec << "]" << std::endl;
  }
}

void TrieBuilder::ToGraphViz()
{
  std::ofstream file("builder.dot", std::ios::out | std::ios::trunc);
  if (file.is_open()){
    file << "digraph triebuilder {" << std::endl;
    //std::map<std::string, int> occ;
    int nb = 0;
    for (auto item : _root.edges){
      NodeToGraphViz(file, item.first, item.second, nb, true);
    }
    file << "}";
  }
  else
    std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
}
